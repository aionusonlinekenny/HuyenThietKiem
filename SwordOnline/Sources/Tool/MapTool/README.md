# Sword Online Map Tool

Map Editor Tool with GUI for viewing maps and extracting Region/Cell coordinates.

## 📋 Tính năng

### ✅ Đã Hoàn Thành:
1. **Load Map Data**
   - Đọc file .wor (Map configuration)
   - Parse Region_C.dat và Region_S.dat files
   - Hỗ trợ cả client và server region files

2. **Visual Map Renderer**
   - Hiển thị region grid (16x32 cells)
   - Render obstacles (màu đỏ)
   - Render traps (màu vàng)
   - Region borders (màu xanh dương)
   - Zoom in/out

3. **Interactive Coordinate Selection**
   - Click chuột để chọn cell
   - Hiển thị tất cả các hệ tọa độ:
     - World (X, Y)
     - Region (X, Y, ID)
     - Cell (X, Y)
     - Offset trong cell
   - Double-click để add entry

4. **Pan & Navigate**
   - Right-click drag để di chuyển view
   - Zoom với buttons
   - Status bar hiển thị tọa độ real-time

5. **Export Functionality**
   - Export ra format Trap file (.txt)
   - Export ra format Object file
   - Statistics về entries
   - Lua script template generator

## 🚀 Cách Sử Dụng

### Build Project

```bash
cd SwordOnline/Sources/Tool/MapTool
dotnet build
```

Hoặc mở trong Visual Studio 2019+:
- File → Open → Project/Solution
- Chọn MapTool.csproj
- Build → Build Solution (Ctrl+Shift+B)

### Chạy Tool

```bash
dotnet run
```

Hoặc từ Visual Studio: F5

### Quy trình làm việc

#### 1. Load Map

**Option A: Load từ thư mục**
- Click "Load Map Dir"
- Chọn thư mục chứa file .wor (ví dụ: `Bin/Server/maps/巴陵縣/劍閣`)

**Option B: Load từ file .wor**
- Click "Load .wor"
- Chọn file .wor trực tiếp

**Kết quả:**
- Hiển thị map info (tên, bounds, số regions)
- List danh sách regions có sẵn

#### 2. Load Regions

**Option A: Từ list**
- Double-click vào region trong list
- Region sẽ được load và hiển thị trên map

**Option B: Nhập tọa độ**
- Nhập Region X, Region Y vào textbox
- Click "Load Region"

**Có thể load nhiều regions** để xem map rộng hơn.

#### 3. Navigate Map

- **Pan**: Right-click drag để di chuyển
- **Zoom**: Dùng buttons "Zoom +" và "Zoom -"
- **Status Bar**: Hiển thị tọa độ dưới con trỏ chuột

#### 4. Select Coordinates

- **Single Click**: Chọn một cell
  - Tọa độ hiển thị ở panel bên phải
  - Cell được highlight màu xanh lá
  - Hiển thị overlay trên map với tất cả tọa độ

- **Double Click**: Add entry vào list
  - Tự động lấy MapId, RegionId, CellX, CellY
  - Sử dụng script file từ textbox
  - Entry được thêm vào list

#### 5. Export Data

- Click "Export to File"
- Chọn vị trí lưu file
- Format: Tab-separated .txt file

**Format đầu ra:**
```
MapId	RegionId	CellX	CellY	ScriptFile	IsLoad
21	100	5	10	\script\maps\trap\21\1.lua	1
21	100	6	10	\script\maps\trap\21\1.lua	1
```

## 📐 Hệ Thống Tọa Độ

### Constants
```
REGION_GRID_WIDTH = 16 cells
REGION_GRID_HEIGHT = 32 cells
LOGIC_CELL_WIDTH = 32 pixels
LOGIC_CELL_HEIGHT = 32 pixels
REGION_PIXEL_WIDTH = 512 pixels (16 * 32)
REGION_PIXEL_HEIGHT = 1024 pixels (32 * 32)
```

### Conversion

**World → Region/Cell:**
```csharp
RegionX = WorldX / 512
RegionY = WorldY / 1024
CellX = (WorldX % 512) / 32
CellY = (WorldY % 1024) / 32
RegionID = RegionX | (RegionY << 16)
```

**Region/Cell → World:**
```csharp
WorldX = (RegionX * 16 + CellX) * 32
WorldY = (RegionY * 32 + CellY) * 32
```

## 🗂️ Cấu Trúc Project

```
MapTool/
├── MapData/
│   ├── DataStructures.cs      - Data models và constants
│   ├── CoordinateConverter.cs - Coordinate conversion logic
│   └── MapFileParser.cs       - Parse .wor và Region_C.dat files
├── Rendering/
│   └── MapRenderer.cs         - Map visualization engine
├── Export/
│   └── TrapExporter.cs        - Export functionality
├── MainForm.cs                - UI logic
├── MainForm.Designer.cs       - UI layout
├── Program.cs                 - Entry point
└── MapTool.csproj             - Project file
```

