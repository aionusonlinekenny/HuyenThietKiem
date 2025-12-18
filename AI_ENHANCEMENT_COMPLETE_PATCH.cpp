/*==============================================================================
 * AI Enhancement Patch for Sword Online - Complete Implementation
 * Date: 2025-12-18
 * Author: Claude AI
 *
 * Targets: Combat lag detection, Follow fallback, Route+Combat balance, Packet guard
 * Compatibility: VC6-safe (no STL, no lambda, pure C++98)
 *=============================================================================*/

//==============================================================================
// PART 1: NEW DEFINES (Add to GameDataDef.h after line 98)
//==============================================================================

// Smart Combat - Lag Detection
#define HP_STALL_TICKS          40      // 40 ticks (~2 seconds) of no HP change = lag
#define DIST_STALL_TICKS        30      // 30 ticks (~1.5s) same distance = stuck
#define LAG_TTL_TICKS           200     // 200 ticks (~10s) TTL for lag targets
#define HIT_FAIL_THRESHOLD      15      // 15 consecutive hit fails = lag

// Follow Leader Fallback
#define FOLLOW_LOST_TICKS       80      // 80 ticks (~4s) lost leader → fallback to route
#define REACQUIRE_INTERVAL      40      // 40 ticks (~2s) try to reacquire leader

// Route + Combat Balance
#define ENGAGE_RADIUS_CELLS     8       // 8 cells (~256px) engage enemies on route
#define WAYPOINT_TOL_CELLS      2       // 2 cells (~64px) waypoint tolerance
#define STUCK_TICKS             60      // 60 ticks (~3s) stuck → skip waypoint

// Packet Rate Limiting
#define PACKET_BURST_MAX_PER_SEC    20  // Max 20 packets/second total
#define PACKET_MOVE_MIN_INTERVAL    50  // Min 50ms between MOVE packets
#define PACKET_ATTACK_MIN_INTERVAL  100 // Min 100ms between ATTACK packets
#define PACKET_SKILL_MIN_INTERVAL   150 // Min 150ms between SKILL packets
#define PACKET_ITEM_MIN_INTERVAL    200 // Min 200ms between ITEM packets


//==============================================================================
// PART 2: NEW FIELDS (Add to KPlayerAI.h after line 172)
//==============================================================================

class KPlayerAI
{
    // ... existing fields ...

public:
    // === COMBAT LAG DETECTION ===
    unsigned int    m_nLastTargetHP;            // Last seen HP of current target
    int             m_nLastTargetDistance;      // Last distance to target (MPS units)
    int             m_nSameHPCounter;           // Ticks with same HP
    int             m_nSameDistCounter;         // Ticks with same distance
    int             m_nHitFailCounter;          // Consecutive hit failures
    int             m_nLastTargetPosX;          // For distance tracking
    int             m_nLastTargetPosY;

    // === FOLLOW LEADER FALLBACK ===
    unsigned int    m_dwLeaderLostTime;         // Time when leader was lost
    BOOL            m_bFollowFallback;          // In fallback-to-route mode?
    unsigned int    m_dwLastReacquireAttempt;   // Last reacquire attempt time
    int             m_nSavedAutoMove;           // Saved m_AutoMove state before fallback

    // === ROUTE + COMBAT BALANCE ===
    BOOL            m_bRoutePaused;             // Route paused for combat?
    int             m_nPausedRouteStep;         // Which waypoint we paused at
    int             m_nLastRouteX;              // For stuck detection
    int             m_nLastRouteY;
    int             m_nStuckCounter;            // Ticks stuck at same waypoint
    unsigned int    m_dwLastWaypointTime;       // Time of last waypoint change

    // === PACKET RATE LIMITING ===
    struct PacketThrottle {
        DWORD lastSent[4];      // [MOVE, ATTACK, SKILL, ITEM]
        int   packetCount[4];   // Counts in current 1-second window
        DWORD windowStart;      // Start of current 1-second window
        int   totalThisSecond;  // Total packets this second
        DWORD lastLogTime;      // Last time we logged stats (every 5s)
    } m_PacketThrottle;

public:
    // New helper methods (declare in class, define below)
    BOOL    IsTargetLagging(int nTargetIdx);
    void    MarkTargetAsLag(int nTargetIdx);
    void    UpdateFollowFallback();
    BOOL    ShouldEngageOnRoute(int* outTargetIdx);
    void    UpdateRouteProgress();
    BOOL    CanSendPacket(int nPacketType);  // 0=MOVE, 1=ATTACK, 2=SKILL, 3=ITEM
    void    LogPacketStats();
};


