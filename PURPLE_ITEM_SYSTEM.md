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
[PENDING] - FEATURE: Add series validation for purple item enchasing system
62005161d - FIX: ESC key now closes crafting UI by closing it when inventory closes
bee4a83df - FIX: ESC key not working - inventory window stealing keyboard input
989ddfab5 - DEBUG: Add comprehensive logging to ESC key handlers
bc2d6b141 - FIX: Crash on close and ESC key not working
7d535a6d9 - FEATURE: Add ghost item fix, movement lock, and ESC key handler
```

---

**Last Updated**: 2026-01-05
**Branch**: claude/analyze-branch-differences-KD0Bv
**Status**: Core system complete, series validation implemented
