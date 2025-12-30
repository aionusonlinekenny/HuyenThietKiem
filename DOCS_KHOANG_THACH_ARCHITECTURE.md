# Khoáng Thạch (Mineral Stone) System Architecture

## Overview
Khoáng thạch items (detail 146-151) are special items that extract and store equipment attributes, preserving the attribute type, min/max range, actual value, and **Series (element type)**.

## Critical Architecture Components

### 1. Dual Storage Pattern
Khoáng thạch attributes are stored in **TWO places** for different purposes:

```cpp
// Storage 1: m_aryMagicAttrib[0] - For tooltip display
pItem->m_aryMagicAttrib[0].nAttribType = Type;  // e.g., 115
pItem->m_aryMagicAttrib[0].nMin = Min;          // e.g., 10
pItem->m_aryMagicAttrib[0].nMax = Max;          // e.g., 30
pItem->m_aryMagicAttrib[0].nValue[0] = Value;   // e.g., 30

// Storage 2: m_GeneratorParam.nGeneratorLevel[0-5] - For ITEM_SYNC protocol + Lua access
pItem->m_GeneratorParam.nGeneratorLevel[0] = Type;    // e.g., 115
pItem->m_GeneratorParam.nGeneratorLevel[1] = Min;     // e.g., 10
pItem->m_GeneratorParam.nGeneratorLevel[2] = Max;     // e.g., 30
pItem->m_GeneratorParam.nGeneratorLevel[3] = Value;   // e.g., 30
pItem->m_GeneratorParam.nGeneratorLevel[4] = Series;  // e.g., 3 (Hỏa/Fire) ⚠️ CRITICAL!
pItem->m_GeneratorParam.nGeneratorLevel[5] = 0;       // Unused
```

**Why both?**
- `m_aryMagicAttrib`: Client-side rendering (tooltips)
- `m_GeneratorParam`: Server-side persistence, client sync via ITEM_SYNC protocol, Lua API access

### 2. Item Creation Flow (Two Paths!)

#### Path A: Add() - Used by Lua AddItemEx
```
Lua AddItemEx()
  → LuaAddItemEx() (ScriptFuns.cpp:2657)
  → ItemSet.Add() (KItemSet.cpp:161)
  → Gen_Script() (generates base item)
  → [KHOANG ADD] Convert GenLvl to MagicAttrib (KItemSet.cpp:215-234)
  → SetGeneratorLevel(pnMagicLevel) to preserve all 6 values
```

#### Path B: AddExist() - Used internally by C++ code
```
C++ Code
  → ItemSet.AddExist() (KItemSet.cpp:227)
  → Gen_ExistScript() (loads template, overwrites item)
  → [KHOANG SAVE/RESTORE] Save GenLvl before, restore after (KItemSet.cpp:246-313)
  → [KHOANG ADDEXIST] Convert GenLvl to MagicAttrib (KItemSet.cpp:318-337)
  → SetGeneratorLevel(pnMagicLevel) to preserve all 6 values
```

**CRITICAL:** Both paths MUST handle khoang thach specially!

### 3. Gen_Script vs Gen_ExistScript
- `Gen_Script()`: Used by Add() - generates new item from template
- `Gen_ExistScript()`: Used by AddExist() - loads existing item template
- **BOTH overwrite the item structure**, clearing generator levels!
- **Solution**: Save before, restore after, then set again to ensure persistence

### 4. Database Persistence (KPlayerDBFuns.cpp:933-945)

```cpp
// For khoang thach: Store magic attributes in generator levels
if (pItemData->igenre == 7 && pItemData->idetailtype >= 146 && pItemData->idetailtype <= 151)
{
    pItemData->imagiclevel1 = Item[nItemIndex].m_aryMagicAttrib[0].nAttribType;
    pItemData->imagiclevel2 = Item[nItemIndex].m_aryMagicAttrib[0].nMin;
    pItemData->imagiclevel3 = Item[nItemIndex].m_aryMagicAttrib[0].nMax;
    pItemData->imagiclevel4 = Item[nItemIndex].m_aryMagicAttrib[0].nValue[0];
    // ⚠️ CRITICAL: Read from m_GeneratorParam, NOT hardcoded!
    pItemData->imagiclevel5 = Item[nItemIndex].m_GeneratorParam.nGeneratorLevel[4];  // Series
    pItemData->imagiclevel6 = Item[nItemIndex].m_GeneratorParam.nGeneratorLevel[5];  // Unused
}
```

### 5. Lua API Access

#### GetItemGeneratorLevels (ScriptFuns.cpp:10419-10459)
```lua
local nType, nMin, nMax, nValue, nSeries, nUnused = GetItemGeneratorLevels(nItemIdx)
```
Returns: `pGenParam->nGeneratorLevel[0-5]`

#### GetItemMagicAttribInfo (ScriptFuns.cpp:10231-10279)
```lua
local nType, nValue, nMin, nMax = GetItemMagicAttribInfo(nItemIdx, nSlot)
```
Returns: `m_aryMagicAttrib[nSlot]`

## Common Bugs & Fixes

### Bug 1: Missing Add() Path Handling
**Symptom**: Series=0 in newly created items via Lua AddItemEx

**Cause**: Only fixed AddExist(), but Lua uses Add()!

