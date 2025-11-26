# Map Region Cell Tools

Bộ công cụ để phân tích và chuyển đổi tọa độ Map/Region/Cell trong game.

## 📁 Các file

- **map_region_parser.py** - Module chính để chuyển đổi tọa độ
- **trap_finder.py** - Tool tìm kiếm và phân tích trap data (interactive mode)
- **list_trap_maps.py** - Liệt kê tất cả maps có trap và hướng dẫn tạo trap mới
- **scan_region_files.py** - Scan region files trong thư mục maps/ và phân tích tọa độ
- **parse_obj_npc_files.py** - Parse Object/NPC files để lấy tọa độ World và convert sang Region/Cell
- **../analyze_map.py** - Script nhanh để phân tích một map cụ thể

## 🎯 Cách sử dụng nhanh

### 1. Phân tích một map

```bash
python3 analyze_map.py <map_id>
```

**Ví dụ:**
```bash
python3 analyze_map.py 11
```

**Kết quả:**
- Tổng số trap trong map
- Danh sách các Region với số lượng trap
- Danh sách các Script file
- Ví dụ 10 trap đầu tiên với tọa độ World

### 2. Kiểm tra map có trap không

```bash
cd tools
python3 list_trap_maps.py <map_id>
```

**Ví dụ:**
```bash
python3 list_trap_maps.py 21
```

Nếu map không có trap, tool sẽ hiển thị hướng dẫn tạo trap mới.

### 3. Liệt kê tất cả maps có trap

```bash
cd tools
python3 list_trap_maps.py list
```

Kết quả: Danh sách 46 maps có trap data

### 4. Xem hướng dẫn về tọa độ

```bash
python3 analyze_map.py help
```

Hiển thị:
- Giải thích về hệ thống tọa độ
- Công thức chuyển đổi
- Ví dụ minh họa

### 5. Sử dụng trong Python

```python
import sys
sys.path.insert(0, 'tools')
from map_region_parser import MapCoordinateConverter, TrapFileParser

# Chuyển đổi tọa độ
converter = MapCoordinateConverter()

# World → Region/Cell
region_x, region_y, cell_x, cell_y = converter.world_to_region_cell(47328, 640)
print(f"World(47328, 640) → Region({region_x}, {region_y}), Cell({cell_x}, {cell_y})")

# Region/Cell → World
world_x, world_y = converter.region_cell_to_world(92, 0, 7, 20)
print(f"Region(92, 0), Cell(7, 20) → World({world_x}, {world_y})")

# Parse RegionID
region_x, region_y = converter.parse_region_id(92)
print(f"RegionID 92 → RegionX={region_x}, RegionY={region_y}")

# Đọc file trap
traps = TrapFileParser.parse_trap_file("Bin/Server/library/maps/Trap/11.txt")
print(f"Tìm thấy {len(traps)} traps")
```

### 6. Kiểm tra region files

```bash
cd tools
python3 scan_region_files.py <map_id>
```

**Chức năng:**
- Kiểm tra map có file region .dat không
- Liệt kê tất cả maps có region files (74-140)
- Gợi ý các cách thay thế nếu map không có region

**Ví dụ:**
```bash
python3 scan_region_files.py 21      # Check map 21
python3 scan_region_files.py list    # List all maps with regions
```

### 7. Phân tích tọa độ từ Object/NPC files

```bash
cd tools
python3 parse_obj_npc_files.py <map_id>
```

**Chức năng:**
- Parse file Obj/[MapID].txt hoặc Npc/[MapID].txt
- Lấy tọa độ World và convert sang Region/Cell
- Thống kê regions và phạm vi tọa độ
- Export sang format Trap

**Ví dụ:**
```bash
python3 parse_obj_npc_files.py 20           # Analyze map 20
python3 parse_obj_npc_files.py 1 export     # Export to file
```

### 8. Tool tìm kiếm interactive

```bash
cd tools
python3 trap_finder.py -i
```

