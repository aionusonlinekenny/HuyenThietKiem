# 🔮 HỆ THỐNG TRANG BỊ TÍM HOÀN CHỈNH

## 📝 Tổng Quan

Hệ thống tạo trang bị TÍM (Purple Equipment) hoàn chỉnh dựa trên **ThienDieuOnline**, bao gồm:
- ✅ NPC Thợ Rèn Huyền Bí với menu đầy đủ
- ✅ 17 loại Huyền Tinh (từ thấp đến cao)
- ✅ 5 công thức hợp thành
- ✅ Hệ thống chất lượng 11 cấp (0-10 dòng)
- ✅ Config system dễ mở rộng

---

## 📁 Cấu Trúc Files

```
HuyenThietKiem/
├── Bin/Server/script/Npc/
│   ├── purple_smith.lua              # NPC cơ bản (v1)
│   ├── purple_smith_v2.lua           # NPC nâng cao với full UI (DÙNG CÁI NÀY)
│   ├── purple_smith_config.lua       # Config hệ thống
│   ├── PURPLE_SMITH_README.md        # Hướng dẫn v1
│   └── PURPLE_SYSTEM_COMPLETE_README.md  # File này
│
└── Bin/Server/Settings/Item/
    └── HUYEN_TINH_ITEMS.txt          # Định nghĩa 17 loại Huyền Tinh
```

---

## 🔮 HỆ THỐNG HUYỀN TINH (17 LOẠI)

### 1. Lam Thủy Tinh (Blue Crystal) - Cấp Thấp

| Cấp | Genre | Detail | Particular | Luck | Mô tả |
|-----|-------|--------|------------|------|-------|
| Cap 1 | 6 | 16 | 1 | +1 | Cấp thấp nhất |
| Cap 2 | 6 | 16 | 2 | +2 | Cấp thấp |
| Cap 3 | 6 | 16 | 3 | +3 | Cấp thấp |

**Nguồn:** Drop từ quái thường

### 2. Tử Thủy Tinh (Purple Crystal) - Cấp Trung

| Cấp | Genre | Detail | Particular | Luck | Mô tả |
|-----|-------|--------|------------|------|-------|
| Cap 1 | 6 | 17 | 1 | +3 | Cấp trung |
| Cap 2 | 6 | 17 | 2 | +4 | Cấp trung |
| Cap 3 | 6 | 17 | 3 | +5 | Cấp trung |
| Cap 4 | 6 | 17 | 4 | +6 | Cấp trung cao |

**Nguồn:** Drop từ quái elite

### 3. Lục Thủy Tinh (Green Crystal) - Cấp Cao

| Cấp | Genre | Detail | Particular | Luck | Mô tả |
|-----|-------|--------|------------|------|-------|
| Cap 1 | 6 | 18 | 1 | +5 | Cấp cao |
| Cap 2 | 6 | 18 | 2 | +6 | Cấp cao |
| Cap 3 | 6 | 18 | 3 | +7 | Cấp cao |
| Cap 4 | 6 | 18 | 4 | +8 | Cấp cao |
| Cap 5 | 6 | 18 | 5 | +9 | Cấp rất cao |

**Nguồn:** Drop từ boss

### 4. Hồng Thủy Tinh (Red Crystal) - Huyền Thoại

| Cấp | Genre | Detail | Particular | Luck | Mô tả |
|-----|-------|--------|------------|------|-------|
| Cap 1 | 6 | 19 | 1 | +8 | Huyền thoại |
| Cap 2 | 6 | 19 | 2 | +9 | Huyền thoại |
| Cap 3 | 6 | 19 | 3 | +10 | Huyền thoại |
| Cap 4 | 6 | 19 | 4 | +11 | Huyền thoại |
| Cap 5 | 6 | 19 | 5 | +12 | Tối thượng |

**Nguồn:** Drop từ world boss / sự kiện

---

## ⚙️ 5 CÔNG THỨC HỢP THÀNH

### 1. Hợp Thành Cơ Bản

