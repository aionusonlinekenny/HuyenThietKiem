# MapTool Usage Guide

Hướng dẫn sử dụng chi tiết Map Tool để tạo dữ liệu Trap/Object cho map.

## 📖 Quick Start

### Scenario 1: Tạo Trap Data Cho Map 21

**Bối cảnh:** Bạn muốn tạo trap data cho Map 21 (Thanh Thành Sơn) - map hiện không có trap file.

#### Bước 1: Khởi động Tool

```batch
cd SwordOnline\Sources\Tool\MapTool\bin\Release\net48
MapTool.exe
```

#### Bước 2: Load Map

1. Nhập **Map ID: 21** vào textbox "Map ID"

2. Click **"Load Map Dir"**
   - Browse đến: `HuyenThietKiem\Bin\Server\maps\`
   - Tìm thư mục map tương ứng (dựa vào MapListDef.ini)
   - Chọn thư mục

   **Hoặc**

3. Click **"Load .wor"**
   - Browse đến file .wor của map
   - Chọn file

**Kết quả:**
```
Map: Thanh Thành Sơn
Path: ...
Indoor: False
Region Bounds: (x, y) to (x, y)
Total Regions: N
```

#### Bước 3: Load Region

**Option A: Từ danh sách**
- Trong panel "Map Information", list "Regions" hiển thị các region có sẵn
- **Double-click** vào region muốn load (ví dụ: "Region (0, 0)")

**Option B: Nhập manual**
- Nhập **Region X: 0**
- Nhập **Region Y: 0**
- Click **"Load Region"**

**Kết quả:**
- Map panel hiển thị region grid (16x32 cells)
- Màu đỏ = obstacles
- Grid lines

#### Bước 4: Select Cells và Add Entries

1. **Navigate:**
   - Zoom với "Zoom +" / "Zoom -" buttons
   - Right-click drag để pan map

2. **Select cell:**
   - **Left-click** vào cell muốn chọn
   - Cell highlight màu xanh lá
   - Tọa độ hiển thị ở panel "Coordinates":
     ```
     Region X: 0
     Region Y: 0
     Region ID: 0
     Cell X: 5
     Cell Y: 10
     World X: 160
     World Y: 320
     ```

3. **Add entry:**
   - Sửa script file nếu cần: `\script\maps\trap\21\1.lua`
   - **Double-click** vào cell
   - Entry được add vào list

4. **Repeat:**
   - Click các cells khác
   - Double-click để add
   - Hoặc load thêm regions và add cells ở đó

#### Bước 5: Review Entries

Panel "Trap Entries" hiển thị:
```
Entries:
- Map 21 | Region 0 | Cell (5, 10)
- Map 21 | Region 0 | Cell (6, 10)
- Map 21 | Region 0 | Cell (7, 11)

Statistics:
Total Entries: 3
Unique Regions: 1
Unique Scripts: 1
```

**Manage entries:**
- **Remove Last**: Xóa entry cuối cùng
- **Clear All**: Xóa tất cả (confirm dialog)

#### Bước 6: Export

1. Click **"Export to File"**

2. Save dialog:
   - Navigate đến: `HuyenThietKiem\Bin\Server\library\maps\Trap\`
   - File name: **21.txt**
   - Click "Save"

3. Success message: "Exported 3 entries successfully!"

**File generated: `Bin/Server/library/maps/Trap/21.txt`**
```
MapId	RegionId	CellX	CellY	ScriptFile	IsLoad
21	0	5	10	\script\maps\trap\21\1.lua	1
21	0	6	10	\script\maps\trap\21\1.lua	1
21	0	7	11	\script\maps\trap\21\1.lua	1
```

#### Bước 7: Tạo Lua Script

Tạo file script: `Bin\Server\script\maps\trap\21\1.lua`

```lua
Include("\\script\\maps\\libtrap.lua");

function main()
    local nMapId = 21;
    local nLevel = GetLevel();
    local nState = GetFightState();

    -- Teleport player to another location
    if (nState < 1) then
        SetPos(5000, 10000);  -- World coordinates
    else
        SetPos(5100, 10100);  -- Different position when in fight
    end;

    -- Check fight state
    CheckFightState(1);

    -- Add skill buff for 3 seconds
    AddSkillTrap();