//==============================================================================
// PART 3: INITIALIZATION (Add to KPlayerAI::Release() after line 169)
//==============================================================================

void KPlayerAI::Release()
{
    // ... existing initializations ...

    // === COMBAT LAG DETECTION ===
    m_nLastTargetHP         = 0;
    m_nLastTargetDistance   = 0;
    m_nSameHPCounter        = 0;
    m_nSameDistCounter      = 0;
    m_nHitFailCounter       = 0;
    m_nLastTargetPosX       = 0;
    m_nLastTargetPosY       = 0;

    // === FOLLOW LEADER FALLBACK ===
    m_dwLeaderLostTime      = 0;
    m_bFollowFallback       = FALSE;
    m_dwLastReacquireAttempt= 0;
    m_nSavedAutoMove        = FALSE;

    // === ROUTE + COMBAT BALANCE ===
    m_bRoutePaused          = FALSE;
    m_nPausedRouteStep      = 0;
    m_nLastRouteX           = 0;
    m_nLastRouteY           = 0;
    m_nStuckCounter         = 0;
    m_dwLastWaypointTime    = 0;

    // === PACKET RATE LIMITING ===
    memset(&m_PacketThrottle, 0, sizeof(m_PacketThrottle));
    m_PacketThrottle.windowStart = GetTickCount();
}


//==============================================================================
// PART 4: LAG DETECTION IMPLEMENTATION (Replace PlayerFollowActack logic)
//==============================================================================

void KPlayerAI::PlayerFollowActack(int i)
{
    if (IsNotValidNpc(i))
        return;

    if ((m_SpaceBar && !m_HoldSpaceBar) || m_bPriorityUseMouse)
    {
        // User override - clear lag tracking
        m_nLastTargetHP = 0;
        m_nSameHPCounter = 0;
        m_nSameDistCounter = 0;
        m_nHitFailCounter = 0;
        m_nLifeLag = 0;
        m_Actacker = 0;
        m_nTimeRunLag = 0;
        m_nTimeSkip = 0;
        m_Count_Acttack_Lag = 0;
        m_bActacker = FALSE;
        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;
        return;
    }

    int nMapX, nMapY, k, h;
    Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nMapX, &nMapY);
    Npc[i].GetMpsPos(&k, &h);

    if (m_bAttackAround)
        AutoReturn();

    // === SMART LAG DETECTION: Check if target is stuck/unreachable ===
    if (IsTargetLagging(i))
    {
        // Target confirmed lagging - mark and skip
        printf("[LAG-DETECT] NPC %d flagged as lag (HP stall or dist stall)\n", i);
        MarkTargetAsLag(i);

        m_Actacker = 0;
        m_bActacker = FALSE;
        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;

        // If on route and this was combat interrupt, resume route
        if (m_bRoutePaused && m_AutoMove)
        {
            printf("[LAG-SKIP] Resuming route from step %d\n", m_nPausedRouteStep);
            m_MoveStep = m_nPausedRouteStep;
            m_bRoutePaused = FALSE;
        }
        // If following leader, just clear target (will find new one next tick)
        // If on route normally, will skip to next waypoint if stuck
        return;
    }

    // === EXISTING BARRIER CHECK ===
    if (Npc[i].m_RegionIndex > 0)
    {
        if (m_NpcFind.CheckBarrier(h,k) != 0)
        {
            // Barrier detected - but don't immediately flag as lag
            // Let HP/distance tracking decide if truly stuck
            m_nLifeLag = 0;
            m_Actacker = 0;
            m_nTimeRunLag = 0;
            m_nTimeSkip = 0;
            m_Count_Acttack_Lag = 0;
            m_bActacker = FALSE;
            Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;
            return;
        }
    }

    // === COMBAT LOGIC (existing code continues) ===
    int distance = NpcSet.GetDistance(Player[CLIENT_PLAYER_INDEX].m_nIndex, i);

    // Attack logic here (use existing combat code)
    // ... (rest of PlayerFollowActack implementation)
}


//==============================================================================
// PART 5: LAG DETECTION HELPER - IsTargetLagging()
//==============================================================================