- **Nguyên liệu:** 6 trang bị xanh
- **Chi phí:** 50 vạn lượng
- **Độ khó:** -4 luck
- **Tỷ lệ:** 60%

### 2. Hợp Thành Tiêu Chuẩn

- **Nguyên liệu:** 7 trang bị xanh
- **Chi phí:** 80 vạn lượng
- **Độ khó:** -3 luck
- **Tỷ lệ:** 70%

### 3. Hợp Thành Nâng Cao ⭐

- **Nguyên liệu:** 8 trang bị xanh
- **Chi phí:** 100 vạn lượng
- **Độ khó:** -3 luck
- **Tỷ lệ:** 75%

### 4. Hợp Thành Chuyên Gia

- **Nguyên liệu:** 9 trang bị xanh
- **Chi phí:** 150 vạn lượng
- **Độ khó:** -2 luck
- **Tỷ lệ:** 80%

### 5. Hợp Thành Đại Cao Thủ

- **Nguyên liệu:** 10 trang bị xanh
- **Chi phí:** 200 vạn lượng
- **Độ khó:** -2 luck
- **Tỷ lệ:** 85%

---

## 🏆 HỆ THỐNG CHẤT LƯỢNG (11 CẤP)

| Số dòng | Chất lượng | Màu | Rank | Mô tả |
|---------|-----------|-----|------|-------|
| 0 | Không có thuộc tính | Gray | 0 | Rất hiếm gặp |
| 1-2 | Rất yếu / Yếu | White | 1 | Kém |
| 3-4 | Bình thường / Khá | Green | 2 | Trung bình |
| 5-6 | Tốt / Rất tốt | Blue | 3 | Khá tốt |
| 7-8 | Xuất sắc / Tuyệt vời | Purple | 4 | Tốt |
| 9 | Huyền thoại | Purple | 5 | Rất tốt |
| 10 | Thần thoại | Orange | 5 | Hoàn hảo! |

---

## 📊 VÍ DỤ TÍNH TOÁN

### Ví Dụ 1: Item 10 Dòng (Thần Thoại)

**Nguyên liệu:**
- 8 trang bị xanh (Nhẫn Hoàng Ngọc cấp 1)
- 2x Hồng Thủy Tinh Cấp 5 (+24 luck)
- 100 vạn lượng

**Tính toán:**
```
Tổng luck = 2 × 12 = 24
Luck sau độ khó = 24 - 3 = 21 → 10 (max)
Số dòng = random(10, 10) = 10 DÒNG
```

**Kết quả:** Item TÍM với **10 dòng** (Thần thoại - Orange)

### Ví Dụ 2: Item 7-10 Dòng (Xuất Sắc - Thần Thoại)

**Nguyên liệu:**
- 7 trang bị xanh
- 2x Lục Thủy Tinh Cấp 3 (+14 luck)
- 80 vạn lượng

**Tính toán:**
```
Tổng luck = 2 × 7 = 14
Luck sau độ khó = 14 - 3 = 11 → 10
Số dòng = random(10, 10) = 10 DÒNG
```

**Kết quả:** Item TÍM với **10 dòng** (Thần thoại)

### Ví Dụ 3: Item 5-10 Dòng (Tốt - Thần Thoại)

**Nguyên liệu:**
- 7 trang bị xanh
- 2x Tử Thủy Tinh Cấp 3 (+10 luck)
- 80 vạn lượng

**Tính toán:**
```
Tổng luck = 2 × 5 = 10
Luck sau độ khó = 10 - 3 = 7
Số dòng = random(7, 10) = 7, 8, 9, hoặc 10 DÒNG
```

**Kết quả:** Item TÍM với **7-10 dòng** (Xuất sắc - Thần thoại)

### Ví Dụ 4: Item 2-10 Dòng (Yếu - Thần Thoại)

**Nguyên liệu:**
- 6 trang bị xanh
- 2x Lam Thủy Tinh Cấp 3 (+6 luck)
- 50 vạn lượng

**Tính toán:**
```
Tổng luck = 2 × 3 = 6
Luck sau độ khó = 6 - 4 = 2
Số dòng = random(2, 10) = 2-10 DÒNG (may rủi lớn!)
```

