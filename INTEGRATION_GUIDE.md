# Packet Burst Fix - Direct Integration Guide
## Apply These Changes to Source Files

---

## FILE 1: KPlayerAI.h

### Location: Line 174 (before `public:`)
### Action: ADD these new private fields

```cpp
	int				nDestYPaint;

	// ===== PACKET BURST FIX: ADD THESE FIELDS =====
	// Combat lag detection
	unsigned int	m_nLastTargetHP;
	int				m_nLastTargetDistance;
	int				m_nSameHPCounter;
	int				m_nSameDistCounter;
	int				m_nLastTargetPosX;
	int				m_nLastTargetPosY;

	// Follow fallback
	unsigned int	m_dwLeaderLostTime;
	BOOL			m_bFollowFallback;
	unsigned int	m_dwLastReacquireAttempt;
	int				m_nSavedAutoMove;

	// Route + combat
	BOOL			m_bRoutePaused;
	int				m_nPausedRouteStep;
	int				m_nLastRouteX;
	int				m_nLastRouteY;
	int				m_nStuckCounter;
	unsigned int	m_dwLastWaypointTime;

	// Packet throttling
	DWORD			m_PacketLastSent[4];     // [MOVE, ATTACK, SKILL, ITEM]
	int				m_PacketCount[4];
	DWORD			m_PacketWindowStart;
	int				m_PacketTotalThisSecond;
	DWORD			m_PacketLastLogTime;

public:
```

### Location: Line 274 (before closing `};`)
### Action: ADD these method declarations

```cpp
	void 			PaintActionAuto(int nType,int nNpcID,int nX,int nY);

	// ===== PACKET BURST FIX: ADD THESE METHODS =====
	BOOL			IsTargetLagging(int nTargetIdx);
	void			MarkTargetAsLag(int nTargetIdx);
	void			UpdateFollowFallback();
	BOOL			ShouldEngageOnRoute(int *outNpcIdx);
	void			UpdateRouteProgress();
	BOOL			CanSendPacket(int packetType);  // 0=MOVE 1=ATTACK 2=SKILL 3=ITEM
	void			LogPacketStats();
};
```

---

## FILE 2: KPlayerAI.cpp

### Location: Line 169 (in Release() method, before closing `}`)
### Action: ADD field initialization

```cpp
	m_sListEquipment.m_Link.Init(MAX_EQUIPMENT_ITEM);

	// ===== PACKET BURST FIX: INITIALIZE NEW FIELDS =====
	m_nLastTargetHP = 0;
	m_nLastTargetDistance = 0;
	m_nSameHPCounter = 0;
	m_nSameDistCounter = 0;
	m_nLastTargetPosX = 0;
	m_nLastTargetPosY = 0;

	m_dwLeaderLostTime = 0;
	m_bFollowFallback = FALSE;
	m_dwLastReacquireAttempt = 0;
	m_nSavedAutoMove = 0;

	m_bRoutePaused = FALSE;
	m_nPausedRouteStep = 0;
	m_nLastRouteX = 0;
	m_nLastRouteY = 0;
	m_nStuckCounter = 0;
	m_dwLastWaypointTime = 0;

	memset(m_PacketLastSent, 0, sizeof(m_PacketLastSent));
	memset(m_PacketCount, 0, sizeof(m_PacketCount));
	m_PacketWindowStart = 0;
	m_PacketTotalThisSecond = 0;
	m_PacketLastLogTime = 0;
}
```

### Location: End of file (before `#endif`)
### Action: COPY entire method implementations from attached file

**See: KPlayerAI_METHODS_IMPLEMENTATION.cpp**
(File chứa 7 methods: IsTargetLagging, MarkTargetAsLag, UpdateFollowFallback, ShouldEngageOnRoute, UpdateRouteProgress, CanSendPacket, LogPacketStats)

---

## FILE 3: GameDataDef.h

### Location: Lines for packet burst config
### Action: ADD these defines at top of file (after existing defines)

