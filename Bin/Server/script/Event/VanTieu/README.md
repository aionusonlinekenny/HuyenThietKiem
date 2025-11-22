# Vận Tiêu (Escort Event) - Implementation Guide

## Overview
Vận Tiêu là event hộ tống xe tiêu từ Thành Đô đến Thanh Thành Sơn. Event được port từ ThienDieuOnline sang HuyenThietKiem với một số modifications và workarounds.

## Status: ⚠️ PARTIALLY IMPLEMENTED

Scripts đã sẵn sàng nhưng cần hoàn thiện một số components.

---

## Files Structure

```
/Bin/Server/script/Event/VanTieu/
├── README.md              (this file)
├── lib.lua                (constants & config)
├── tieudau.lua            (Tiêu Đầu - Quest giver NPC)
├── tieusu.lua             (Tiêu Sư - Quest receiver NPC)
├── tieuxa.lua             (Escort cart NPC)
├── tieuxahong.lua         (Robbed cart NPC)
├── ruongcuop.lua          (Treasure chest)
├── bossvantieu.lua        (Boss monsters)
└── trap.lua               (Traps/blockades)
```

---

## Task IDs (Added to TaskLib.lua)

```lua
TASK_VANTIEU      = 750  -- Main quest task
TASK_NPCVANTIEU   = 751  -- Cart NPC ID
TASK_RESET_VANTIEU = 752 -- Daily reset counter
```

---

## Item IDs Needed

**CẦN TẠO TRONG ITEM DATABASE:**

| Item ID | Name | Type | Description |
|---------|------|------|-------------|
| 4771 | Tiêu Kỳ | Quest Item | Dropped when cart is robbed |
| 4772 | Vé Mở Khóa Vận Tiêu | Unlock Item | Allows extra daily runs |
| 4774 | Hồ Tiêu Lệnh | Currency | Event currency/reward |
| 4775 | Tăng Tốc | Consumable | Speed boost |
| 4776 | Hồi Máu | Consumable | HP recovery |
| 4778 | Dịch Chuyển | Consumable | Teleport item |
| 4838 | Rương Vận Tiêu | Reward Box | Event reward box |

---

## NPC Template IDs Needed

**CẦN TẠO TRONG NPC TEMPLATES:**

| Template ID | Name | Description |
|-------------|------|-------------|
| 2084 | Đồng Tiêu Xa | Bronze cart |
| 2085 | Bạc Tiêu Xa | Silver cart |
| 2086 | Vàng Tiêu Xa | Gold cart |
| 1185 | Tiêu Xa Hồng | Robbed cart (red) |
| 844 | Rương Cướp | Treasure chest |

**Tiêu Đầu** và **Tiêu Sư** - Cần tạo 2 NPCs tại:
- Tiêu Đầu: Thành Đô (hoặc major city) - link to `tieudau.lua`
- Tiêu Sư: Thanh Thành Sơn (243, 219) - link to `tieusu.lua`

---

## Missing Lua Functions (Workarounds Implemented)

Các functions sau THIẾU trong HuyenThiet, đã có **WORKAROUNDS** trong code:

### ✅ Có Workaround (tạm dùng được):

1. **AddRespect** → dùng `AddRepute` thay thế
2. **SetNpcValue/GetNpcValue** → dùng `SetNpcParam/GetNpcParam`
3. **FindNearNpc** → dùng `FindAroundNpc` (kém chính xác hơn)
4. **AddItemIDStack** → dùng loop `AddItem`

### ⚠️ Có Stub nhưng CHƯA HOÀN CHỈNH:

5. **SetNpcOwner** - NPC follow player
   - Hiện tại: Chỉ mark ownership trong param
   - Cần: Implement NPC follow mechanism

6. **SetNpcTimeout** - Auto despawn NPC
   - Hiện tại: Chỉ store timeout value
   - Cần: Implement auto-cleanup timer

7. **AddNpcWithScript** - Spawn NPC with custom script
   - Hiện tại: `AddNpc` + `SetNpcScript`
   - Works nhưng cần test

---

## Implementation Steps (TODO)

### Phase 1: Core Functions (CRITICAL) ✅ WORKAROUND DONE

Đã implement workarounds cho tất cả critical functions.