**Menu chức năng:**
1. Liệt kê tất cả maps có trap
2. Phân tích một map cụ thể
3. Tìm trap theo tọa độ World (X, Y)
4. Tạo file mapping cho một Region
5. Chuyển đổi World → Region/Cell
6. Chuyển đổi Region/Cell → World

## 📐 Hệ thống tọa độ

### Region (Vùng)
- Mỗi Region là một lưới **16 × 32 cells**
- RegionID = `MAKELPARAM(RegionX, RegionY)` = `RegionX | (RegionY << 16)`
- Kích thước: 512 × 1024 pixels

### Cell (Ô)
- Mỗi Region chia thành **16 × 32 cells**
- CellX: 0-15 (ngang)
- CellY: 0-31 (dọc)
- Kích thước: 32 × 32 pixels

### World Coordinates (Tọa độ thế giới)
- Tọa độ tuyệt đối trong game
- Đơn vị: pixels

## 🔄 Công thức chuyển đổi

### World → Region/Cell

```python
RegionX = WorldX // 512
RegionY = WorldY // 1024
CellX = (WorldX % 512) // 32
CellY = (WorldY % 1024) // 32
RegionID = RegionX | (RegionY << 16)
```

### Region/Cell → World

```python
WorldX = (RegionX * 16 + CellX) * 32
WorldY = (RegionY * 32 + CellY) * 32
```

### Parse RegionID

```python
RegionX = RegionID & 0xFFFF  # LOWORD
RegionY = (RegionID >> 16) & 0xFFFF  # HIWORD
```

## 📊 Ví dụ minh họa

### Ví dụ 1: World → Region/Cell

```
Input:  World(47328, 640)
Output: Region(92, 0), Cell(7, 20)
        RegionID = 92
```

**Giải thích:**
- RegionX = 47328 / 512 = 92
- RegionY = 640 / 1024 = 0
- CellX = (47328 % 512) / 32 = 224 / 32 = 7
- CellY = (640 % 1024) / 32 = 640 / 32 = 20

### Ví dụ 2: Region/Cell → World

```
Input:  Region(92, 0), Cell(7, 20)
Output: World(47328, 640)
```

**Giải thích:**
- WorldX = (92 * 16 + 7) * 32 = 1479 * 32 = 47328
- WorldY = (0 * 32 + 20) * 32 = 20 * 32 = 640

### Ví dụ 3: Parse RegionID

```
Input:  RegionID = 92
Output: RegionX = 92, RegionY = 0

Input:  RegionID = 143
Output: RegionX = 143, RegionY = 0

Input:  RegionID = 65536  (0x10000)
Output: RegionX = 0, RegionY = 1
```

## 📂 Cấu trúc file Trap

**File:** `Bin/Server/library/maps/Trap/[MapID].txt`

**Format:**
```
MapId	RegionId	CellX	CellY	ScriptFile	IsLoad
11	92	7	20	\script\maps\trap\11\1.lua	1
11	92	8	20	\script\maps\trap\11\1.lua	1
```

**Các cột:**
1. **MapId** - ID của map
2. **RegionId** - ID của region (packed format)
3. **CellX** - Tọa độ X của cell trong region (0-15)
4. **CellY** - Tọa độ Y của cell trong region (0-31)
5. **ScriptFile** - Đường dẫn đến script Lua
6. **IsLoad** - Flag để load trap (0 hoặc 1)

## 🔍 Các trường hợp sử dụng

### 1. Tìm trap tại một vị trí cụ thể trong game

Bạn có tọa độ World (ví dụ: player đứng tại 47328, 640), muốn biết trap nào ở đó:

```python
converter = MapCoordinateConverter()
region_x, region_y, cell_x, cell_y = converter.world_to_region_cell(47328, 640)
region_id = converter.make_region_id(region_x, region_y)

# Sau đó tìm trong file Trap/[MapID].txt với RegionId, CellX, CellY
```

### 2. Tạo trap mới tại một vị trí

Bạn muốn tạo trap tại World(50000, 800):