## 🎨 UI Layout

```
┌──────────────────────────────────────────────────────────────┐
│ Sword Online Map Tool                                        │
├────────────────────────────────┬─────────────────────────────┤
│                                │  Map Information            │
│                                │  - Map ID: [  ]             │
│                                │  - [Load Dir] [Load .wor]   │
│                                │  - [ ] Use Server Files     │
│                                │  - Map Name: ...            │
│         MAP PANEL              │  - Regions List             │
│         (800x600)              │                             │
│                                ├─────────────────────────────┤
│  - Click để select             │  Coordinates                │
│  - Double-click để add entry   │  - Region X/Y: [  ] [  ]   │
│  - Right-drag để pan           │    [Load Region]            │
│                                │  - World X/Y: ...           │
│  [Zoom +] [Zoom -]             │  - Region ID: ...           │
│                                │  - Cell X/Y: ...            │
│                                ├─────────────────────────────┤
│                                │  Trap Entries               │
│                                │  - Script: [           ]    │
│                                │  - Entries list             │
│                                │  - Statistics               │
│                                │  [Remove] [Clear] [Export]  │
└────────────────────────────────┴─────────────────────────────┘
│ Status: Ready                                                │
└──────────────────────────────────────────────────────────────┘
```

## 🎯 Use Cases

### 1. Tạo Trap Data Cho Map Mới

```
1. Load map directory
2. Load các regions cần thiết
3. Click/double-click vào các vị trí cần đặt trap
4. Sửa script file nếu cần
5. Export ra file Trap/[MapID].txt
```

### 2. Xem Obstacle Data

```
1. Load map
2. Load region
3. Màu đỏ = obstacles
4. Màu vàng = traps có sẵn
```

### 3. Convert World Coordinates → Region/Cell

```
1. Load bất kỳ map nào
2. Không cần load region
3. Nhập World X/Y vào calculator
4. Hoặc click vào map ở vị trí tương ứng
```

## 📝 File Formats

### Region_C.dat / Region_S.dat Format

```
[Header]
DWORD elemCount = 6

[File Sections Array]
KCombinFileSection[6]:
  - Offset (uint)
  - Length (uint)

[Element 0: Obstacles]
long[16][32] grid

[Element 1: Traps]
DWORD[16][32] grid

[Element 2: NPCs]
KSPNpc[] array

[Element 3: Objects]
KSPObj[] array

[Element 4: Ground]
KSPRCrunode[] array

[Element 5: Buildings]
KBuildinObj[] array
```

### .wor File Format (INI)

```ini
[MapParam]
IsInDoor=0
rect=0,0,10,10

[Light0]
; Lighting config...

[Weather]
; Weather config...
```

## 🐛 Troubleshooting

### "No .wor file found"
- Đảm bảo chọn đúng thư mục chứa file .wor
- Check đường dẫn: `Bin/Server/maps/{MapName}/`

### "Region file not found"
- Region files có format: `v_YYY/XXX_Region_C.dat`
- YYY = Region Y (3 digits)
- XXX = Region X (3 digits)
- Thử toggle "Use Server Files" nếu không tìm thấy

### "Invalid element count"
- File Region .dat bị corrupt
- Hoặc không đúng format

### Map không hiển thị
- Kiểm tra đã load region chưa
- Thử zoom out
- Pan view để tìm region

## 🔧 Development

### Requirements
- .NET Framework 4.8
- Visual Studio 2019+ hoặc .NET SDK 5.0+
- Windows OS (Windows Forms)

### Future Enhancements
- [ ] Support for NPC data display
- [ ] Object data visualization
- [ ] Ground layer rendering
- [ ] Building objects display
- [ ] Import from existing Trap files
- [ ] Batch region loading
- [ ] Mini-map overview
- [ ] Search by coordinate
- [ ] Undo/Redo for entries
- [ ] Copy/Paste coordinates

## 📚 References

**Based on:**
- `SwordOnline/Sources/Core/Src/KSubWorld.cpp` - Map loading logic
- `SwordOnline/Sources/Core/Src/KRegion.cpp` - Region data structures
- `SwordOnline/Sources/Core/Src/Scene/SceneDataDef.h` - Data definitions

**Coordinate conversion formulas from:**
- `KSubWorld::Mps2Map` - World to Map
- `KSubWorld::Map2Mps` - Map to World

## 📄 License

Part of Sword Online project.

## 👥 Author

Created for analyzing and editing Sword Online map data.

---

**Version:** 1.0.0
**Last Updated:** 2025-11-26
