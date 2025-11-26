# Map Tool - Tóm tắt và Hướng dẫn Sử dụng

## 🎯 Mục tiêu

Tạo tool để:
1. Load map data từ game
2. Preview map visually
3. Click chọn cells để lấy tọa độ
4. Export ra file Trap/Object format

---

## ✅ Đã hoàn thành

Tôi đã tạo **3 công cụ** cho bạn:

### 1. Python Tools 🐍
**Location:** `tools/`

**Files:**
- `map_region_parser.py` - Coordinate conversion library
- `analyze_map.py` - Quick map analysis
- `parse_obj_npc_files.py` - Parse Object/NPC files
- `scan_region_files.py` - Scan region files
- `list_trap_maps.py` - List all maps with traps
- `trap_finder.py` - Interactive trap finder

**Sử dụng:**
```bash
# Analyze map
python3 analyze_map.py 11

# Check map có trap không
cd tools
python3 list_trap_maps.py 21

# Parse object data
python3 parse_obj_npc_files.py 20

# Interactive menu
python3 trap_finder.py -i
```

**Ưu điểm:**
- ✅ Hoàn chỉnh, nhiều features
- ✅ Command-line, automation friendly
- ✅ Đã test với maps 11, 20, 21

**Nhược điểm:**
- ❌ Không có UI visual
- ❌ Không render map

---

### 2. C# Windows Forms MapTool 🖥️
**Location:** `SwordOnline/Sources/Tool/MapTool/`

**Features:**
- ✅ Visual map renderer với grid
- ✅ Load .wor và Region_C.dat files
- ✅ Interactive click để select cells
- ✅ Zoom, pan navigation
- ✅ Real-time coordinate display
- ✅ Export to Trap file format
- ✅ Obstacle và trap visualization
- ✅ Double-click để add entries

**Build:**
```batch
cd SwordOnline\Sources\Tool\MapTool
dotnet build
```

**Run:**
```batch
dotnet run
```

Hoặc mở trong Visual Studio:
- File → Open → Project
- Chọn `MapTool.csproj`
- F5 để run

**UI Layout:**
```
┌─────────────────────────────────┬──────────────────────┐
│                                 │  Map Information     │
│                                 │  - Load Map/Region   │
│         MAP PANEL               │  - Regions List      │
│         (800x600)               ├──────────────────────┤
│                                 │  Coordinates         │
│  - Màu đỏ: Obstacles            │  - World X/Y         │
│  - Màu vàng: Traps              │  - Region X/Y/ID     │
│  - Grid: Cells                  │  - Cell X/Y          │
│  - Left click: Select           ├──────────────────────┤
│  - Double click: Add entry      │  Trap Entries        │
│  - Right drag: Pan              │  - Script File       │
│                                 │  - Entries List      │
│  [Zoom +] [Zoom -]              │  [Remove] [Export]   │
└─────────────────────────────────┴──────────────────────┘
```

**Workflow:**
1. Click "Load .wor" → chọn map directory
2. Double-click region trong list → load region
3. Left-click cells → see coordinates
4. Double-click cells → add to trap list
5. Click "Export to File" → save as .txt

**Documentation:**
- `README.md` - Features overview
- `USAGE_GUIDE.md` - Step-by-step tutorial

**Ưu điểm:**
- ✅ UI đẹp, trực quan
- ✅ Visual map preview
- ✅ Interactive, dễ dùng
- ✅ Đầy đủ features nhất

**Nhược điểm:**
- ❌ Cần .NET Framework 4.8
- ❌ Không tích hợp trực tiếp với C++ Core

---

### 3. C++ Integration Guide 📚
**Location:** `SwordOnline/Sources/Tool/MapToolCpp/`

**Files:**
- `HOW_TO_INTEGRATE_WITH_CORE.md` - Chi tiết hướng dẫn integrate

**Nội dung:**
- Cách link với Core library
- Sample code sử dụng KSubWorld, KRegion classes
- Win32 UI example với GDI rendering
- Build setup instructions

**Ưu điểm:**
- ✅ Native C++ code
- ✅ Tích hợp trực tiếp với game engine
- ✅ Reuse KSubWorld::LoadMap, Map2Mps, etc.

**Nhược điểm:**
- ❌ Phức tạp, cần thời gian setup
- ❌ Requires understanding game engine
- ❌ Cần build Core library trước

---

## 🎯 Khuyến nghị sử dụng

### Nếu bạn muốn tool hoàn chỉnh ngay:
→ **Dùng C# MapTool**
```
SwordOnline/Sources/Tool/MapTool/
```
- Build và run ngay được
- UI đẹp, đầy đủ tính năng
- Perfect cho việc tạo trap data

### Nếu bạn muốn command-line/scripting:
→ **Dùng Python Tools**
```
tools/analyze_map.py
tools/parse_obj_npc_files.py
```
- Nhanh, tiện lợi
- Automation friendly
- Batch processing