```python
converter = MapCoordinateConverter()
region_x, region_y, cell_x, cell_y = converter.world_to_region_cell(50000, 800)
region_id = converter.make_region_id(region_x, region_y)

# Thêm dòng mới vào file Trap/[MapID].txt:
# MapId  RegionId  CellX  CellY  ScriptFile  IsLoad
# 11     <region_id>  <cell_x>  <cell_y>  \script\maps\trap\11\new.lua  1
```

### 3. Xem tất cả trap trong một Region

```python
traps = TrapFileParser.parse_trap_file("Bin/Server/library/maps/Trap/11.txt")
region_id = 92

region_traps = [t for t in traps if t['RegionId'] == region_id]
print(f"Region {region_id} có {len(region_traps)} traps")

for trap in region_traps:
    print(f"  Cell({trap['CellX']}, {trap['CellY']}) → {trap['ScriptFile']}")
```

### 4. Tính khoảng cách giữa 2 trap

```python
# Trap 1 tại World(47328, 640)
# Trap 2 tại World(47360, 672)

distance = ((47360 - 47328)**2 + (672 - 640)**2)**0.5
print(f"Khoảng cách: {distance:.2f} pixels")
```

## 🛠️ API Reference

### MapCoordinateConverter

#### `world_to_region_cell(world_x, world_y)`
Chuyển đổi tọa độ World sang Region/Cell.

**Returns:** `(region_x, region_y, cell_x, cell_y)`

#### `region_cell_to_world(region_x, region_y, cell_x, cell_y)`
Chuyển đổi Region/Cell sang tọa độ World.

**Returns:** `(world_x, world_y)`

#### `make_region_id(region_x, region_y)`
Tạo RegionID từ RegionX, RegionY.

**Returns:** `region_id` (int)

#### `parse_region_id(region_id)`
Phân tích RegionID thành RegionX, RegionY.

**Returns:** `(region_x, region_y)`

### TrapFileParser

#### `parse_trap_file(filepath)`
Đọc và phân tích file Trap mapping.

**Returns:** List of trap dictionaries với các keys:
- `MapId` - Map ID
- `RegionId` - Region ID (packed)
- `RegionX`, `RegionY` - Region coordinates
- `CellX`, `CellY` - Cell coordinates
- `ScriptFile` - Script path
- `IsLoad` - Load flag
- `WorldX`, `WorldY` - World coordinates (calculated)

#### `generate_trap_mapping(map_id, trap_data, output_file)`
Tạo file Trap mapping từ dữ liệu.

## 💡 Tips

1. **RegionY thường = 0** cho hầu hết các map, vì map thường rộng hơn cao
2. **CellX, CellY luôn trong khoảng hợp lệ** (0-15 và 0-31)
3. **World coordinates luôn là bội số của 32** nếu tính từ góc cell
4. **Một trap có thể cover nhiều cells** bằng cách tạo nhiều entries với cùng ScriptFile

## 🐛 Troubleshooting

### Lỗi: ModuleNotFoundError

**Giải pháp:**
```bash
# Thêm thư mục tools vào PYTHONPATH
export PYTHONPATH="${PYTHONPATH}:$(pwd)/tools"

# Hoặc dùng script wrapper
python3 analyze_map.py 11
```

### File trap không tồn tại

**Kiểm tra:**
```bash
ls -la Bin/Server/library/maps/Trap/
```

Đảm bảo file `[MapID].txt` tồn tại.

### Tọa độ không chính xác

**Nguyên nhân:** Có thể do offset của map hoặc special region.

**Giải pháp:** Kiểm tra file `WorldSet.ini` và `MapListDef.ini` để xem map có offset đặc biệt không.

## 📚 Tham khảo

- `SwordOnline/Sources/Core/Src/GameDataDef.h` - Định nghĩa constants
- `SwordOnline/Sources/Core/Src/KRegion.h` - Class KRegion
- `SwordOnline/Sources/Core/Src/KSubWorld.cpp` - Implementation Map2Mps, Mps2Map
- `Bin/Server/script/NpcLib/Begin_Head.lua` - Lua trap loading logic
