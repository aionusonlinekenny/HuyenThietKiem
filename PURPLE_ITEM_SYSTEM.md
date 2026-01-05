# Purple Item Enchasing System - Implementation Guide

## Overview
This document records all fixes and implementation details for the purple item (trang bị tím) enchasing system in JXOnline game server.

---

## System Architecture

### Components
1. **Client UI** (C++): `UiCompoundItem.cpp` - Main crafting interface with tabs
2. **Client Inventory** (C++): `UiItem.cpp` - Inventory window management
3. **Server Logic** (Lua): `script/item/紫装合成/` - Purple item creation and enchasing
4. **Protocol**: Client-Server communication via `GOI_*` commands and `GDI_*` queries

### Workflow
```
[Blue Item + Materials] → BuildBox → Create Purple Item (6 empty slots)
→ Purple Item + Minerals → Enchase Slots → Final Purple Item
```

---

## Critical Fixes Applied

### 1. Client Crash on Close Button (Commit: bc2d6b141)

**Problem**:
- Clicking close button crashed client immediately
- Log showed: "Returning 17 items from BuildBox"

**Root Cause**:
```cpp
// WRONG CODE:
int nCount = g_pCoreShell->GetGameData(GDI_BUILD_ITEM, &Item, 0);
// ↑ Returns ALL 17 craftable items in inventory, NOT items in BuildBox!

if (nCount > 0) {  // Always true → tries to return all 17 items → CRASH
    g_pCoreShell->OperationRequest(GOI_RECOVERY_BOX_COMMAND, pos_builditem, 0);
}
```

**Fix**:
```cpp
// CORRECT CODE:
g_pCoreShell->OperationRequest(GOI_RECOVERY_BOX_COMMAND, pos_builditem, 0);
// ↑ Call unconditionally - server filters items by container position
```

**Key Learning**:
- `GDI_BUILD_ITEM` returns ALL craftable items globally, not container-specific
- `GOI_RECOVERY_BOX_COMMAND` server operation filters by `pos_builditem` parameter
- Trust server-side filtering instead of client-side item counting

**File**: `SwordOnline/Sources/S3Client/Ui/UiCase/UiCompoundItem.cpp:135-154`

---

### 2. ESC Key Not Working (Commits: 989ddfab5, bee4a83df, 62005161d)

**Problem**:
- Pressing ESC didn't close crafting UI
- Only closed inventory window
- User observation: "Lúc mở UI tạo trang bị tím thì có mở luôn inventory, bấm ESC chỉ có inventory đóng"

**Debug Phase** (989ddfab5):
Added logging to all 6 WM_KEYDOWN handlers:
```cpp
case WM_KEYDOWN:
    g_DebugLog("[COMPOUND-MAIN] WM_KEYDOWN received: uParam=%d", uParam);
    if (uParam == VK_ESCAPE) {
        g_DebugLog("[COMPOUND-MAIN] ESC pressed, calling CloseWindow()");
        CloseWindow();
    }
    break;
```

**Result**: NO logs appeared when pressing ESC → Messages not reaching WndProc!

**Z-Order Fix Attempt** (bee4a83df):
```cpp
m_pSelf->BringToTop();           // First call
KUiItem::OpenWindow();           // Opens inventory
m_pSelf->BringToTop();           // Second call - bring back to top
g_DebugLog("[COMPOUND-MAIN] BringToTop called after inventory open");
```

**Result**: Still didn't work - Z-order ≠ keyboard focus in this engine

**Final Solution - Cascade Close** (62005161d):
Modified inventory's CloseWindow() to detect and close crafting UI:

```cpp
// File: UiItem.cpp
#include "UiCompoundItem.h"  // Line 23

void KUiItem::CloseWindow(bool bDestroy)
{
    if (m_pSelf)
    {
        // When inventory closes, close crafting UI if it's open
        if (KUiComItem::GetIfVisible())
        {
            g_DebugLog("[INVENTORY] Closing crafting UI along with inventory");
            KUiComItem::CloseWindow();
        }
        // ... rest of close logic
    }
}
```

**How It Works**:
1. User presses ESC
2. Game engine's global handler closes inventory (before WndProc)
3. `KUiItem::CloseWindow()` detects crafting UI is open
4. Calls `KUiComItem::CloseWindow()` to close crafting UI
5. Both windows close together, items return safely

**Key Learning**:
- Game engine has global ESC handler that processes before window messages
- Cascade close pattern works when direct keyboard routing doesn't
- Dependent windows should close together for better UX