**Kết quả:** Item TÍM với **2-10 dòng** (rủi ro cao!)

---

## 🎮 HƯỚNG DẪN SỬ DỤNG

### Bước 1: Cài Đặt NPC

**Thêm vào dialoger map** (ví dụ `Bin/Server/library/maps/dialoger/11.txt`):
```
300	11	99314	163834	\script\npc\purple_smith_v2.lua	Thợ Rèn Huyền Bí	1
```

### Bước 2: Spawn Huyền Tinh (GM)

```
/give 6 16 1    -- Lam Thủy Tinh Cấp 1
/give 6 17 3    -- Tử Thủy Tinh Cấp 3
/give 6 18 4    -- Lục Thủy Tinh Cấp 4
/give 6 19 5    -- Hồng Thủy Tinh Cấp 5
```

### Bước 3: Hợp Thành

1. **Gặp NPC:** Tìm "Thợ Rèn Huyền Bí"

2. **Chọn công thức:**
   - "Hợp thành trang bị TIM"
   - Chọn 1 trong 5 công thức

3. **Đặt nguyên liệu:**
   - **GiveBox (khung lớn):** 6-10 trang bị xanh (cùng loại!)
   - **Build Container (khung nhỏ):** 1-7 Huyền Tinh

4. **Xác nhận hợp thành:**
   - Tỷ lệ thành công: 60-85% (tùy công thức)
   - Thất bại: mất 50% nguyên liệu

### Bước 4: Nhận Kết Quả

**Thành công:**
- Nhận 1 trang bị TÍM
- Số dòng phụ thuộc Huyền Tinh
- Thông báo toàn server (nếu 7+ dòng)

**Thất bại:**
- Mất 50% trang bị xanh
- Mất 50% Huyền Tinh
- Mất 20% tiền

---

## 🔧 MENU NPC ĐẦY ĐỦ

### Menu Chính

1. **Hợp thành trang bị TIM**
   - Chọn công thức
   - Bắt đầu hợp thành

2. **Xem danh sách Huyền Tinh**
   - 17 loại Huyền Tinh
   - Luck của từng loại

3. **Xem công thức hợp thành**
   - 5 công thức
   - Yêu cầu và tỷ lệ

4. **Bảng xếp hạng chất lượng**
   - 11 cấp chất lượng
   - 0-10 dòng

5. **Hướng dẫn chi tiết**
   - Cách tính số dòng
   - Ví dụ cụ thể

---

## 💡 MẸO VÀ CHIẾN THUẬT

### Mẹo 1: Tối Ưu Huyền Tinh

**Mục tiêu 10 dòng (Thần thoại):**
- Cần: Tổng luck >= 13
- Tốt nhất: 2x Lục Cấp 3+ hoặc 2x Hồng Cấp 1+

**Mục tiêu 7+ dòng (Xuất sắc+):**
- Cần: Tổng luck >= 10
- Có thể: 2x Tử Cấp 3 hoặc 1x Lục Cấp 4

### Mẹo 2: Chọn Công Thức

**Nếu ít Huyền Tinh cao cấp:**
- Dùng công thức 1-2 (6-7 trang bị)
- Tiết kiệm chi phí, chấp nhận tỷ lệ thấp hơn

**Nếu nhiều Huyền Tinh cao cấp:**
- Dùng công thức 4-5 (9-10 trang bị)
- Tỷ lệ cao, luck giảm ít

### Mẹo 3: Đường Nâng Cấp Huyền Tinh

```
3x Lam Cấp 1 → 1x Lam Cấp 2 (tự làm hoặc đổi NPC)
3x Lam Cấp 2 → 1x Lam Cấp 3
3x Lam Cấp 3 → 1x Tử Cấp 1
...
```

**Lưu ý:** Có thể cần thêm NPC "Nâng Cấp Huyền Tinh" (tùy chọn)

---

## 📈 KINH TẾ GAME

### Chi Phí Trung Bình (Item 10 Dòng)

**Nguyên liệu:**
- 10-15 trang bị xanh (kể cả thất bại)
- 2-3 Huyền Tinh cao cấp
- 150-200 vạn lượng

