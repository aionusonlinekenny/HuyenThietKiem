# Phân tích Map 21 - Thanh Thành Sơn

## 📋 Tóm tắt

**Map 21 (Thanh Thành Sơn) KHÔNG có dữ liệu tọa độ region/cell trong thư mục server.**

## 🔍 Kết quả kiểm tra

### ❌ Không tồn tại:
- **Region files:** `Bin/Server/maps/**/*021_region_*.dat` - KHÔNG CÓ
- **Trap file:** `Bin/Server/library/maps/Trap/21.txt` - KHÔNG CÓ
- **Object file:** `Bin/Server/library/maps/Obj/21.txt` - KHÔNG CÓ
- **NPC file:** `Bin/Server/library/maps/Npc/21.txt` - KHÔNG CÓ

### ✅ Có tồn tại:
- **MapListDef.ini:** Map 21 được định nghĩa
  - Tên: Thanh Thành Sơn
  - Loại: Field
  - MapPos: 85,381
  - Level: 45 (min/max)

## 📊 So sánh với các maps khác

### Maps có region files (.dat):
- **Chỉ có maps 74-140** (67 maps) có file region trong `Bin/Server/maps/`
- Map 21 nằm ngoài khoảng này → **KHÔNG có region files**

### Maps có trap data:
- **46 maps** có trap files: 1, 2, 3, 4, 6, 11, 20, 37, 38, ...
- Map 21 **KHÔNG** nằm trong danh sách

### Maps có object data:
- **18 maps** có object files: 1, 11, 20, 37, 53, 78, 80, 99, ...
- Map 21 **KHÔNG** nằm trong danh sách

### Map gần nhất có data:
- **Map 20 (Giang Tân Thôn):**
  - ✅ Có Object file với 12 objects
  - ✅ Có Trap file với trap data
  - Tọa độ World: X:107,805→119,319, Y:192,949→203,077
  - Regions: X:210-233, Y:188-198

## 💡 Giải thích

### Tại sao Map 21 không có dữ liệu?

**1. Hệ thống Region files:**
- Region files (.dat) chỉ tồn tại cho maps lớn/chính (74-140)
- Map 21 là map nhỏ/phụ → không cần region files riêng
- Có thể sử dụng data từ client (*.map files)

**2. Map cũ:**
- Map 21 có thể là map từ phiên bản cũ
- Sử dụng hệ thống khác (không phải region-based)
- Data được load trực tiếp từ client

**3. Map động:**
- Không có objects/NPCs cố định
- Spawn động trong scripts
- Hoặc map đặc biệt (event map, instance map)

## 🛠️ Các cách lấy tọa độ cho Map 21

### Phương pháp 1: Từ Client Map Files ⭐ (Khuyến nghị)
Client thường có file `.map` hoặc `.smap` chứa toàn bộ dữ liệu map:

```
Client/maps/清城山.map
Client/smap/021.smap
```

**Cần:**
- Tool để parse file .map/.smap (binary format)
- Hoặc dùng map editor từ client

**Ưu điểm:**
- Có đầy đủ thông tin (collision, obstacles, regions)
- Chính xác 100%

**Nhược điểm:**
- Cần access client files
- Cần tool để parse binary format

### Phương pháp 2: Từ Scripts
Kiểm tra trong scripts có tọa độ teleport/spawn không:

```bash
grep -r "SetPos.*21\|mapid.*21" Bin/Server/script/
```

Tìm các function như:
- `SetPos(x, y)` - tọa độ teleport
- `AddNpc()` - tọa độ spawn NPC
- `CreateTrap()` - tọa độ trap

**Ví dụ:**
```lua
-- Trong script nào đó
if mapid == 21 then
    SetPos(12345, 67890)  -- Tọa độ World
end
```

### Phương pháp 3: Từ Database (nếu có)
Kiểm tra các bảng:
- `maps` - thông tin map
- `regions` - region data
- `map_cells` - cell data
- `teleport_points` - điểm teleport

### Phương pháp 4: Tạo dữ liệu mới
Nếu không tìm thấy data nào, có thể tự tạo:

**Bước 1:** Xác định kích thước map
- Từ MapListDef.ini hoặc client data
- Hoặc ước lượng từ level range (45)

**Bước 2:** Chia regions
```
Giả sử map 21 có kích thước 5000×5000 pixels:
- Số regions ngang: 5000 ÷ 512 ≈ 10 regions
- Số regions dọc: 5000 ÷ 1024 ≈ 5 regions
- Tổng: ~50 regions
```

**Bước 3:** Tạo file trap/obj nếu cần
```
Bin/Server/library/maps/Trap/21.txt
Bin/Server/library/maps/Obj/21.txt
```

### Phương pháp 5: Từ In-Game Logging
Nếu có quyền access server runtime:

**1. Thêm logging vào code:**
```cpp
// Trong KPlayer::DoTrap() hoặc KPlayer::OnEnterRegion()
if (SubWorldID == 21) {
    printf("Map 21 - RegionID: %d, Cell: (%d, %d), World: (%d, %d)\n",
           region_id, cell_x, cell_y, world_x, world_y);
}
```

**2. Chơi qua map 21 và ghi log:**
- Di chuyển khắp map
- Log sẽ ghi lại tất cả tọa độ
- Aggregate data để tạo map hoàn chỉnh

## 📝 Kết luận

**Cho Map 21:**
1. ❌ **KHÔNG có** region files trong `Bin/Server/maps/`
2. ❌ **KHÔNG có** trap/obj/npc files
3. ✅ **CÓ** định nghĩa trong `MapListDef.ini`
4. 💡 **Nên** lấy data từ client map files hoặc scripts

**Khuyến nghị:**
- Nếu cần tọa độ để tạo trap/obj → Dùng **Phương pháp 1** (Client files)
- Nếu chỉ cần một vài điểm cụ thể → Dùng **Phương pháp 2** (Scripts)
- Nếu cần toàn bộ map → Dùng **Phương pháp 5** (Runtime logging)

## 🔧 Tools đã tạo

### Kiểm tra region files:
```bash
cd tools
python3 scan_region_files.py 21
```

### Kiểm tra trap:
```bash
cd tools
python3 list_trap_maps.py 21
```

### Phân tích Object/NPC (cho maps khác):
```bash
cd tools
python3 parse_obj_npc_files.py 20  # Map 20 có objects
```

## 📚 Tham khảo

**So sánh với Map 20 (có đầy đủ data):**
- Map 20: ✅ Trap (có), ✅ Object (12 objects), World X:107,805→119,319
- Map 21: ❌ Trap (không), ❌ Object (không), ❓ Tọa độ (chưa rõ)

**Công thức chuyển đổi (khi có World coordinates):**
```python
from tools.map_region_parser import MapCoordinateConverter

converter = MapCoordinateConverter()
region_x, region_y, cell_x, cell_y = converter.world_to_region_cell(world_x, world_y)
region_id = converter.make_region_id(region_x, region_y)
```

---

**Ngày phân tích:** 2025-11-26
**Branch:** claude/map-cell-script-data-01CfedzqEM8vHBeTT4eZM1Pw