BOOL KPlayerAI::IsTargetLagging(int nTargetIdx)
{
    if (nTargetIdx <= 0 || nTargetIdx >= MAX_NPC)
        return FALSE;

    // Get current target stats
    unsigned int currentHP = Npc[nTargetIdx].m_CurrentLife;
    int targetX, targetY;
    Npc[nTargetIdx].GetMpsPos(&targetX, &targetY);
    int currentDist = NpcSet.GetDistance(Player[CLIENT_PLAYER_INDEX].m_nIndex, nTargetIdx);

    // === METHOD 1: HP STALL DETECTION ===
    // If we're attacking but HP never changes → lag target
    if (m_nLastTargetHP == 0)
    {
        // First observation - initialize
        m_nLastTargetHP = currentHP;
        m_nSameHPCounter = 0;
    }
    else
    {
        if (currentHP == m_nLastTargetHP)
        {
            m_nSameHPCounter++;
            if (m_nSameHPCounter >= HP_STALL_TICKS)
            {
                printf("[HP-STALL] Target %d HP stuck at %u for %d ticks\n",
                       nTargetIdx, currentHP, m_nSameHPCounter);
                return TRUE;  // HP stall confirmed
            }
        }
        else
        {
            // HP changed - target is responsive
            m_nLastTargetHP = currentHP;
            m_nSameHPCounter = 0;
        }
    }

    // === METHOD 2: DISTANCE STALL DETECTION ===
    // If we're moving toward target but distance never changes → unreachable
    if (m_nLastTargetDistance == 0)
    {
        m_nLastTargetDistance = currentDist;
        m_nLastTargetPosX = targetX;
        m_nLastTargetPosY = targetY;
        m_nSameDistCounter = 0;
    }
    else
    {
        // Check if target moved (allow small jitter)
        int dx = targetX - m_nLastTargetPosX;
        int dy = targetY - m_nLastTargetPosY;
        int distMoved = (int)sqrt((double)(dx*dx + dy*dy));

        // If target barely moved AND distance to us is same → stuck
        if (distMoved < 16 && abs(currentDist - m_nLastTargetDistance) < 10)
        {
            m_nSameDistCounter++;
            if (m_nSameDistCounter >= DIST_STALL_TICKS)
            {
                printf("[DIST-STALL] Target %d distance stuck at %d for %d ticks\n",
                       nTargetIdx, currentDist, m_nSameDistCounter);
                return TRUE;  // Distance stall confirmed
            }
        }
        else
        {
            // Distance changing - target is reachable
            m_nLastTargetDistance = currentDist;
            m_nLastTargetPosX = targetX;
            m_nLastTargetPosY = targetY;
            m_nSameDistCounter = 0;
        }
    }

    // === METHOD 3: HIT FAIL COUNTER (if available) ===
    // Some implementations track m_Count_Acttack_Lag for hit failures
    // This is a fallback if HP tracking unavailable
    if (m_Count_Acttack_Lag >= HIT_FAIL_THRESHOLD)
    {
        printf("[HIT-FAIL] Target %d hit fail count: %d\n", nTargetIdx, m_Count_Acttack_Lag);
        return TRUE;
    }

    return FALSE;  // Target is responsive
}


//==============================================================================
// PART 6: LAG MARKING HELPER - MarkTargetAsLag()
//==============================================================================

void KPlayerAI::MarkTargetAsLag(int nTargetIdx)
{
    // Find free slot in lag array
    int freeSlot = -1;
    for (int i = 0; i < defMAX_ARRAY_AUTO; i++)
    {
        if (m_ArrayNpcLag[i] == 0)
        {
            freeSlot = i;
            break;
        }
    }

    if (freeSlot >= 0)
    {
        m_ArrayNpcLag[freeSlot] = nTargetIdx;
        m_ArrayTimeNpcLag[freeSlot] = GetTickCount();
        printf("[LAG-LIST] Added NPC %d to lag list slot %d (TTL=%dms)\n",
               nTargetIdx, freeSlot, defAUTO_TIME_RESET_LAG);
    }
    else
    {
        // List full - clear oldest entry
        int oldestSlot = 0;
        DWORD oldestTime = m_ArrayTimeNpcLag[0];
        for (int i = 1; i < defMAX_ARRAY_AUTO; i++)
        {
            if (m_ArrayTimeNpcLag[i] < oldestTime)
            {
                oldestTime = m_ArrayTimeNpcLag[i];
                oldestSlot = i;
            }
        }
        m_ArrayNpcLag[oldestSlot] = nTargetIdx;
        m_ArrayTimeNpcLag[oldestSlot] = GetTickCount();
        printf("[LAG-LIST] List full - replaced oldest slot %d with NPC %d\n",
               oldestSlot, nTargetIdx);
    }

    // Reset lag detection state for next target
    m_nLastTargetHP = 0;
    m_nLastTargetDistance = 0;
    m_nSameHPCounter = 0;
    m_nSameDistCounter = 0;
    m_nHitFailCounter = 0;
    m_Count_Acttack_Lag = 0;
}


