# Training Area Radius - Implementation Guide

## Overview

This feature adds a configurable radius limit for the Auto-Play training area. When enabled, the character will stay within a specified distance from the preset waypoints and return to them when:
1. The leader is lost (in follow mode)
2. The character goes too far from waypoints while chasing NPCs

---

## Changes Applied

### FILE 1: KPlayerAI.h

**Added Fields (Lines 190-192)**:
```cpp
// Training area radius limit (prevent running too far from waypoints)
int				m_nTrainingRadius;			// Max distance from waypoints (in cells, 0 = unlimited)
BOOL			m_bStayInTrainingArea;		// Force stay within training radius?
```

**Added Method Declarations (Lines 289-291)**:
```cpp
// ===== TRAINING AREA RADIUS ENFORCEMENT =====
BOOL			IsWithinTrainingArea(int *outNearestWaypointIdx);
void			ReturnToTrainingArea();
```

---

### FILE 2: KPlayerAI.cpp

**Field Initialization in Release() (Lines 193-195)**:
```cpp
// Training area radius
m_nTrainingRadius = 0;			// Default: unlimited
m_bStayInTrainingArea = FALSE;	// Default: off
```

**New Methods (Lines 3704-3789)**:

#### IsWithinTrainingArea()
Checks if character is within m_nTrainingRadius (in cells) of any waypoint.
- Returns TRUE if within radius or feature disabled
- Returns FALSE if out of bounds
- Outputs nearest waypoint index via pointer parameter

#### ReturnToTrainingArea()
Forces character to return to nearest waypoint when:
- Out of training area radius
- Following leader but leader is lost (m_bFollowPeople=TRUE, m_FollowPeopleIdx=0)

**Updated Active() Logic (Lines 846-858)**:
```cpp
else if (m_bFollowPeople && m_FollowPeopleIdx == 0 && m_AutoMove)
{
	// FIX: When following but lost leader, return to training waypoints
	// instead of staying at current position
	ReturnToTrainingArea();
}

// Check if out of training area radius - return to waypoints
int nearestWp = -1;
if (m_bStayInTrainingArea && !IsWithinTrainingArea(&nearestWp))
{
	ReturnToTrainingArea();
}
```

**Updated FindNearNpc2Array() (Lines 810-838)**:
Filters out NPCs that are outside the training area radius before selecting them as targets. This prevents the character from chasing enemies too far from waypoints.

---

## Configuration

### Fields to Expose in UI

The Auto-Play UI should add these controls:

1. **Checkbox**: "Stay in Training Area"
   - Binds to: `m_bStayInTrainingArea`
   - Default: FALSE (off)

2. **Numeric Input**: "Training Radius (cells)"
   - Binds to: `m_nTrainingRadius`
   - Default: 0 (unlimited)
   - Range: 0-100 cells
   - Note: 1 cell = 32 MPS units

### Behavior

**When Enabled (`m_bStayInTrainingArea = TRUE`)**:
- Character will only engage NPCs within `m_nTrainingRadius` cells of any waypoint
- If character goes outside radius (by chasing NPC or following leader), it will return to nearest waypoint
- When leader is lost, character returns to waypoints instead of staying at current position

**When Disabled (`m_bStayInTrainingArea = FALSE`)**:
- No radius enforcement
- Character can chase NPCs anywhere
- When leader lost, returns to waypoints (if `m_AutoMove = TRUE`)

---

## Logic Priority

The new priority when Auto-Play is active:

1. **Follow Leader** (if `m_bFollowPeople = TRUE` and leader found):
   - Attack leader's target
   - Stay near leader
   - No radius check (leader takes priority)

2. **Leader Lost** (if `m_bFollowPeople = TRUE` but leader not found):
   - **NEW**: Return to preset waypoints (if `m_AutoMove = TRUE`)
   - Train around waypoints
   - Only engage NPCs within radius (if enabled)

3. **Waypoint Route** (if `m_AutoMove = TRUE` and not following):
   - Follow waypoint route
   - Engage nearby NPCs
   - Only engage NPCs within radius (if enabled)
   - Return to waypoints if out of bounds

4. **Free Roam** (if both `m_bFollowPeople = FALSE` and `m_AutoMove = FALSE`):
   - Attack nearby NPCs around current position
   - No waypoint logic

---

## Testing Checklist

- [ ] Character stays within radius when training with waypoints
- [ ] Character ignores NPCs outside radius
- [ ] When leader is lost, character returns to waypoints
- [ ] Character continues route from nearest waypoint after returning
- [ ] Radius check disabled when `m_bStayInTrainingArea = FALSE`
- [ ] Radius of 0 means unlimited (same as disabled)
- [ ] UI controls properly bind to `m_nTrainingRadius` and `m_bStayInTrainingArea`
- [ ] Radius in cells (1 cell = 32 pixels) works correctly

---

## UI Implementation TODO

**Location**: Need to find Auto-Play UI file (likely in `SwordOnline/Sources/S3Client/Ui/...`)

**Required Changes**:
1. Add checkbox control for "Stay in Training Area"
2. Add numeric input for training radius
3. Bind controls to `Player[CLIENT_PLAYER_INDEX].m_cAI.m_nTrainingRadius` and `m_bStayInTrainingArea`
4. Save/load these settings with other Auto-Play configuration

**Search Hints**:
```bash
# Find Auto-Play UI files
find . -name "*Auto*.cpp" -o -name "*Auto*.h"
grep -r "m_bFollowPeople" --include="*.cpp" --include="*.h" SwordOnline/Sources/S3Client/
```

---

## Summary

| Component | Status | Lines Changed |
|-----------|--------|---------------|
| KPlayerAI.h | ✅ Complete | +4 fields, +2 methods |
| KPlayerAI.cpp | ✅ Complete | +3 init, +85 implementation, +30 filter logic |
| UI Integration | ⚠️ TODO | Need to find and update UI file |
| **Total** | **85% Complete** | **~122 lines** |

---

## Expected Behavior

**Before**:
- When leader lost: Character stays at current position, attacks nearby NPCs
- Can chase NPCs indefinitely far from waypoints

**After**:
- When leader lost: Character returns to nearest waypoint, continues route
- Character only engages NPCs within radius of waypoints (configurable)
- Prevents running too far from training area
- More controlled and predictable training behavior