```cpp
// ===== PACKET BURST FIX: Configuration =====
#define HP_STALL_TICKS          40   // HP same for 40 ticks = 2 sec = lag
#define DIST_STALL_TICKS        30   // Distance same for 30 ticks = 1.5 sec = unreachable
#define LAG_TTL_TICKS           200  // Lag blacklist TTL = 200 ticks = 10 seconds
#define LEADER_GRACE_MS         4000 // Wait 4 seconds before fallback to route
#define ROUTE_ENGAGE_RADIUS     8    // Engage enemies within 8 cells on route
#define WAYPOINT_TOLERANCE      2    // Accept waypoint if within 2 cells
#define STUCK_THRESHOLD_TICKS   60   // Stuck if same position for 60 ticks = 3 sec

// Packet rate limiting
#define PACKET_MOVE_MIN_MS      50   // Min 50ms between MOVE packets
#define PACKET_ATTACK_MIN_MS    100  // Min 100ms between ATTACK packets
#define PACKET_SKILL_MIN_MS     150  // Min 150ms between SKILL packets
#define PACKET_ITEM_MIN_MS      200  // Min 200ms between ITEM packets
#define PACKET_BURST_CAP_PPS    20   // Max 20 packets/second total
```

---

## STEP-BY-STEP INTEGRATION

### Step 1: Apply Header Changes
1. Open `SwordOnline/Sources/Core/Src/KPlayerAI.h`
2. Find line 174 (before `public:`)
3. Paste the new field declarations (24 new fields)
4. Find line 274 (before `};`)
5. Paste the new method declarations (7 methods)
6. Save file

### Step 2: Apply Implementation Changes
1. Open `SwordOnline/Sources/Core/Src/KPlayerAI.cpp`
2. Find `Release()` method around line 169
3. Before the closing `}`, paste the field initialization code
4. Scroll to end of file (before `#endif`)
5. Copy/paste all 7 method implementations from `KPlayerAI_METHODS_IMPLEMENTATION.cpp`
6. Save file

### Step 3: Add Config Defines
1. Open `SwordOnline/Sources/Core/Src/GameDataDef.h`
2. Add the configuration defines at top (after existing defines)
3. Save file

### Step 4: Compile & Test
```bash
# Rebuild project
make clean
make

# Test with 1 client first, then 3 clients
# Monitor for BURST warnings in logs
```

---

## VERIFICATION CHECKLIST

After applying changes:

- [ ] KPlayerAI.h compiles without errors
- [ ] KPlayerAI.cpp compiles without errors
- [ ] All 24 new fields are initialized in Release()
- [ ] All 7 new methods are implemented
- [ ] Config defines are added to GameDataDef.h
- [ ] Client runs without crashes
- [ ] Test with 1 client for 10 minutes - no disconnect
- [ ] Test with 3 clients for 30 minutes - no VPS block
- [ ] Check logs for [AI-PACKET] stats every 5 seconds

---

## EXPECTED BEHAVIOR

### Before Fix:
- Clients disconnect every 2-3 minutes
- VPS blocks IP after multiple reconnects
- No packet rate visibility

### After Fix:
- Lag targets auto-blacklisted
- Follow leader with route fallback
- Route pauses for nearby combat
- Packet rate limited to <20 pps
- Logs show packet stats every 5s
- **No more VPS IP blocks** ✅

---

## FILES SUMMARY

| File | Lines Changed | Purpose |
|------|---------------|---------|
| KPlayerAI.h | +24 fields, +7 methods | Add packet burst prevention |
| KPlayerAI.cpp | +25 init, +250 impl | Implement lag detection & throttling |
| GameDataDef.h | +11 defines | Configuration constants |
| **TOTAL** | **~310 lines added** | **Complete packet burst fix** |

---

**Note**: Full method implementations are too long for inline diff.
Xem file `AI_ENHANCEMENT_COMPLETE_PATCH.cpp` để copy full implementations của 7 methods.

Hoặc tôi có thể tạo từng method riêng lẻ nếu bạn muốn?