end;
```

#### Bước 8: Test In-Game

1. Copy file `21.txt` vào server
2. Copy script `21\1.lua` vào server
3. Restart server hoặc reload maps
4. Vào game, đi vào Map 21
5. Step vào cells đã mark → trap được trigger

---

### Scenario 2: Analyze Map 11 (Thành Đô)

**Bối cảnh:** Xem obstacle và trap data của Map 11 để hiểu cấu trúc.

#### Steps:

1. **Load Map 11**
   - Map ID: 11
   - Load map directory

2. **Load Multiple Regions**
   - Region (7, 20) - có trap theo file Trap/11.txt
   - Region (8, 20)
   - Region (9, 20)

3. **Observe:**
   - Màu đỏ = obstacles (walls, barriers)
   - Màu vàng = existing traps
   - Region borders màu xanh

4. **Click Around:**
   - Click vào các trap cells (màu vàng)
   - Xem tọa độ: RegionID, CellX, CellY
   - So sánh với file `Trap/11.txt` hiện có

---

### Scenario 3: Convert Tọa Độ World → Region/Cell

**Bối cảnh:** Bạn có tọa độ World (50000, 20000) từ game logs và muốn biết RegionID, CellX, CellY.

#### Method 1: Load Any Map và Click

1. Load bất kỳ map nào (không cần region data)
2. Không hiển thị map, nhưng tool vẫn tính toán được
3. **Lưu ý:** Tool hiện tại cần load ít nhất 1 region để hiển thị

#### Method 2: Use Python Tools (Đã tạo trước đó)

```bash
cd tools
python3 -c "from map_region_parser import MapCoordinateConverter; \
            c = MapCoordinateConverter(); \
            rx, ry, cx, cy = c.world_to_region_cell(50000, 20000); \
            print(f'World(50000,20000) → Region({rx},{ry}), Cell({cx},{cy})')"
```

**Output:**
```
World(50000,20000) → Region(97,19), Cell(8,16)
```

---

### Scenario 4: Tạo Object Data

**Bối cảnh:** Thay vì Trap, bạn muốn tạo Object data (chests, NPCs positions).

#### Steps:

1-5. **Giống Scenario 1** (Load map, select cells, add entries)

6. **Export as Object:**
   - Hiện tại tool export format Trap
   - Có thể modify code hoặc manually convert

**Manual Convert:**

File Trap generated:
```
MapId	RegionId	CellX	CellY	ScriptFile	IsLoad
21	0	5	10	\script\maps\trap\21\1.lua	1
```

Convert to Object format:
```
ObjID	MapID	PosX	PosY	Dir	State	ScriptFile	IsLoad
1	21	160	320	0	0	\script\maps\obj\21\1.lua	1
```

**Calculation:**
- Region(0, 0), Cell(5, 10)
- WorldX = (0 * 16 + 5) * 32 = 160
- WorldY = (0 * 32 + 10) * 32 = 320

---

## 🎨 Visual Guide

### Map Panel Colors

```
┌─────────────────────────────────┐
│  Map Renderer                   │
│                                  │
│  ⬛ Black/Dark Gray = Empty      │
│  🟥 Red = Obstacle               │
│  🟨 Yellow = Trap                │
│  🟩 Green Highlight = Selected   │
│  🟦 Blue Border = Region Edge    │
│  ⬜ White Grid = Cell Lines      │
│                                  │
└─────────────────────────────────┘
```

### Mouse Controls

| Action | Control |
|--------|---------|
| Select Cell | Left Click |
| Add Entry | Left Double-Click |
| Pan View | Right Click + Drag |
| Zoom In | Button "Zoom +" |
| Zoom Out | Button "Zoom -" |

### Coordinate Display

```
[Coordinates Panel]
┌──────────────────────────┐
│ Region X:  [0     ]      │  ← Manual input
│ Region Y:  [0     ]      │
│   [Load Region]          │
├──────────────────────────┤
│ World X:   160           │  ← Readonly (from selection)
│ World Y:   320           │
│ Region ID: 0             │
│ Cell X:    5             │
│ Cell Y:    10            │
└──────────────────────────┘