**Files**:
- `SwordOnline/Sources/S3Client/Ui/UiCase/UiItem.cpp:23,73-79`
- `SwordOnline/Sources/S3Client/Ui/UiCase/UiCompoundItem.cpp:111-119`

---

### 3. Effect Animation (Previous Session)

**Problem**: No visual effect when enchasing success

**Fix**: Added effect sprite INI sections

**File**: `Bin/Client/Ui/Ui3/UiCompoundItem_Inout.ini:158-183`

```ini
[Effect_0]
Left=2
Top=0
Width=120
Height=200
Image=\Spr\Ui3\物品合成\合成特效.spr
Moveable=0
Trans=0
```

---

### 4. Movement Lock (Previous Session)

**Problem**: Player could move while crafting UI open

**Fix**:
```cpp
// On UI open:
Wnd_GameSpaceHandleInput(false);  // Lock movement

// On UI close:
Wnd_GameSpaceHandleInput(true);   // Unlock movement
```

**File**: `SwordOnline/Sources/S3Client/Ui/UiCase/UiCompoundItem.cpp`

---

## Important Code Patterns

### Container Positions
```cpp
pos_builditem = 15    // BuildBox for crafting operations
```

### Item Recovery
```cpp
// Always call unconditionally - server filters automatically
g_pCoreShell->OperationRequest(GOI_RECOVERY_BOX_COMMAND, pos_builditem, 0);
```

### Window Management
```cpp
// Open with movement lock
Wnd_GameSpaceHandleInput(false);
m_pSelf->Show();
m_pSelf->BringToTop();

// Close with cleanup
Wnd_GameSpaceHandleInput(true);
KUiComItem::CloseWindow();
```

### Cascade Close for Dependent Windows
```cpp
// In parent window close handler:
if (DependentWindow::GetIfVisible())
{
    DependentWindow::CloseWindow();
}
```

---

## Testing Checklist

- [ ] Create purple item from blue item (6 empty slots)
- [ ] Close UI with close button (no crash)
- [ ] Close UI with ESC key (both inventory and crafting UI close)
- [ ] Items return to inventory on close
- [ ] Player movement locked while UI open
- [ ] Effect animation plays on enchase success
- [ ] All tabs switch correctly (COMPOUND, DISTILL, FORGE, ENCHASE, ATLAS)

---

## Series (Ngũ Hành) System - IMPLEMENTED

### Overview
Series (Ngũ Hành / Five Elements) is a fundamental property in JXOnline:
- **Values**: 0=Kim (Metal), 1=Moc (Wood), 2=Thuy (Water), 3=Hoa (Fire), 4=Tho (Earth)
- **Item-level property**: Series belongs to the item, not individual attributes
- **Purple item series**: Inherited from the blue item used to create it

### Purple Item Creation (FORGE Tab)
**File**: `Bin/Server/script/npc/compound_master.lua:87-184`

When creating purple item from blue item:
```lua
-- Line 107: Get blue item properties including series
local nGenre, nDetail, nParti, nLevel, nSeries, nLuck = GetItemProp(nEquipIdx)

-- Line 173: Pass series to AddItemPurple
local nPurpleIdx = AddItemPurple(nDetail, nParti, nLevel, nSeries)
```

**C++ Implementation**: `SwordOnline/Sources/Core/Src/ScriptFuns.cpp:10573-10700`
```cpp
// Line 10592: Series parameter received
int nSeries = (int)Lua_ValueToNumber(L, 4);

// Line 10633: Series passed to ItemSet.Add
const int nItemIdx = ItemSet.Add(
    item_purpleequip,  // genre
    nSeries,           // series (element) ← CRITICAL
    nLevel,            // level
    ...
);
```

**Result**: Purple item has same series as the blue item used to create it!

---

### Enchasing (ENCHASE Tab) - Series Validation
**File**: `Bin/Server/script/npc/compound_master.lua:619-736`

#### Mineral Types
- **146** (Huyen Thiet Nguyen Khoang) - Flexible series
- **147** (Khong Tuoc Nguyen Thach) - Strict series
- **148** (Mat Ngan Nguyen Khoang) - Flexible series
- **149** (Phu Dung Nguyen Thach) - Strict series
- **150** (Chu Sa Nguyen Khoang) - Flexible series
- **151** (Chung Nho Nguyen Thach) - Strict series

#### Validation Logic (Lines 672-711)

**Strict Minerals (147, 149, 151)**:
- MUST have same series as purple item
- If series mismatch: Show error and block enchasing
- Error message: "Khoang thach (X) khong cung ngu hanh voi trang bi (Y)! Khong the kham nam!"

