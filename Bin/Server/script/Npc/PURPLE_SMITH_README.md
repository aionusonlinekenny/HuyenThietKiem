# NPC: Thợ Rèn Huyền Bí (Purple Smith)

## 📝 Mô Tả

NPC "Thợ Rèn Huyền Bí" cho phép người chơi hợp thành **Trang Bị TÍM (Purple Equipment)** từ trang bị xanh và Huyền Tinh.

Hệ thống này được phát triển dựa trên code của **ThienDieuOnline** (`builditem.lua`).

---

## 🎯 Chức Năng

### 1. Hợp Thành Trang Bị Tím

**Nguyên liệu cần:**
- 6-10 trang bị xanh (cùng loại: detail + particular phải giống nhau)
- 1-7 Huyền Tinh (khoáng thạch)
- 100 vạn lượng

**Kết quả:**
- Trang bị TÍM với luck >= 1,000,000,000
- Số dòng thuộc tính phụ thuộc vào Huyền Tinh

---

## 🔮 Hệ Thống Huyền Tinh

### Loại Huyền Tinh

| Tên | Detail ID | Luck | Mô tả |
|-----|-----------|------|-------|
| Lam Thủy Tinh | 16 | +2 | Cấp thấp |
| Tử Thủy Tinh | 17 | +5 | Cấp trung |
| Lục Thủy Tinh | 18 | +8 | Cấp cao |

### Công Thức Tính Số Dòng

```
1. Luck = (Tổng luck từ Huyền Tinh) - 3 (độ khó)
2. Luck giới hạn: 0 ≤ Luck ≤ 10
3. Số dòng cuối cùng = random(Luck, 10)
```

### Ví Dụ

| Huyền Tinh | Tổng Luck | Luck sau độ khó | Số dòng item |
|------------|-----------|-----------------|--------------|
| 2x Lục (18) | 16 | 13 → 10 | random(10,10) = **10 dòng** |
| 1x Lục + 1x Tử | 13 | 10 | random(10,10) = **10 dòng** |
| 2x Tử (17) | 10 | 7 | random(7,10) = **7-10 dòng** |
| 1x Lục (18) | 8 | 5 | random(5,10) = **5-10 dòng** |
| 2x Lam (16) | 4 | 1 | random(1,10) = **1-10 dòng** |
| 1x Lam | 2 | 0 | random(0,10) = **0-10 dòng** |

---

## ⚙️ Cách Sử Dụng

### Bước 1: Chuẩn Bị Nguyên Liệu

1. Thu thập **6-10 trang bị xanh cùng loại**
   - Ví dụ: 6 Nhẫn Hoàng Ngọc cấp 1
   - PHẢI cùng detail và particular!

2. Thu thập **Huyền Tinh** (càng nhiều càng tốt)
   - Lục Thủy Tinh tốt nhất (+8 luck/viên)
   - Tối đa 7 viên

3. Chuẩn bị **100 vạn lượng**

### Bước 2: Gặp NPC

1. Tìm NPC "Thợ Rèn Huyền Bí" trong game
2. Chọn "Bắt đầu hợp thành"

### Bước 3: Đặt Nguyên Liệu

**GiveBox (khung lớn):**
- Đặt 6-10 trang bị xanh vào

**Build Container (khung nhỏ - pos 12):**
- Slot 1-7: Đặt Huyền Tinh vào

### Bước 4: Xác Nhận Hợp Thành

- Hệ thống sẽ kiểm tra nguyên liệu
- Tính toán số dòng thuộc tính
- Tỷ lệ thành công: **70%**

### Kết Quả

**Thành công (70%):**
- Mất tất cả nguyên liệu
- Mất 100 vạn lượng
- Nhận được **1 trang bị TÍM** với số dòng tính theo công thức

**Thất bại (30%):**
- Mất 50% trang bị xanh
- Mất 50% Huyền Tinh
- Mất 20 vạn lượng (20%)

---

## 🔧 Cài Đặt NPC

### 1. File Script

File NPC đã được tạo tại:
```
Bin/Server/script/Npc/purple_smith.lua
```

### 2. Thêm NPC Vào Game

