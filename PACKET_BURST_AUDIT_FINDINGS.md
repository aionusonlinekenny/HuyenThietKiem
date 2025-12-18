# Packet Burst DDoS Audit - Findings Report

## Executive Summary

This audit identified the root causes of excessive packet bursts that trigger VPS/WAF rate limiting and lead to IP blocks. The primary issue is **reconnect spam** sending 3 immediate connection attempts without backoff. Secondary issues involve PING retry mechanisms that have been partially fixed but need refinement.

---

## Critical Findings

### ISSUE #1: Reconnect Burst Spam ⚠️ **CRITICAL**

**Location:** `SwordOnline/Sources/S3Client/Ui/UiCase/UiReconnect.cpp:302-307`

**Root Cause:**
When a client disconnects (e.g., server change, connection loss), the reconnect logic attempts **3 immediate reconnections with ZERO delay** between attempts.

**Evidence:**
```cpp
if (m_nChangeWorldReconnectTimes < 3)
{
    StartReconnect();  // ← IMMEDIATE retry, no delay!
    m_nChangeWorldReconnectTimes++;
    return;
}
```

**Disabled Delay Logic (lines 291-301):**
The code shows a commented-out delay mechanism that was previously implemented but disabled:
```cpp
/* DISABLED CODE:
unsigned int now = IR_GetCurrentTime();
if (now - m_uLastReconnectTick >= m_uLastReconnectTime)
{
    StartReconnect();
    m_uLastReconnectTick = now;
    m_nChangeWorldReconnectTimes++;
    return;
}
*/
```

**Attack Pattern:**
1. Client disconnects → UiReconnect::FirstReconnect() called
2. Loop starts calling Breathe() every frame (~16ms)
3. Lines 302-307: 3x StartReconnect() called **immediately** (no delay)
4. Each StartReconnect() triggers:
   - g_LoginLogic.ReturnToIdle()
   - g_LoginLogic.AutoLogin() → sends TCP SYN + account login packets
5. **Result:** 3 full connection handshakes + login packets in <50ms
6. **VPS/WAF sees:** Burst of ~9-12 packets from same IP in <50ms → Flags as DDoS → Blocks IP

**Impact:**
- Multiple clients disconnecting simultaneously (e.g., server restart) cause 3N immediate reconnect bursts
- WAF rate limits typically allow 10-20 packets/second per IP
- 3 clients × 3 retries × 4 packets each = 36 packets in <50ms = **720 packets/second burst** → Instant IP block

---

### ISSUE #2: PING Retry Tolerance (PARTIALLY FIXED)

**Location:** `SwordOnline/Sources/MultiServer/GameServer/KSOServer.cpp:2548-2596`

**Previous Issue:**
PING retry tolerance was set to 10 ticks, causing `PingClient()` to be called 10 times per retry window when a client was unresponsive.

**Fix Applied (lines 2575):**
```cpp
// REDUCED from 10 to 2!
int remainder = elapsed % defMAX_PING_INTERVAL;
if (remainder > 0 && remainder <= 2)  // Only 2 ticks tolerance now
{
    printf("[PING-RETRY] lnID=%d elapsed=%d remainder=%d -> resending ping\n",
           lnID, elapsed, remainder);
    PingClient(lnID);
}
```

**Status:** ✅ **FIXED** - Tolerance reduced from 10 ticks to 2 ticks (from ~166ms window to ~33ms window)

**Before:** 10 retry packets per interval = potential for 10+ PING packets in 166ms
**After:** 2 retry packets max per interval = max 2 PING packets in 33ms

---

### ISSUE #3: PING Interval (FIXED)

**Location:** `SwordOnline/Sources/MultiServer/GameServer/KSOServer.cpp:2551`

**Previous Issue:**
PING interval was too frequent (3.3 seconds), causing high packet rate.

**Fix Applied:**
```cpp
// FIX: Increase PING interval from 3.3s to 10s to reduce packet rate
// OLD: 10*20=200 ticks = 3.3 seconds → high frequency can trigger VPS rate limiting
// NEW: 30*20=600 ticks = 10 seconds → lower frequency, less likely to be flagged as spam
#define defMAX_PING_INTERVAL  30 * 20  // ← Changed from 10*20 to 30*20
```

**Status:** ✅ **FIXED** - Interval increased from 3.3s to 10s

**Before:** 18 PING packets/minute per client
**After:** 6 PING packets/minute per client (67% reduction)

---

### ISSUE #4: PING State Reset on Disconnect (FIXED)

**Location:** `SwordOnline/Sources/MultiServer/GameServer/KSOServer.cpp:2589-2595, 2625-2632`

**Previous Issue:**
When a player disconnected or timed out, PING timers were not reset, causing continued retry spam for dead connections.

**Fix Applied:**
```cpp
// FIX: Reset nGameStatus to prevent repeated PING-TIMEOUT logging
m_pGameStatus[lnID].nGameStatus = enumPlayerBegin;
m_pGameStatus[lnID].nPlayerIndex = 0;
m_pGameStatus[lnID].nSendPingTime  = 0;
m_pGameStatus[lnID].nReplyPingTime = 1;

// FIX: Reset ping timers to stop PING-RETRY spam for dead connections
m_pGameStatus[lnID].nSendPingTime = 0;
m_pGameStatus[lnID].nReplyPingTime = 1;  // Mark as inactive
```

**Status:** ✅ **FIXED** - State properly reset on disconnect/timeout

---

## Architecture Overview

### Server Game Loop (20 FPS, ~50ms per tick)
```
Breathe()
  → MainLoop()
    → m_pCoreServerShell->Breathe()
      → g_SubWorldSet.MainLoop()  // Game logic, NPC AI, skills
        → SubWorld[i].Activate()
    → PING handling (checks 1 player per tick)
    → m_pServer->SendPackToClient(-1)  // Flush packets every OTHER tick
```