```lua
if bStrictMineral then
    if nKhoangSeries ~= nPurpleSeries then
        Talk(1, "", format(
            "<color=red>Khoang thach (%s) khong cung ngu hanh voi trang bi (%s)! Khong the kham nam!<color>",
            szKhoangSeries, szPurpleSeries
        ))
        return
    end
end
```

**Flexible Minerals (146, 148, 150)**:
- NO series validation required
- Can enchase regardless of mineral series
- Attribute automatically inherits purple item's series

```lua
else
    -- Minerals 146/148/150: No series validation needed
    Msg2Player(format(
        "[NGU HANH] Khoang thach loai %d khong can kiem tra ngu hanh, cho phep kham nam",
        nKDetail
    ))
end
```

---

### Series Architecture Notes

1. **Series is item-level, not attribute-level**
   - Each item has ONE series that applies to all its attributes
   - Individual magic attributes do NOT have separate series values

2. **Khoang thach (mineral) series storage**
   - Stored in generator levels: `GetItemGeneratorLevels(nKhoangIdx)` returns 6 values
   - Position [5] = Series value (0-4 for Kim/Moc/Thuy/Hoa/Tho)

3. **Why two mineral types?**
   - **Strict (147/149/151)**: Prevent players from enchasing wrong-element minerals
     - Example: Can't enchase Fire mineral onto Water equipment
   - **Flexible (146/148/150)**: Allow cross-element enchasing for flexibility
     - Example: Can use any-element mineral, attribute adopts equipment's element

4. **Attribute inheritance**
   - When enchasing, attribute's effective series = purple item's series
   - This happens automatically because series is item-level property
   - No special code needed to "convert" attribute series

---

### Testing Series Logic

**Test Case 1: Purple Item Creation**
1. Get blue item with Series=2 (Thuy/Water)
2. Create purple item from it
3. Verify: Purple item has Series=2 (same as blue item)

**Test Case 2: Strict Mineral Enchasing**
1. Purple item Series=2 (Thuy/Water)
2. Mineral 147 Series=2 (Thuy/Water) → Should succeed
3. Mineral 147 Series=3 (Hoa/Fire) → Should fail with error message

**Test Case 3: Flexible Mineral Enchasing**
1. Purple item Series=2 (Thuy/Water)
2. Mineral 146 Series=3 (Hoa/Fire) → Should succeed (no validation)
3. Enchased attribute inherits Series=2 from purple item

---

---

## Commit History
```
(pending) - FIX: Series display in tooltip - Gen_ExistPurpleEquipment was overriding series
e994ce103 - FIX: Khoang thach series inheritance - Gen_Script was overriding series
8606da9a8 - FIX: Purple item series inheritance - SetAttrib_CBR was overriding series
16b0bdcf7 - DEBUG: Add series validation logs for purple item creation
be051a15b - FIX: Keep materials in UI when enchasing validation fails
8c861197d - FEATURE: Add series validation for purple item enchasing system
62005161d - FIX: ESC key now closes crafting UI by closing it when inventory closes
bee4a83df - FIX: ESC key not working - inventory window stealing keyboard input
989ddfab5 - DEBUG: Add comprehensive logging to ESC key handlers
bc2d6b141 - FIX: Crash on close and ESC key not working
7d535a6d9 - FEATURE: Add ghost item fix, movement lock, and ESC key handler
```

---

## Latest Fixes: Series Inheritance (e994ce103 + 8606da9a8)

### Problem
Purple items and khoang thach (minerals) were created with series = -1 (no element) even though correct series was passed to creation functions. Debug logs showed:
- Blue item series = 2 (Thuy/Water)
- Purple item series = -1 (Unknown) ← WRONG!
- Khoang thach series = -1 (Unknown) ← WRONG!

### Root Cause
Both `Gen_PurpleEquipment()` and `Gen_Script()` use assignment operators that override series:
1. **Purple items**: `SetAttrib_CBR()` uses `*this = *pData` to copy equipment table data
2. **Khoang thach**: `Gen_Script()` uses `*pItem = *pScript` to copy script table data

Equipment/script tables don't have fixed series (default -1), so assignment overwrites the series parameter.

**Flow (BEFORE fix)**:
```cpp
// Purple item
SetSeries(nSeriesReq);  // series = 2
SetAttrib_CBR(pEqu);    // *this = *pData → series = -1 (OVERRIDE!)
SetGenre(item_purpleequip);
// Result: series = -1

// Khoang thach
Gen_Script(detail, pItem);  // *pItem = *pScript → series = -1 (OVERRIDE!)
// Result: series = -1
```