**Cách 1: Thêm vào dialoger map**

Tạo file mới hoặc sửa file dialoger (ví dụ: `Bin/Server/library/maps/dialoger/11.txt`):

```
<NPC_ID>	11	<X>	<Y>	\script\npc\purple_smith.lua	Thợ Rèn Huyền Bí	1
```

Thay thế:
- `<NPC_ID>`: ID NPC duy nhất (ví dụ: 300)
- `<X>`, `<Y>`: Tọa độ NPC

**Cách 2: Test bằng GM Command**

Nếu bạn là GM, có thể test NPC bằng cách:
```lua
dofile("script/npc/purple_smith.lua")
main(0)
```

### 3. Tạo Item Huyền Tinh

**Thêm vào file item definition** (ví dụ: `settings/item/questkey.txt`):

```
16	Lam Thủy Tinh	Dùng để hợp thành trang bị tím (+2 luck)
17	Tử Thủy Tinh	Dùng để hợp thành trang bị tím (+5 luck)
18	Lục Thủy Tinh	Dùng để hợp thành trang bị tím (+8 luck)
```

---

## 📊 Thống Kê

### Tỷ Lệ Thành Công

- Cơ bản: **70%**
- Thất bại: **30%** (mất 50% nguyên liệu)

### Chi Phí Trung Bình

**Thành công ngay lần 1:**
- 6-10 trang bị xanh
- 1-7 Huyền Tinh
- 100 vạn lượng

**Trung bình (tính cả thất bại):**
- ~10-15 trang bị xanh
- ~2-10 Huyền Tinh
- ~150 vạn lượng

---

## 🔍 So Sánh Với ThienDieuOnline

| Đặc điểm | ThienDieuOnline | HuyenThietKiem |
|----------|-----------------|----------------|
| **Loại item tím** | NATURE_VIOLET | item_purpleequip (luck >= 1 tỷ) |
| **Số item xanh** | 6-10 | 6-10 |
| **Nguyên liệu** | Khoáng thạch | Huyền Tinh (tương tự) |
| **Max dòng** | 10 | 10 |
| **Tỷ lệ thành công** | ~30-70% | 70% |
| **Công thức luck** | `random(nLuck, 10)` | `random(nLuck, 10)` |

---

## ⚠️ Lưu Ý Quan Trọng

1. **Trang bị xanh phải cùng loại:**
   - Detail phải giống nhau
   - Particular phải giống nhau
   - Ví dụ: Tất cả phải là "Nhẫn" (detail=3, particular=0)

2. **Trang bị TÍM không thể nâng cấp:**
   - Không thể dùng NPC "Cao Thủ Rèn Duc" để nâng cấp
   - Không thể khảm nâm qua Tremble

3. **Số dòng thuộc tính ngẫu nhiên:**
   - Phụ thuộc vào Huyền Tinh
   - Nhiều Huyền Tinh cao cấp = Nhiều dòng hơn
   - Nhưng vẫn có yếu tố random

4. **Thất bại:**
   - Mất 50% nguyên liệu
   - Có thể thử lại với nguyên liệu còn lại

---

## 🐛 Debug & Testing

File có debug messages chi tiết. Để xem:

```lua
Msg2Player("...")  -- Hiển thị debug info
```

Các debug points:
- Số lượng trang bị xanh
- Số lượng Huyền Tinh và tổng luck
- Luck sau khi trừ độ khó
- Số dòng thuộc tính cuối cùng
- Kết quả success/fail

---

## 📝 Changelog

### v1.0 (2025-12-26)
- Tạo NPC Thợ Rèn Huyền Bí
- Implement hệ thống hợp thành purple item
- Dựa trên ThienDieuOnline builditem.lua
- Hỗ trợ 3 loại Huyền Tinh
- Tỷ lệ thành công 70%

---

## 📞 Hỗ Trợ

Nếu có lỗi hoặc cần hỗ trợ:
1. Kiểm tra log server
2. Kiểm tra debug messages trong game
3. Xem code tại: `Bin/Server/script/Npc/purple_smith.lua`

---

**Chúc may mắn với việc hợp thành trang bị TÍM!** 🎉