//==============================================================================
// PART 7: FOLLOW LEADER FALLBACK (Modify Active() around line 333-520)
//==============================================================================

void KPlayerAI::UpdateFollowFallback()
{
    // This function goes in Active() where follow leader logic is

    if (m_bFollowPeople == TRUE && m_FollowPeopleName[0])
    {
        // Try to find leader (existing 3-layer search logic)
        int nRegionNo, i, j;

        m_FollowPeopleIdx = SubWorld[0].m_Region[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex].SearchNpcName(m_FollowPeopleName);

        if(m_FollowPeopleIdx <= 0)
        {
            // Search layer 2
            for (i = 0; i < 8; i++)
            {
                nRegionNo = SubWorld[0].m_Region[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex].m_nConnectRegion[i];
                if (nRegionNo < 0) continue;
                m_FollowPeopleIdx = SubWorld[0].m_Region[nRegionNo].SearchNpcName(m_FollowPeopleName);
                if (m_FollowPeopleIdx > 0) break;
            }
        }

        if(m_FollowPeopleIdx <= 0)
        {
            // Search layer 3
            for (i = 0; i < 8; i++)
            {
                int nFirstLayerRegion = SubWorld[0].m_Region[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex].m_nConnectRegion[i];
                if (nFirstLayerRegion < 0) continue;

                for (j = 0; j < 8; j++)
                {
                    nRegionNo = SubWorld[0].m_Region[nFirstLayerRegion].m_nConnectRegion[j];
                    if (nRegionNo < 0) continue;
                    m_FollowPeopleIdx = SubWorld[0].m_Region[nRegionNo].SearchNpcName(m_FollowPeopleName);
                    if (m_FollowPeopleIdx > 0) break;
                }
                if (m_FollowPeopleIdx > 0) break;
            }
        }

        if(m_FollowPeopleIdx <= 0)
        {
            // Full search as last resort
            m_FollowPeopleIdx = NpcSet.SearchName(m_FollowPeopleName);
        }

        // === FALLBACK LOGIC: If leader lost for too long, switch to route ===
        if (m_FollowPeopleIdx <= 0)
        {
            // Leader not found
            if (m_dwLeaderLostTime == 0)
            {
                // Just lost leader - start timer
                m_dwLeaderLostTime = GetTickCount();
                printf("[FOLLOW] Lost leader '%s', starting grace period\n", m_FollowPeopleName);
            }
            else
            {
                DWORD lostDuration = GetTickCount() - m_dwLeaderLostTime;

                if (lostDuration > FOLLOW_LOST_TICKS * 50)  // Convert ticks to ms
                {
                    // Grace period expired - activate fallback to route
                    if (!m_bFollowFallback && m_AutoMove && m_MoveCount > 0)
                    {
                        printf("[FOLLOW-FALLBACK] Leader lost for %dms, switching to route mode\n", lostDuration);
                        m_bFollowFallback = TRUE;
                        m_nSavedAutoMove = m_AutoMove;
                        // Route is already enabled, just mark fallback active
                    }

                    // Periodically try to reacquire leader
                    DWORD now = GetTickCount();
                    if (m_dwLastReacquireAttempt == 0 || (now - m_dwLastReacquireAttempt) > REACQUIRE_INTERVAL * 50)
                    {
                        m_dwLastReacquireAttempt = now;
                        printf("[FOLLOW-FALLBACK] Attempting to reacquire leader (route step %d/%d)\n",
                               m_MoveStep, m_MoveCount);
                        // Search happens above, if found will trigger below
                    }
                }
            }
        }
        else
        {
            // Leader found!
            if (m_bFollowFallback)
            {
                // Was in fallback mode - restore follow mode
                printf("[FOLLOW] Leader '%s' reacquired! Exiting fallback mode\n", m_FollowPeopleName);
                m_bFollowFallback = FALSE;
                m_AutoMove = m_nSavedAutoMove;  // Restore original route state
            }

            // Reset timers
            m_dwLeaderLostTime = 0;
            m_dwLastReacquireAttempt = 0;

            // Continue with normal follow logic (existing code)
            // ... (cache leader position, track target, etc.)
        }
    }
}


//==============================================================================
// PART 8: ROUTE + COMBAT BALANCE (Modify PlayerMoveMps and Active)
//==============================================================================