### Phase 2: Data Setup (REQUIRED) ⚠️ PENDING

**CẦN LÀM:**

1. **Tạo Items** (7 items)
   - File: Item database hoặc ItemTemplate.txt
   - Add icons/sprites nếu cần custom

2. **Tạo NPC Templates** (5 NPCs)
   - File: NPC template database
   - Set sprites (có thể dùng existing)
   - Link scripts

3. **Tạo Quest NPCs**
   - Tiêu Đầu NPC tại Thành Đô
   - Tiêu Sư NPC tại Thanh Thành Sơn

### Phase 3: C++ Enhancement (OPTIONAL)

**Để event hoàn chỉnh 100%, cần implement trong C++:**

```cpp
// File: SwordOnline/Sources/Core/Src/ScriptFuns.cpp

1. LuaSetNpcOwner(Lua_State* L)
   - Make NPC follow player
   - Set Npc[nIdx].m_dwOwnerID = playerID
   - Implement follow AI

2. LuaSetNpcTimeout(Lua_State* L)
   - Set auto-despawn timer
   - Npc[nIdx].m_nTimeOut = nSeconds
   - Auto DelNpc when timeout

3. LuaFindNearNpc(Lua_State* L)
   - Search entire map for NPC by m_dwID
   - More accurate than FindAroundNpc

4. LuaAddRespect(Lua_State* L) (nếu khác AddRepute)
   - Player[idx].m_nRespect += nAmount
   - Sync to client
```

**Registration trong GameScriptFuns[]:**
```cpp
{"SetNpcOwner",    LuaSetNpcOwner},
{"SetNpcTimeout",  LuaSetNpcTimeout},
{"FindNearNpc",    LuaFindNearNpc},
{"AddRespect",     LuaAddRespect},
```

### Phase 4: Testing

**Test checklist:**

- [ ] Start quest (batdau)
- [ ] Cart spawns
- [ ] Cart follows player (if SetNpcOwner works)
- [ ] Navigate to receiver
- [ ] Complete quest (hoanthanh)
- [ ] Get rewards
- [ ] Cart robbery flow
- [ ] Chest loot
- [ ] Daily limits
- [ ] Shop system

---

## Configuration

### Adjust cho server của bạn:

**File: lib.lua**

```lua
-- Map IDs
SUBWORLD_START = 21  -- Change to your Thành Đô SubWorld ID

-- Coordinates
POS_START_X = 2623   -- Spawn position
POS_START_Y = 4503
POS_END_X = 243      -- Delivery position
POS_END_Y = 219

-- Costs & Rewards
COST_START_QUEST = 150000       -- Adjust cost
KINH_NGHIEM_BASE = 1000000      -- Adjust exp rewards

-- Limits
MAX_DAILY_RUNS = 3
CART_TIMEOUT = 30 * 60          -- 30 minutes
```

---

## Known Limitations

1. **NPC Follow** - Chưa implement, cart sẽ đứng yên
2. **Auto Despawn** - Chưa có, NPCs sẽ tồn tại mãi mãi
3. **FindNearNpc** - Dùng FindAroundNpc, có thể không tìm thấy cart nếu player đi xa

---

## How to Enable

1. **Tạo items** (7 items) trong database
2. **Tạo NPC templates** (5 templates)
3. **Tạo NPCs** trong game world:
   - Place Tiêu Đầu NPC tại main city
   - Place Tiêu Sư NPC tại destination
4. **Link scripts** cho NPCs
5. **Restart server**
6. **Test!**

---

## Future Enhancements

**Nice to have:**

- Team escort (party members help)
- Guild missions
- Weekly leaderboards
- Better UI (quest tracker)
- Anti-cheat (speed hack, teleport hack)
- More cart types
- Boss difficulty scaling

---

## Support

Nếu gặp lỗi, check:

1. **Task IDs** - Có conflict với existing tasks không?
2. **SubWorld ID** - Map có tồn tại không?
3. **Coordinates** - Vị trí có hợp lệ không?
4. **Items/NPCs** - Đã tạo chưa?
5. **Script errors** - Check server logs

---

## Credits

- **Original**: ThienDieuOnline team
- **Port**: Claude AI Assistant
- **Date**: 2025-11-15
- **Version**: 1.0 Beta