### Client Reconnect Flow
```
Disconnect detected
  → UiReconnect::FirstReconnect()
    → m_bWaitToReconnect = true
    → Breathe() called every frame (~16ms)
      → StartReconnect() [ISSUE: Called 3x immediately!]
        → g_LoginLogic.AutoLogin()
          → ConnectToGameSvr()
            → TCP SYN + account login packets
```

---

## Packet Burst Scenarios

### Scenario A: Single Client Reconnect
```
Time    Event                           Packets Sent
0ms     Disconnect detected             0
0ms     StartReconnect() attempt #1     → TCP SYN, ACK, Login (4 packets)
16ms    StartReconnect() attempt #2     → TCP SYN, ACK, Login (4 packets)
32ms    StartReconnect() attempt #3     → TCP SYN, ACK, Login (4 packets)
----------------------------------------------------------
Total:  12 packets in 32ms = 375 packets/second burst rate
```

### Scenario B: 5 Clients Reconnect (Server Restart)
```
Time    Event                           Total Packets
0ms     5 clients disconnect            0
0-50ms  5 × 3 reconnect attempts        → 60 packets in 50ms
----------------------------------------------------------
Burst:  60 packets in 50ms = 1200 packets/second
Result: WAF blocks IP (typical threshold: 100-500 pps)
```

---

## Proposed Solutions

### Fix #1: Reconnect Backoff (REQUIRED)
**File:** `SwordOnline/Sources/S3Client/Ui/UiCase/UiReconnect.cpp`

**Change:** Re-enable the delay mechanism with exponential backoff:
```cpp
// Before (lines 302-307):
if (m_nChangeWorldReconnectTimes < 3)
{
    StartReconnect();  // ← No delay!
    m_nChangeWorldReconnectTimes++;
    return;
}

// After:
if (m_nChangeWorldReconnectTimes < 3)
{
    unsigned int now = IR_GetCurrentTime();
    unsigned int delay = 500 * (1 << m_nChangeWorldReconnectTimes); // 500ms, 1s, 2s
    if (now - m_uLastReconnectTick >= delay)
    {
        StartReconnect();
        m_uLastReconnectTick = now;
        m_nChangeWorldReconnectTimes++;
    }
    return;
}
```

**Impact:** Reduces reconnect burst from 375 pps to <2 pps per client

---

### Fix #2: Rate Telemetry (RECOMMENDED)
Add lightweight per-connection packet counters in `JXServer.cpp::BeginToSend()`:

```cpp
struct ConnectionStats {
    DWORD last_second;
    int packets_this_second;
    int max_burst;
} g_connStats[MAX_CLIENT_CANBELINKED];

// In BeginToSend(), before sending:
DWORD now = GetTickCount() / 1000;
if (g_connStats[ulnID].last_second != now) {
    if (g_connStats[ulnID].packets_this_second > 50) {
        printf("[BURST-WARN] Client %lu sent %d packets in 1s\n",
               ulnID, g_connStats[ulnID].packets_this_second);
    }
    g_connStats[ulnID].last_second = now;
    g_connStats[ulnID].packets_this_second = 0;
}
g_connStats[ulnID].packets_this_second++;
```

---

## Testing Recommendations

### Test Case 1: Single Client Reconnect
1. Client connects to server
2. Force disconnect (kill game server process)
3. Monitor client network traffic with Wireshark
4. **Expected:** 3 reconnect attempts with 500ms, 1s, 2s delays
5. **Verify:** No more than 4 packets per second sustained rate

### Test Case 2: Multi-Client Burst
1. Start 10 clients, all connected
2. Restart game server
3. Monitor aggregate traffic at VPS
4. **Expected:** Spread reconnects over 3-5 seconds
5. **Verify:** Peak rate < 100 packets/second

### Test Case 3: PING Spam Prevention
1. Client connects
2. Simulate network lag (delay packets by 2 seconds)
3. Monitor PING packet frequency
4. **Expected:** Max 2 retry PINGs per 10-second interval
5. **Verify:** No PING storms even with packet loss

---

## Summary

| Issue | Severity | Status | Impact |
|-------|----------|--------|--------|
| Reconnect burst spam | **CRITICAL** | ❌ **UNFIXED** | Causes IP blocks |
| PING retry tolerance | Medium | ✅ **FIXED** | Reduced spam by 80% |
| PING interval | Medium | ✅ **FIXED** | Reduced rate by 67% |
| PING state reset | Low | ✅ **FIXED** | Prevents zombie retry |

**Recommendation:** Apply Fix #1 (Reconnect Backoff) immediately. This is the primary cause of IP blocks.

---

## Code Locations Reference

### Critical Files:
1. **Reconnect logic:** `SwordOnline/Sources/S3Client/Ui/UiCase/UiReconnect.cpp:302-307`
2. **PING mechanism:** `SwordOnline/Sources/MultiServer/GameServer/KSOServer.cpp:2510-2728`
3. **Packet flush:** `SwordOnline/Sources/Core/Src/NetWork/JXServer.cpp:389-525`
4. **Game loop:** `SwordOnline/Sources/MultiServer/GameServer/KSOServer.cpp:817-859`

### Function Call Chain:
```
Reconnect: UiReconnect::Breathe() → StartReconnect() → AutoLogin() → ConnectToGameSvr()
PING: MainLoop() → PingClient() → PackDataToClient() → BeginToSend() → WSASend()
Flush: MainLoop() → SendPackToClient(-1) → BeginToSend() → Write() → WSASend()
```