[Map Overlay]
World: (160, 320)
Region: (0, 0) [ID: 0]
Cell: (5, 10)
Offset: (0, 0)
```

---

## 💡 Tips & Tricks

### 1. Load Many Regions at Once

Để xem map rộng hơn:
```
- Load Region (0, 0)
- Load Region (1, 0)
- Load Region (0, 1)
- Load Region (1, 1)
```

Tạo view 2x2 regions = 32x64 cells

### 2. Script File Naming Convention

Đặt tên script theo số thứ tự:
```
\script\maps\trap\21\1.lua   - Trap type 1
\script\maps\trap\21\2.lua   - Trap type 2
\script\maps\trap\21\3.lua   - Trap type 3
```

Mỗi script type có behavior khác nhau (teleport, buff, damage, etc.)

### 3. Group Cells by Functionality

Khi add entries:
```
Group 1: Entrance trap (cells 0-10)
  → Script: trap/21/entrance.lua

Group 2: Exit trap (cells 11-20)
  → Script: trap/21/exit.lua

Group 3: Punishment trap (cells 21-30)
  → Script: trap/21/punishment.lua
```

Sửa script file trước khi add entries cho từng group.

### 4. Use Statistics

Panel "Statistics" giúp bạn:
- Kiểm tra số lượng entries
- Đảm bảo không miss regions
- Review unique scripts

### 5. Backup Before Export

Luôn backup file cũ trước khi export:
```
Trap/21.txt → Trap/21.txt.bak
```

### 6. Test Incrementally

Không add quá nhiều traps một lúc:
```
1. Add 5-10 traps
2. Export
3. Test in-game
4. If OK, add more
5. Repeat
```

---

## 🐛 Common Issues

### Issue 1: "No map loaded"

**Cause:** Chưa load map

**Solution:**
- Click "Load Map Dir" hoặc "Load .wor"
- Check path đúng

### Issue 2: Region list empty

**Cause:**
- Map không có region files
- Hoặc sai format

**Solution:**
- Check thư mục `v_XXX` có tồn tại không
- Check files `XXX_Region_C.dat` có trong đó
- Try toggle "Use Server Files"

### Issue 3: Map panel blank after load region

**Cause:**
- Region nằm ngoài view
- Zoom quá lớn

**Solution:**
- Click "Zoom -" vài lần
- Try right-drag pan around

### Issue 4: Can't select cells

**Cause:**
- Đang ở pan mode (right-click)

**Solution:**
- Release right mouse button
- Use left click

### Issue 5: Export file empty

**Cause:** Không có entries

**Solution:**
- Double-click cells để add entries
- Check "Trap Entries" panel có entries

---

## 🔄 Workflow Recommendations

### For New Maps (No Trap Data):

```
1. Research map in-game
   - Note down important locations
   - Mark teleport points
   - Find PvP zones

2. Load map in tool
   - Load all regions of interest
   - Visual overview

3. Mark critical cells
   - Entrance/Exit points
   - Boss spawn areas
   - Safe zones

4. Add entries systematically
   - Group by functionality
   - Use consistent script naming

5. Export và test
   - Small batches first
   - Verify in-game

6. Iterate
   - Adjust based on testing
   - Re-export
```

### For Existing Maps (Edit Trap Data):

```
1. Import existing Trap file
   - Load into tool (future feature)
   - Or manually review

2. Load map visually
   - See current trap locations

3. Add/Remove entries
   - Fill gaps
   - Remove redundant

4. Export updated file

5. Test changes
```

---

## 📚 Advanced Topics

### Custom Rendering

Edit `MapRenderer.cs` để thay đổi màu sắc:

```csharp
private Color _obstacleColor = Color.FromArgb(120, 255, 0, 0); // Red
private Color _trapColor = Color.FromArgb(120, 255, 255, 0);   // Yellow
```

### Add More Data Types

Extend tool to show NPCs, Objects:

1. Parse Element 2, 3 in `MapFileParser.cs`
2. Add fields to `RegionData`
3. Render in `MapRenderer.cs`

### Batch Processing

Create script để auto-generate traps cho nhiều maps:

```csharp
foreach (int mapId in new[] { 1, 2, 3, 11, 20, 21 })
{
    var config = MapFileParser.LoadMapConfig($"maps/{mapId}/*.wor");
    var regions = MapFileParser.GetAvailableRegions(config.MapPath);

    foreach (var (x, y) in regions)
    {
        var region = MapFileParser.LoadRegion(CoordinateConverter.GetRegionFilePath(...), x, y);
        // Auto-generate traps based on obstacles
        // ...
    }
}
```

---

**Happy Mapping! 🗺️**