**Fix**: Add khoang thach handling in BOTH Add() and AddExist()
```cpp
// KItemSet.cpp:212-234 (Add function)
if (nItemGenre == item_script && nDetailType >= 146 && nDetailType <= 151)
{
    // Convert GenLvl to MagicAttrib
    pItem->m_aryMagicAttrib[0].nAttribType = pnMagicLevel[0];
    // ... set other fields ...

    // Call SetGeneratorLevel again to ensure persistence
    pItem->SetGeneratorLevel(pnMagicLevel);
}
```

### Bug 2: SetItemMagicAttrib Overwrites Series
**Symptom**: Series correct at creation, but becomes 0 after SetItemMagicAttrib is called

**Cause**: SetItemMagicAttrib hardcoded GenLvl[4]=0 and GenLvl[5]=0

**Fix**: Preserve existing Series value
```cpp
// ScriptFuns.cpp:10399-10408
// Save existing Series BEFORE modifying
int nExistingSeries = pGenParam->nGeneratorLevel[4];

pGenParam->nGeneratorLevel[0] = nAttribType;
pGenParam->nGeneratorLevel[1] = nMin;
pGenParam->nGeneratorLevel[2] = nMax;
pGenParam->nGeneratorLevel[3] = nValue;
// PRESERVE existing Series instead of hardcoding to 0!
pGenParam->nGeneratorLevel[4] = nExistingSeries;
pGenParam->nGeneratorLevel[5] = 0;
```

### Bug 3: Database Save Hardcoding Series=0
**Symptom**: Series correct in-game, but becomes 0 after logout/login

**Cause**: KPlayerDBFuns.cpp hardcoded imagiclevel5=0 and imagiclevel6=0

**Fix**: Read from m_GeneratorParam instead
```cpp
// Read from actual item data, don't hardcode!
pItemData->imagiclevel5 = Item[nItemIndex].m_GeneratorParam.nGeneratorLevel[4];
pItemData->imagiclevel6 = Item[nItemIndex].m_GeneratorParam.nGeneratorLevel[5];
```

### Bug 4: Wrong Attribute Extraction (Position vs Slot)
**Symptom**: Detail 146 extracts attribute from slot 0, but user wants 1st VISIBLE attribute

**Cause**: Direct mapping detail→slot (146→0) instead of finding Nth visible attribute

**Fix**: Enumerate all slots, count visible attributes (Type > 0), select Nth one
```lua
-- compound_master.lua:524-556
local nTargetPosition = nKhoangDetail - 145  -- 146->1, 147->2, etc.
local nCurrentPosition = 0
for nSlot = 0, 5 do
    local nSlotOp, nSlotValue, nSlotMin, nSlotMax = GetItemMagicAttribInfo(nEquipIdx, nSlot)
    if nSlotOp and nSlotOp > 0 then
        nCurrentPosition = nCurrentPosition + 1
        if nCurrentPosition == nTargetPosition then
            -- Found target attribute!
            break
        end
    end
end
```

## Debugging Checklist

When debugging khoang thach issues, check:

1. **Creation**: `[KHOANG ADD]` or `[KHOANG ADDEXIST]` shows GenLvl=[Type,Min,Max,Value,**Series**,0]
2. **After SetItemMagicAttrib**: `[KHOANG SETATTRIB]` shows `Series=X (preserved)`
3. **Database Save**: `[KHOANG SAVE]` shows GenLvl with correct Series
4. **Database Load**: `[KHOANG LOAD]` shows correct AttribType and Series
5. **OnUse**: `[ONUSE DEBUG]` shows `Series=X (from GenLvl[4])` with correct value

## Critical Rules

1. **ALWAYS set BOTH m_aryMagicAttrib AND m_GeneratorParam** for khoang thach
2. **NEVER hardcode GenLvl[4]=0 or GenLvl[5]=0** - preserve existing values!
3. **Check BOTH Add() and AddExist() code paths** when making changes
4. **Test with NEW items** - old database items may have stale data
5. **Verify Series value at EVERY step** from creation to OnUse

## Synchronization Flow

```
Server Item Creation
  ↓
m_aryMagicAttrib[0] ← Set for tooltip
m_GeneratorParam.nGeneratorLevel[0-5] ← Set for persistence
  ↓
Database Save (KPlayerDBFuns.cpp)
  ↓
imagiclevel1-6 ← From m_aryMagicAttrib + m_GeneratorParam
  ↓
ITEM_SYNC Protocol
  ↓
Client Receives
  ↓
Client m_GeneratorParam.nGeneratorLevel[0-5]
  ↓
Lua GetItemGeneratorLevels() → Returns Series
  ↓
OnUse Dialog Display
```

## Files Modified (Commit History)

1. `compound_master.lua` - Attribute extraction by visible position
2. `item_xuantiekuang.lua` (+ 5 other khoang scripts) - Read from slot 0
3. `KItemSet.cpp` - Add() and AddExist() khoang thach handling
4. `KPlayerDBFuns.cpp` - Database save with correct Series
5. `ScriptFuns.cpp` - SetItemMagicAttrib Series preservation

## Testing Protocol

1. Create equipment with Series=3 (Hỏa/Fire)
2. Place visible attribute at position 1
3. Extract with detail 146 khoang thach
4. Check server log: `[KHOANG ADD] GenLvl=...,3,0`
5. Check server log: `[KHOANG SETATTRIB] Series=3 (preserved)`
6. Right-click khoang thach
7. Verify OnUse dialog: "Ngũ Hành: Hỏa" (NOT "Kim")
8. Logout/Login
9. Right-click again - should still show "Hỏa"

---

**Document Version**: 1.0
**Last Updated**: 2025-12-30
**Author**: Claude Code Analysis
