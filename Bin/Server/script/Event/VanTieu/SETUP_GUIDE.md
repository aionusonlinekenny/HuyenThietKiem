# QUICK SETUP GUIDE - Vận Tiêu Event

## ✅ HOÀN THÀNH

### 1. Items (7/7) ✅ DONE
Đã thêm vào `/Bin/Server/Settings/Item/questkey.txt`:
- DetailType 68: Tiêu Kỳ
- DetailType 69: Vé Mở Khóa Vận Tiêu
- DetailType 70: Hồ Tiêu Lệnh
- DetailType 71: Tăng Tốc
- DetailType 72: Hồi Máu
- DetailType 73: Dịch Chuyển
- DetailType 74: Rương Vận Tiêu

### 2. Scripts (9/9) ✅ DONE
Tất cả scripts đã sẵn sàng tại `/Bin/Server/script/Event/VanTieu/`

### 3. Task IDs (3/3) ✅ DONE
Đã thêm vào TaskLib.lua:
- TASK_VANTIEU = 750
- TASK_NPCVANTIEU = 751
- TASK_RESET_VANTIEU = 752

---

## ⚠️ CÒN PHẢI LÀM

### 1. NPC Templates (0/5) - OPTIONAL

**Cách 1: Dùng existing NPCs (RECOMMENDED cho test)**
Thay vì tạo NPC templates mới, dùng NPCs có sẵn:
- Cart NPCs: Dùng bất kỳ NPC nào (ví dụ: animal NPCs)
- Chest: Dùng object NPC có sẵn
- Chỉ cần link script là được

**Cách 2: Tạo NPC templates mới (CHO PRODUCTION)**
File: `/Bin/Server/Settings/Npcs.txt`
- Format cực kỳ phức tạp với 100+ fields
- Khuyến nghị: Dùng in-game GM tools để tạo

### 2. Quest NPCs (0/2) - REQUIRED

**Tiêu Đầu (Quest Giver)**
- Vị trí đề xuất: Thành Đô hoặc major city
- Script: `\script\Event\VanTieu\tieudau.lua`
- Tạo bằng GM command hoặc map editor

**Tiêu Sư (Quest Receiver)**
- Vị trí: Thanh Thành Sơn (243, 219) - hoặc điều chỉnh trong lib.lua
- Script: `\script\Event\VanTieu\tieusu.lua`
- Tạo bằng GM command hoặc map editor

---

## 🎮 CÁCH TẠO NPCs IN-GAME

### Option A: GM Commands (nếu có)
```
/addnpc <template_id> <name> <script_path>
```

### Option B: Map Editor
1. Mở map editor tool
2. Place NPC tại vị trí mong muốn
3. Set script path
4. Save map

### Option C: Database (nếu NPCs lưu trong DB)
Thêm vào NPC table:
```sql
INSERT INTO npcs (name, template_id, map_id, pos_x, pos_y, script_path, ...)
VALUES ('Tiêu Đầu', ..., 'MAPID', 100, 200, '\script\Event\VanTieu\tieudau.lua', ...);
```

---

## 📋 TESTING CHECKLIST

### Minimum Test (Chỉ cần 2 NPCs)

1. [ ] Tạo NPC "Tiêu Đầu" với script `tieudau.lua`
2. [ ] Tạo NPC "Tiêu Sư" với script `tieusu.lua`
3. [ ] Restart server để load items mới
4. [ ] Talk to Tiêu Đầu
5. [ ] Select "Vận tiêu"
6. [ ] Select "Bắt đầu" (cần 15 vạn lượng + level 120)
7. [ ] Observe: Cart should spawn (or error if NPC template missing)
8. [ ] Go to Tiêu Sư location
9. [ ] Talk to Tiêu Sư -> Giao tiêu
10. [ ] Return to Tiêu Đầu -> Hoàn thành

### Expected Behaviors

**✅ Nếu thành công:**
- Cart spawns near player
- Can complete quest
- Get rewards (exp, items)

**⚠️ Nếu cart không spawn:**
- Normal! NPC template chưa có
- Quest vẫn có thể complete (skip cart mechanics)
- Hoặc dùng existing NPC template ID thay vì 2084-2086

---

## 🔧 WORKAROUNDS

### Nếu cart không spawn:

**Solution 1**: Dùng existing NPC template
```lua
-- File: lib.lua
-- Thay đổi:
NPC_DONG_TIEUXA = 2084  -- Thay bằng ID có sẵn, ví dụ: 100
NPC_BAC_TIEUXA = 2085   -- 101
NPC_VANG_TIEUXA = 2086  -- 102
```

**Solution 2**: Skip cart mechanics
- Comment out cart spawn code
- Làm quest đơn giản hơn: Talk to NPC A -> Talk to NPC B -> Done

### Nếu items không xuất hiện:

1. Check file encoding (phải là UTF-8 hoặc ANSI phù hợp)
2. Restart server
3. Clear cache nếu có

---

## 🚀 QUICK START (1 phút)

**Minimum viable test:**

```bash
# 1. Đã có: Items & Scripts ✅

# 2. Place 2 NPCs in-game:
# - Tiêu Đầu: Any city, script=tieudau.lua
# - Tiêu Sư: Any location, script=tieusu.lua

# 3. Update coordinates nếu cần:
# Edit: /Bin/Server/script/Event/VanTieu/lib.lua
# - SUBWORLD_START
# - POS_START_X/Y
# - POS_END_X/Y

# 4. Restart server

# 5. Test!
```

---

## 📞 TROUBLESHOOTING

### Quest không xuất hiện
- Check NPC script path đúng chưa
- Check TaskLib.lua đã include trong global không

### Cart không spawn
- Bình thường! Dùng existing NPC ID
- Hoặc tạo NPC templates

### Rewards không nhận được
- Check AddRespect -> AddRepute mapping
- Check item IDs (68-74 in genre 6)

---

## 📝 FILES MODIFIED

```
✅ Bin/Server/Settings/Item/questkey.txt (+7 items)
✅ Bin/Server/script/lib/TaskLib.lua (+3 task IDs)
✅ Bin/Server/script/Event/VanTieu/*.lua (9 files)
```

## ⏭️ NEXT: Place NPCs & Test!