**Thời gian farm:**
- Trang bị xanh: 2-3 giờ
- Huyền Tinh: 5-10 giờ (từ boss)
- Tổng: ~10-15 giờ cho 1 item TÍM 10 dòng

### Giá Trị Item

| Số dòng | Giá trị ước tính | Độ hiếm |
|---------|------------------|---------|
| 10 dòng | 500-1000 vạn | Rất hiếm |
| 8-9 dòng | 200-500 vạn | Hiếm |
| 6-7 dòng | 100-200 vạn | Ít |
| 4-5 dòng | 50-100 vạn | Phổ biến |
| 0-3 dòng | 10-50 vạn | Rất phổ biến |

---

## 🐛 DEBUG VÀ TROUBLESHOOTING

### Bật Debug Mode

Mở `purple_smith_v2.lua`, các dòng `Msg2Player()` sẽ hiển thị:
- Số lượng trang bị xanh
- Danh sách Huyền Tinh và luck
- Luck cuối cùng sau độ khó
- Số dòng được random
- Kết quả thành công/thất bại

### Lỗi Thường Gặp

**1. "Tất cả trang bị xanh phải cùng loại"**
- Nguyên nhân: Detail hoặc Particular khác nhau
- Giải pháp: Chỉ dùng trang bị giống hệt nhau

**2. "Không tìm thấy công thức"**
- Nguyên nhân: Chưa chọn công thức
- Giải pháp: Chọn lại từ menu "Hợp thành trang bị TIM"

**3. "Không đủ Huyền Tinh"**
- Nguyên nhân: Chưa đặt Huyền Tinh vào Build Container
- Giải pháp: Đặt vào slot 1-7 của BUILD_POS (12)

---

## 📝 CHANGELOG

### v2.0 (2025-12-26)
- ✅ Thêm 17 loại Huyền Tinh (4 tier)
- ✅ Thêm 5 công thức hợp thành
- ✅ Thêm hệ thống chất lượng 11 cấp
- ✅ Menu NPC đầy đủ với 5 options
- ✅ Config system dễ mở rộng
- ✅ Item definitions hoàn chỉnh

### v1.0 (2025-12-26)
- ✅ NPC cơ bản
- ✅ 3 loại Huyền Tinh
- ✅ 1 công thức mặc định

---

## 🔗 FILES LIÊN QUAN

1. **purple_smith_v2.lua** - NPC script chính (DÙNG CÁI NÀY)
2. **purple_smith_config.lua** - Config hệ thống
3. **HUYEN_TINH_ITEMS.txt** - Item definitions
4. **PURPLE_SMITH_README.md** - Hướng dẫn v1 (cũ)

---

## 🎯 ROADMAP (Tương Lai)

### Phase 1: UI Client (Optional)
- [ ] Tạo C++ UI giống ThienDieuOnline
- [ ] Preview số dòng trước khi hợp thành
- [ ] Animation effects

### Phase 2: Nâng Cấp Huyền Tinh
- [ ] NPC "Nhà Giả Kim"
- [ ] Hợp 3 Huyền Tinh cùng cấp → 1 cấp cao hơn
- [ ] Tỷ lệ thành công 70%

### Phase 3: Thêm Tính Năng
- [ ] Purple item có thể ghép ngọc
- [ ] Purple item có thể nâng cấp đặc biệt
- [ ] Thêm "Phù May Mắn" tăng tỷ lệ

---

## 📞 HỖ TRỢ

**Nếu có lỗi:**
1. Kiểm tra log server
2. Kiểm tra debug messages (Msg2Player)
3. Xem code: `purple_smith_v2.lua`

**Liên hệ:**
- GitHub: aionusonlinekenny/HuyenThietKiem
- Branch: claude/purple-item-npc-code-N0ECG

---

**🎉 CHÚC MAY MẮN VỚI HỆ THỐNG TRANG BỊ TÍM!**

*Hệ thống này được phát triển dựa trên nghiên cứu sâu của ThienDieuOnline builditem.lua*