### Nếu bạn muốn integrate sâu với game engine:
→ **Follow C++ Integration Guide**
```
SwordOnline/Sources/Tool/MapToolCpp/HOW_TO_INTEGRATE_WITH_CORE.md
```
- Requires C++ knowledge
- Cần thời gian develop
- Powerful nhất về lâu dài

---

## 📝 Ví dụ Workflow

### Scenario: Tạo Trap cho Map 21

**Using C# MapTool:**

1. **Build tool:**
   ```batch
   cd SwordOnline\Sources\Tool\MapTool
   dotnet build
   dotnet run
   ```

2. **Load map:**
   - Nhập Map ID: `21`
   - Click "Load .wor" → chọn map directory
   - Map info hiển thị

3. **Load regions:**
   - Double-click "Region (0, 0)" trong list
   - Map render ra grid 16x32 cells

4. **Select cells:**
   - Click cells muốn đặt trap
   - Double-click để add vào list
   - Coordinates tự động lấy

5. **Export:**
   - Click "Export to File"
   - Save as `Bin/Server/library/maps/Trap/21.txt`

6. **Result:**
   ```
   MapId	RegionId	CellX	CellY	ScriptFile	IsLoad
   21	0	5	10	\script\maps\trap\21\1.lua	1
   21	0	6	10	\script\maps\trap\21\1.lua	1
   ```

**Using Python Tools:**

```bash
# Check if map 21 has existing data
cd tools
python3 list_trap_maps.py 21

# If no data, need to create manually
# Can use coordinate converter:
python3 -c "from map_region_parser import MapCoordinateConverter; \
            c = MapCoordinateConverter(); \
            rx,ry,cx,cy = c.world_to_region_cell(5000, 10000); \
            print(f'Region({rx},{ry}), Cell({cx},{cy})')"
```

---

## 📊 So sánh Tools

| Feature | Python Tools | C# MapTool | C++ Integrated |
|---------|-------------|------------|----------------|
| **Visual Map** | ❌ | ✅ | ✅ (nếu implement) |
| **Coordinate Conversion** | ✅ | ✅ | ✅ |
| **Export Trap File** | ✅ | ✅ | ✅ |
| **Interactive Click** | ❌ | ✅ | ✅ (nếu implement) |
| **Batch Processing** | ✅ | ❌ | ✅ (có thể) |
| **Easy to Use** | 🟡 | ✅ | ❌ |
| **Native Performance** | ❌ | ❌ | ✅ |
| **Engine Integration** | ❌ | ❌ | ✅ |
| **Setup Time** | 0 min | 5 min | 30+ min |

---

## 🔄 Migration Path

Nếu bạn muốn dần chuyển từ C# → C++:

**Phase 1: Use C# Tool (Ngay bây giờ)**
- Familiar với workflow
- Tạo trap data cần thiết
- Hiểu coordinate system

**Phase 2: Understand Core Library**
- Đọc KSubWorld.cpp, KRegion.cpp
- Hiểu map loading mechanism
- Study coordinate conversions

**Phase 3: Create C++ Tool**
- Follow integration guide
- Start với simple console app
- Gradually add UI features

**Phase 4: Full Integration**
- Link với Core library
- Reuse all engine code
- Extend game functionality

---

## 📚 References

**Python Tools:**
- `tools/README.md` - Full documentation
- `MAP_21_ANALYSIS.md` - Map 21 case study

**C# MapTool:**
- `SwordOnline/Sources/Tool/MapTool/README.md` - Features
- `SwordOnline/Sources/Tool/MapTool/USAGE_GUIDE.md` - Tutorial

**C++ Integration:**
- `SwordOnline/Sources/Tool/MapToolCpp/HOW_TO_INTEGRATE_WITH_CORE.md`
- `SwordOnline/Sources/Tool/TOOL_COMPARISON.md`

**Core Code:**
- `SwordOnline/Sources/Core/Src/KSubWorld.cpp` - Map loading
- `SwordOnline/Sources/Core/Src/KRegion.cpp` - Region data
- `SwordOnline/Sources/Core/Src/Scene/SceneDataDef.h` - Data structures

---

## 🚀 Next Steps

1. **Immediate Use:**
   - Dùng C# MapTool để tạo trap data
   - Tool đã sẵn sàng, chỉ cần build và run

2. **Learn & Explore:**
   - Study Python tools code
   - Understand coordinate system
   - Read C++ integration guide

3. **Advanced:**
   - Implement C++ version nếu cần
   - Extend với features riêng
   - Integrate với game engine

---

**Bạn đã có đầy đủ công cụ để làm việc với map data!** 🎉

Chọn tool phù hợp với nhu cầu và bắt đầu ngay.

---

**Branch:** `claude/map-cell-script-data-01CfedzqEM8vHBeTT4eZM1Pw`
**Created:** 2025-11-26
