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

### 4. Quest NPCs (2/2) ✅ DONE
Đã thêm vào `/Bin/Server/library/maps/dialoger/11.txt`:
- **Tiêu Đầu**: NPC 377 tại map 11 (Dương Châu) - Tọa độ: 98432, 164864 (~3076, 5152)
- **Tiêu Sư**: NPC 377 tại map 11 (Dương Châu) - Tọa độ: 103200, 166500 (~3225, 5203)

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

---

## 📋 TESTING CHECKLIST

### Minimum Test - ✅ READY TO TEST!

1. [✅] Tạo NPC "Tiêu Đầu" với script `tieudau.lua` - DONE
2. [✅] Tạo NPC "Tiêu Sư" với script `tieusu.lua` - DONE
3. [ ] Restart server để load items và NPCs mới
4. [ ] Vào game, đến map 11 (Dương Châu)
5. [ ] Tìm NPC "Tiêu Đầu" (tọa độ: 98432, 164864 hoặc ~3076, 5152 in-game)
6. [ ] Talk to Tiêu Đầu
7. [ ] Select "Vận tiêu"
8. [ ] Select "Bắt đầu" (cần 15 vạn lượng + level 120)
9. [ ] Observe: Cart should spawn (or error if NPC template 2084 missing)
10. [ ] Đi đến NPC "Tiêu Sư" (tọa độ: 103200, 166500 hoặc ~3225, 5203 in-game)
11. [ ] Talk to Tiêu Sư -> Giao tiêu
12. [ ] Return to Tiêu Đầu -> Hoàn thành

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

## 🚀 QUICK START - ✅ READY!

**Tất cả đã setup xong! Chỉ cần restart server:**

```bash
# ✅ DONE: Items created in questkey.txt
# ✅ DONE: Scripts created in /Event/VanTieu/
# ✅ DONE: Task IDs added to TaskLib.lua
# ✅ DONE: NPCs added to map 11 dialoger

# 1. Restart server để load NPCs và items mới
systemctl restart jxserver  # hoặc lệnh restart server của bạn

# 2. Login và test event:
# - Map 11 (Dương Châu)
# - NPC "Tiêu Đầu" at ~(3076, 5152)
# - NPC "Tiêu Sư" at ~(3225, 5203)

# 3. Requirements:
# - Level ≥ 120
# - Money ≥ 150,000 lượng
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
✅ Bin/Server/library/maps/dialoger/11.txt (+2 NPCs)
```

## ⏭️ NEXT: Restart server & Test!