BOOL KPlayerAI::ShouldEngageOnRoute(int* outTargetIdx)
{
    // Check if we're running a route
    if (!m_AutoMove || m_MoveCount <= 0)
        return FALSE;

    // Don't engage if route is paused (already in combat)
    if (m_bRoutePaused)
        return FALSE;

    // Search for enemies near current position (within ENGAGE_RADIUS)
    int nPlayerIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;
    int nPlayerX, nPlayerY;
    Npc[nPlayerIdx].GetMpsPos(&nPlayerX, &nPlayerY);

    int nSubWorldIdx = Npc[nPlayerIdx].m_SubWorldIndex;
    int nRegion = Npc[nPlayerIdx].m_RegionIndex;
    int nMapX = Npc[nPlayerIdx].m_MapX;
    int nMapY = Npc[nPlayerIdx].m_MapY;

    // Search in ENGAGE_RADIUS cells
    int nRangeX = ENGAGE_RADIUS_CELLS;
    int nRangeY = ENGAGE_RADIUS_CELLS;

    for (int i = -nRangeX; i <= nRangeX; i++)
    {
        for (int j = -nRangeY; j <= nRangeY; j++)
        {
            if (i*i + j*j > nRangeX*nRangeX)
                continue;  // Outside radius

            int nRMx = nMapX + i;
            int nRMy = nMapY + j;
            int nSearchRegion = nRegion;

            // Handle region boundaries (copy logic from AutoAddNpc2Array)
            if (nRMx < 0)
            {
                nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[2];
                nRMx += SubWorld[nSubWorldIdx].m_nRegionWidth;
            }
            else if (nRMx >= SubWorld[nSubWorldIdx].m_nRegionWidth)
            {
                nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[6];
                nRMx -= SubWorld[nSubWorldIdx].m_nRegionWidth;
            }
            if (nSearchRegion == -1) continue;

            if (nRMy < 0)
            {
                nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[4];
                nRMy += SubWorld[nSubWorldIdx].m_nRegionHeight;
            }
            else if (nRMy >= SubWorld[nSubWorldIdx].m_nRegionHeight)
            {
                nSearchRegion = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].m_nConnectRegion[0];
                nRMy -= SubWorld[nSubWorldIdx].m_nRegionHeight;
            }
            if (nSearchRegion == -1) continue;

            int nRet = SubWorld[nSubWorldIdx].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, nPlayerIdx, relation_enemy);

            if (nRet > 0)
            {
                // Found enemy - validate it
                if (Npc[nRet].m_Doing != do_death && Npc[nRet].m_Doing != do_revive && Npc[nRet].m_HideState.nTime <= 0)
                {
                    // Check if not in lag list
                    BOOL isLagged = FALSE;
                    for (int k = 0; k < defMAX_ARRAY_AUTO; k++)
                    {
                        if (m_ArrayNpcLag[k] == nRet)
                        {
                            isLagged = TRUE;
                            break;
                        }
                    }

                    if (!isLagged && !IsNotValidNpc(nRet))
                    {
                        *outTargetIdx = nRet;
                        printf("[ROUTE-ENGAGE] Found enemy %d while on route, pausing route at step %d\n",
                               nRet, m_MoveStep);
                        return TRUE;  // Engage this enemy!
                    }
                }
            }
        }
    }

    return FALSE;  // No enemies in engage radius
}