### Solution
Added `SetSeries()` calls AFTER the overriding operations to restore series value:

**Flow (AFTER fix)**:
```cpp
// Purple item (KItemGenerator.CPP line 384-386)
SetSeries(nSeriesReq);  // series = 2
SetAttrib_CBR(pEqu);    // *this = *pData → series = -1
SetSeries(nSeriesReq);  // series = 2 (RESTORE!)
SetGenre(item_purpleequip);
// Result: series = 2 ✓

// Khoang thach (KItemSet.cpp line 203-206)
Gen_Script(detail, pItem);  // *pItem = *pScript → series = -1
SetSeries(nSeries);         // series = 2 (RESTORE!)
// Result: series = 2 ✓
```

**Files Modified**:
- `SwordOnline/Sources/Core/Src/KItemGenerator.CPP:384-386` (purple items)
- `SwordOnline/Sources/Core/Src/KItemSet.cpp:203-206` (khoang thach)

### Impact
- Purple items now correctly inherit series from blue items
- Khoang thach minerals retain series from source equipment during extraction
- Series validation for enchasing works properly (147/149/151 minerals)
- Cross-element enchasing allowed for flexible minerals (146/148/150)

---

## Earlier Fix: Material Box Preservation (be051a15b)

### Problem
When series validation failed (e.g., Fire mineral on Water equipment), material boxes (Box1/Box2) were cleared even though enchasing didn't succeed. Items remained in BuildBox on server, but client UI showed empty boxes. User had to switch tabs to refresh UI.

### Root Cause
Animation `Breathe()` cleared boxes BEFORE sending request to server:
1. Animation completes (55 frames)
2. **Client clears Box1/Box2** ← Too early!
3. Client sends enchase request
4. Server validates series, fails, returns error
5. Items still in BuildBox, but UI already cleared

### Solution
Removed premature box clearing (lines 2798-2799). Now boxes only clear when server successfully deletes items:
- **Success**: Server deletes → `s2cRemoveItem` → `UpdateItem(bAdd=false)` → Boxes clear
- **Validation fail**: Server returns error → Items stay in BuildBox → Boxes still show materials

**File**: `SwordOnline/Sources/S3Client/Ui/UiCase/UiCompoundItem.cpp:2797-2801`

---

## Latest Fix: Series Display in Tooltip (2026-01-05)

### Problem
Purple items had series set correctly internally (debug logs showed series = 2), but tooltips didn't display "Thuộc tính ngũ hành: Thủy" line. The series appeared in debug logs when items were first created, but disappeared when items were loaded from inventory.

### Root Cause
**Gen_ExistPurpleEquipment()** - Used when loading existing purple items from database - was missing the second `SetSeries()` call after `SetAttrib_CBR()`.

**Flow (BEFORE fix)**:
```cpp
// Gen_ExistPurpleEquipment (line 524-540)
pItem->SetSeries(nSeries);      // series = 2
pItem->SetAttrib_CBR(pEqu);     // *this = *pData → series = -1 (OVERRIDE!)
pItem->SetGenre(item_purpleequip);
// Result: series = -1, tooltip shows nothing
```

**Why tooltip was empty**:
- `KItem::GetDesc()` checks `m_CommonAttrib.cSeries` in switch statement (KItem.cpp:1220-1237)
- When series = -1, doesn't match any case (0=Kim, 1=Moc, 2=Thuy, 3=Hoa, 4=Tho)
- No default case → no series text added to tooltip

### Solution
Added `SetSeries(nSeries)` call AFTER `SetAttrib_CBR()` to restore series value, matching the fix in `Gen_PurpleEquipment()`.

**Flow (AFTER fix)**:
```cpp
// Gen_ExistPurpleEquipment (line 524-540)
pItem->SetSeries(nSeries);      // series = 2
pItem->SetAttrib_CBR(pEqu);     // *this = *pData → series = -1
pItem->SetSeries(nSeries);      // series = 2 (RESTORE!)
pItem->SetGenre(item_purpleequip);
// Result: series = 2, tooltip shows "Thuộc tính ngũ hành: Thủy" ✓
```

**File Modified**:
- `SwordOnline/Sources/Core/Src/KItemGenerator.CPP:534-536` (Gen_ExistPurpleEquipment)

### Impact
- Purple items now display correct series in tooltip when loaded from inventory
- Consistent with newly created purple items
- Series validation for enchasing works properly with loaded items

---

**Last Updated**: 2026-01-05
**Branch**: claude/analyze-branch-differences-KD0Bv
**Status**: Core system complete, all fixes applied including tooltip display