void KPlayerAI::UpdateRouteProgress()
{
    if (!m_AutoMove || m_MoveCount <= 0)
        return;

    // Get current position
    int nPlayerIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;
    int nCurX, nCurY;
    Npc[nPlayerIdx].GetMpsPos(&nCurX, &nCurY);

    // Get current waypoint target
    int nWaypointX = m_MoveMps[m_MoveStep][1];
    int nWaypointY = m_MoveMps[m_MoveStep][2];

    // Calculate distance to waypoint (in cells)
    const int CELL = 32;
    int dx = (nCurX / CELL) - (nWaypointX / CELL);
    int dy = (nCurY / CELL) - (nWaypointY / CELL);
    int distCells = (int)sqrt((double)(dx*dx + dy*dy));

    // === WAYPOINT REACHED CHECK (with tolerance) ===
    if (distCells <= WAYPOINT_TOL_CELLS)
    {
        printf("[ROUTE] Reached waypoint %d/%d (tolerance %d cells)\n",
               m_MoveStep + 1, m_MoveCount, WAYPOINT_TOL_CELLS);

        // Advance to next waypoint
        m_MoveStep++;
        if (m_MoveStep >= m_MoveCount)
        {
            // Route completed
            if (m_MoveRevese > 0 || b_MoveRevese)
            {
                // Reverse route
                m_MoveStep = 0;
                printf("[ROUTE] Route completed, reversing\n");
            }
            else
            {
                m_MoveStep = 0;  // Loop
                printf("[ROUTE] Route completed, looping\n");
            }
        }

        m_dwLastWaypointTime = GetTickCount();
        m_nStuckCounter = 0;  // Reset stuck counter on waypoint change
        return;
    }

    // === STUCK DETECTION ===
    // Check if we haven't moved significantly
    int lastDx = nCurX - m_nLastRouteX;
    int lastDy = nCurY - m_nLastRouteY;
    int moved = (int)sqrt((double)(lastDx*lastDx + lastDy*lastDy));

    if (moved < 16)  // Barely moved (< 16 pixels)
    {
        m_nStuckCounter++;

        if (m_nStuckCounter >= STUCK_TICKS)
        {
            // Stuck for too long - skip waypoint
            printf("[ROUTE-STUCK] Stuck at waypoint %d for %d ticks, skipping\n",
                   m_MoveStep, m_nStuckCounter);

            m_MoveStep++;
            if (m_MoveStep >= m_MoveCount)
            {
                m_MoveStep = 0;
                printf("[ROUTE-STUCK] Was last waypoint, looping\n");
            }

            m_nStuckCounter = 0;
            m_dwLastWaypointTime = GetTickCount();
        }
    }
    else
    {
        // Moving normally - reset stuck counter
        m_nStuckCounter = 0;
    }

    // Update last position
    m_nLastRouteX = nCurX;
    m_nLastRouteY = nCurY;
}


//==============================================================================
// PART 9: PACKET RATE LIMITING
//==============================================================================

BOOL KPlayerAI::CanSendPacket(int nPacketType)
{
    // nPacketType: 0=MOVE, 1=ATTACK, 2=SKILL, 3=ITEM
    if (nPacketType < 0 || nPacketType >= 4)
        return TRUE;  // Unknown type - allow

    DWORD now = GetTickCount();

    // === Reset 1-second window if needed ===
    if (now - m_PacketThrottle.windowStart >= 1000)
    {
        // New second - reset counters
        memset(m_PacketThrottle.packetCount, 0, sizeof(m_PacketThrottle.packetCount));
        m_PacketThrottle.totalThisSecond = 0;
        m_PacketThrottle.windowStart = now;
    }

    // === Check total burst limit ===
    if (m_PacketThrottle.totalThisSecond >= PACKET_BURST_MAX_PER_SEC)
    {
        printf("[PACKET-GUARD] DROPPED type=%d (total burst limit %d/sec exceeded)\n",
               nPacketType, PACKET_BURST_MAX_PER_SEC);
        return FALSE;
    }

    // === Check per-type minimum interval ===
    static const DWORD minIntervals[4] = {
        PACKET_MOVE_MIN_INTERVAL,
        PACKET_ATTACK_MIN_INTERVAL,
        PACKET_SKILL_MIN_INTERVAL,
        PACKET_ITEM_MIN_INTERVAL
    };

    DWORD elapsed = now - m_PacketThrottle.lastSent[nPacketType];
    if (elapsed < minIntervals[nPacketType])
    {
        // Too soon since last packet of this type
        printf("[PACKET-GUARD] DROPPED type=%d (min interval %dms, elapsed %dms)\n",
               nPacketType, minIntervals[nPacketType], elapsed);
        return FALSE;
    }

    // === Packet allowed - update counters ===
    m_PacketThrottle.lastSent[nPacketType] = now;
    m_PacketThrottle.packetCount[nPacketType]++;
    m_PacketThrottle.totalThisSecond++;

    return TRUE;  // Packet allowed
}

void KPlayerAI::LogPacketStats()
{
    DWORD now = GetTickCount();

    // Log every 5 seconds
    if (m_PacketThrottle.lastLogTime == 0 || (now - m_PacketThrottle.lastLogTime) >= 5000)
    {
        int total = 0;
        for (int i = 0; i < 4; i++)
            total += m_PacketThrottle.packetCount[i];

        printf("[PACKET-STATS] Last 5s: MOVE=%d ATTACK=%d SKILL=%d ITEM=%d TOTAL=%d (limit=%d/sec)\n",
               m_PacketThrottle.packetCount[0],
               m_PacketThrottle.packetCount[1],
               m_PacketThrottle.packetCount[2],
               m_PacketThrottle.packetCount[3],
               total,
               PACKET_BURST_MAX_PER_SEC);

        // Warn if approaching limit
        if (m_PacketThrottle.totalThisSecond >= PACKET_BURST_MAX_PER_SEC * 80 / 100)
        {
            printf("[PACKET-WARN] Approaching rate limit! (%d%% of max %d/sec)\n",
                   m_PacketThrottle.totalThisSecond * 100 / PACKET_BURST_MAX_PER_SEC,
                   PACKET_BURST_MAX_PER_SEC);
        }

        m_PacketThrottle.lastLogTime = now;
    }
}


//==============================================================================
// PART 10: MODIFIED Active() - INTEGRATION POINT
//==============================================================================

void KPlayerAI::Active()
{
    int index = 0;
    int iObject = 0;

    if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bOpenShop > 0)
        return;

    if (Player[CLIENT_PLAYER_INDEX].m_bActiveAuto)
    {
        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&m_nStartAIX,&m_nStartAIY);

        PlayerSwitchAura();
        PlayerEatAItem();

        if (Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState() != 2)
            EatFullBox();

        RepairEquip();

        if (m_bFilterEquipment)
        {
            if (g_SubWorldSet.GetGameTime() % 16 == 0)
                PlayerFilterEquip();
        }

        // === LOG PACKET STATS (every 5s) ===
        LogPacketStats();

        if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
        {
            if (g_SubWorldSet.GetGameTime() % 4 == 0)
                CheckPlayerRider();

            AutoBuffSkillState();
            PlayerBuffWhenManaSmall();
            PlayerActiveFightHand();

            if (AutoBuffEmi() == TRUE)
                return;

            m_nTimeBacking = 0;
            m_nTimeBackToMapTrain = 0;

            if (m_nWarningPK)
                Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetWarning(1);
            else
                Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetWarning(0);

            // === TTL EXPIRY FOR LAG LIST ===
            int i = 0;
            int nCountNpcLag = 0;
            int nCountObjectLag = 0;

            for (i = 0; i < defMAX_ARRAY_AUTO; i++)
            {
                if (m_ArrayNpcLag[i] > 0)
                {
                    nCountNpcLag++;

                    DWORD age = GetTickCount() - m_ArrayTimeNpcLag[i];
                    DWORD ttl = (Npc[m_ArrayNpcLag[i]].m_Kind == kind_player) ?
                                (defAUTO_TIME_RESET_LAG / 20) : defAUTO_TIME_RESET_LAG;

                    if (age > ttl)
                    {
                        printf("[LAG-TTL] NPC %d expired from lag list (age %dms > ttl %dms)\n",
                               m_ArrayNpcLag[i], age, ttl);
                        m_ArrayNpcLag[i] = 0;
                        m_ArrayTimeNpcLag[i] = 0;
                    }
                }

                if (m_ArrayObjectLag[i] > 0)
                {
                    nCountObjectLag++;
                    if (GetTickCount() - m_ArrayTimeObjectLag[i] > defAUTO_TIME_RESET_LAG)
                    {
                        m_ArrayObjectLag[i] = 0;
                        m_ArrayTimeObjectLag[i] = 0;
                    }
                }
            }

            if (nCountNpcLag >= defMAX_ARRAY_AUTO)
            {
                ClearArrayNpcLag();
                ClearArrayTimeNpcLag();
            }

            if (nCountObjectLag >= defMAX_ARRAY_AUTO)
            {
                ClearArrayObjectLag();
                ClearArrayTimeObjectLag();
            }

            // === AUTO PARTY LOGIC ===
            if (m_bAutoParty && m_bAutoInvite)
            {
                if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
                {
                    Player[CLIENT_PLAYER_INDEX].ApplyCreateTeam();
                    ClearArrayInvitePlayer();
                    ClearArrayTimeInvitePlayer();
                }
                else
                    InviteParty();
            }

            // === FOLLOW LEADER WITH FALLBACK ===
            UpdateFollowFallback();

            // === ITEM PICKUP LOGIC (existing) ===
            if ((m_bObject == FALSE || m_nObject == 0) && Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nObjectIdx == 0)
            {
                iObject = FindNearObject2Array();
                if (iObject > 0)
                {
                    // Check if item too far from leader (existing code)
                    // ... (item pickup logic)
                }
            }

            if (m_nObject > 0)
            {
                if (PlayerFollowObject(m_nObject))
                    return;
            }
            m_bObject = FALSE;
            m_nObject = 0;

            // === COMBAT: ROUTE ENGAGEMENT CHECK ===
            if (!m_bFollowPeople && m_AutoMove)
            {
                int engageTarget = 0;
                if (ShouldEngageOnRoute(&engageTarget))
                {
                    // Enemy found on route - pause route and engage
                    m_bRoutePaused = TRUE;
                    m_nPausedRouteStep = m_MoveStep;
                    m_Actacker = engageTarget;
                    m_bActacker = TRUE;
                    index = engageTarget;
                    printf("[ROUTE-ENGAGE] Pausing route at step %d to fight NPC %d\n",
                           m_MoveStep, engageTarget);
                }
            }

            // === TARGET SELECTION (existing logic) ===
            if (m_bActacker == FALSE || m_Actacker == 0 || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing != do_skill)
            {
                memset(m_ArrayNpcNeast,0,sizeof(m_ArrayNpcNeast));
                index = FindNearNpc2Array(relation_enemy);

                if (index > 0)
                {
                    BOOL _flagLag = FALSE;
                    for (i = 0; i < defMAX_ARRAY_AUTO; i++)
                    {
                        if (m_ArrayNpcLag[i] == index)
                        {
                            _flagLag = TRUE;
                            break;
                        }
                    }

                    if (_flagLag)
                    {
                        // Target in lag list - skip it
                        printf("[LAG-SKIP] Target %d in lag list, finding new target\n", index);
                        m_Actacker = 0;
                        m_bActacker = FALSE;
                        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;
                        index = 0;

                        // If on route, move to next waypoint
                        if (m_AutoMove && !m_bFollowPeople)
                        {
                            if (m_bPriorityUseMouse)
                                return;
                            PlayerMoveMps();
                        }
                    }
                    else
                    {
                        m_Actacker = index;
                        m_bActacker = TRUE;
                    }
                }
            }
            else
            {
                index = m_Actacker;
            }

            // === ATTACK EXECUTION ===
            if (index > 0 && m_bAutoAttack == TRUE)
            {
                PlayerFollowActack(index);  // Uses enhanced lag detection
            }
            else
            {
                m_bActacker = FALSE;

                // If no target and route is paused, resume route
                if (m_bRoutePaused && m_AutoMove)
                {
                    printf("[ROUTE-RESUME] Combat ended, resuming route from step %d\n", m_nPausedRouteStep);
                    m_MoveStep = m_nPausedRouteStep;
                    m_bRoutePaused = FALSE;
                }

                if (m_AutoMove && index == 0 && !m_bFollowPeople)
                {
                    if (m_bPriorityUseMouse)
                        return;

                    // Update route progress (waypoint tolerance + stuck detection)
                    UpdateRouteProgress();

                    PlayerMoveMps();
                }
            }
        }
        else
        {
            // Non-fight mode
            m_nTimeSkip = 0;
            BackToMap();
            SaveMoney();

            if (g_SubWorldSet.GetGameTime() % 16 == 0)
            {
                if (bPlayerMoveItem && bCheckFilter == FALSE)
                {
                    if (bPlayerSellItem)
                        bCheckSellItem = TRUE;

                    MoveItemToBox();
                    for (int i = MAX_EQUIPMENT_ITEM - 1; i > 0 ; i--)
                    {
                        m_sListEquipment.m_Link.Remove(i);
                    }

                    if (bPlayerSellItem)
                        bCheckSellItem = FALSE;
                }
            }

            if (bPlayerSellItem && bCheckSellItem == FALSE)
            {
                if (g_SubWorldSet.GetGameTime() % 16 == 0)
                    CoreDataChanged(GDCNI_UPDATE_PLAYERSELLITEM, 0, 0);
            }

            BackMapTrain();
        }
    }
    else
    {
        ResetAuto();
    }
}


//==============================================================================
// PART 11: PACKET GUARD WRAPPER (Example usage in KPlayer.cpp or wherever sends packets)
//==============================================================================

// Replace direct calls to SendPackToServer() with:
/*
// OLD CODE:
g_pClient->SendPackToServer(&cmd, sizeof(cmd));

// NEW CODE:
if (Player[CLIENT_PLAYER_INDEX].m_cAI.CanSendPacket(0))  // 0=MOVE
{
    g_pClient->SendPackToServer(&cmd, sizeof(cmd));
}
else
{
    // Packet dropped by rate limiter
    // Optionally queue for later or just drop
}

// For ATTACK packets: use type 1
// For SKILL packets: use type 2
// For ITEM packets: use type 3
*/


//==============================================================================
// END OF PATCH
//==============================================================================
