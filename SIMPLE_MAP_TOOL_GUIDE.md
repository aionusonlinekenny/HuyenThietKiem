# Simple Map Tool - Hướng Dẫn Đơn Giản

## 🎯 Tool Mới Sẽ Đơn Giản Như Thế Nào?

### ❌ Tool Cũ (Phức tạp):
```
1. User phải biết map folder path
2. User phải manually load .wor file
3. User phải nhập RegionX, RegionY manually
4. User phải click "Load Region" cho từng region
5. Dễ nhầm lẫn, nhiều bước
```

### ✅ Tool Mới (Đơn giản):
```
1. Chọn game folder: D:\HuyenThietKiem\Bin\Server
2. Nhập Map ID: 11
3. Click "Load Map"
4. XONG! Tool tự động load TẤT CẢ
```

---

## 🚀 UI Tool Mới

```
┌──────────────────────────────────────────────────────┐
│ Simple Map Coordinate Tool                           │
├──────────────────────────────────────────────────────┤
│                                                       │
│ 📁 Game Folder:                                      │
│    [D:\HuyenThietKiem\Bin\Server___________] [📁]    │
│    ○ Server    ○ Client                              │
│                                                       │
│ 🗺️ Map ID:                                           │
│    [11__] [Load Map]                                 │
│                                                       │
├──────────────────────────────────────────────────────┤
│ ℹ️ Map Info:                                          │
│    Name: Thành Đô (成都)                             │
│    Folder: 场景地图\城市\成都                        │
│    Region Grid: 4x4 (16 regions total)               │
│    Map Size: 2048 x 4096 pixels                      │
│                                                       │
├──────────────────────────────────────────────────────┤
│ 🖼️ Map Viewer                                         │
│ ┌──────────────────────────────────────────────────┐ │
│ │░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│ │
│ │░░░░░░░░░██████░░░░░░░░░░░░░░░░░░░░░██████░░░░░░│ │
│ │░░░░░░░░░██████░░░░░░░░░░░░░░░░░░░░░██████░░░░░░│ │
│ │░░░██████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██████│ │
│ │░░░██████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██████│ │
│ │░░░░░░░░░░░░░░░🟢░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│ │ ← 🟢 = Selected cell
│ │░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│ │ ← ░ = Walkable
│ │░░░░░░░░░██████░░░░░░░░░░░░░░░░░░░░░██████░░░░░░│ │ ← █ = Obstacle
│ │░░░░░░░░░██████░░░░░░░░░░░░░░░░░░░░░██████░░░░░░│ │
│ └──────────────────────────────────────────────────┘ │
│ [Zoom In] [Zoom Out] [Reset View]                    │
│                                                       │
├──────────────────────────────────────────────────────┤
│ 📍 Selected Coordinates:                              │
│    World:    (5000, 10000)                           │
│    Region:   (9, 9)      [RegionID: 589833]          │
│    Cell:     (12, 24)                                │
│    Offset:   (8, 16)                                 │
│                                                       │
│ 📝 Trap Entry:                                        │
│    11	589833	12	24	\script\maps\trap\11\1.lua	1 │
│                                                       │
│ [Copy Coordinates] [Add to List] [Export to File]    │
│                                                       │
└──────────────────────────────────────────────────────┘
```

---

## 🔄 Workflow Tự Động

### Bước 1: User Input
```
Game Folder: D:\HuyenThietKiem\Bin\Server
Map ID: 11
```

### Bước 2: Tool Tự Động Làm

#### 2.1. Đọc MapList.ini
```csharp
string mapListPath = Path.Combine(gameFolder, "Settings", "MapList.ini");
string mapFolder = ReadIniValue(mapListPath, "List", "11");
// Result: mapFolder = "场景地图\城市\成都"

string mapName = ReadIniValue(mapListPath, "List", "11_name");
// Result: mapName = "Thành Đô"
```

#### 2.2. Load .wor File
```csharp
string worPath = Path.Combine(gameFolder, "maps", mapFolder, "成都.wor");
RECT rect = ReadIniRect(worPath, "MAIN", "rect");
// Result: rect = {left=0, top=0, right=3, bottom=3}

int regionWidth = rect.right - rect.left + 1;   // 4
int regionHeight = rect.bottom - rect.top + 1;  // 4
int totalRegions = regionWidth * regionHeight;  // 16
```

#### 2.3. Auto Load TẤT CẢ Regions
```csharp
for (int y = rect.top; y <= rect.bottom; y++)
{
    for (int x = rect.left; x <= rect.right; x++)
    {
        string regionPath = Path.Combine(
            gameFolder, "maps", mapFolder,
            $"v_{y:D3}",
            $"{x:D3}_Region_S.dat"  // hoặc _Region_C.dat nếu là client
        );

        if (File.Exists(regionPath))
        {
            LoadRegionData(regionPath, x, y);
            progressBar.Value = (y * regionWidth + x + 1) * 100 / totalRegions;
        }
    }
}
```

#### 2.4. Parse Region Files
```csharp
void LoadRegionData(string path, int regionX, int regionY)
{
    using (BinaryReader reader = new BinaryReader(File.OpenRead(path)))
    {
        // Read combined file header
        uint numSections = reader.ReadUInt32();
        var sections = new Section[numSections];

        for (int i = 0; i < numSections; i++)
        {
            sections[i].offset = reader.ReadUInt32();
            sections[i].length = reader.ReadUInt32();
        }

        int headerSize = 4 + numSections * 8;

        // Load Obstacle grid (16x32 cells)
        reader.BaseStream.Seek(headerSize + sections[0].offset, SeekOrigin.Begin);
        byte[,] obstacles = new byte[16, 32];
        for (int cy = 0; cy < 32; cy++)
            for (int cx = 0; cx < 16; cx++)
                obstacles[cx, cy] = reader.ReadByte();

        // Load Trap grid (16x32 cells) if exists
        if (numSections > 1 && sections[1].length > 0)
        {
            reader.BaseStream.Seek(headerSize + sections[1].offset, SeekOrigin.Begin);
            byte[,] traps = new byte[16, 32];
            for (int cy = 0; cy < 32; cy++)
                for (int cx = 0; cx < 16; cx++)
                    traps[cx, cy] = reader.ReadByte();
        }

        // Store in map data
        mapData[regionX, regionY] = new RegionData
        {
            RegionX = regionX,
            RegionY = regionY,
            Obstacles = obstacles,
            Traps = traps
        };
    }
}
```

#### 2.5. Render Map
```csharp
void RenderMap(Graphics g)
{
    for (int ry = 0; ry < regionHeight; ry++)
    {
        for (int rx = 0; rx < regionWidth; rx++)
        {
            var region = mapData[rx, ry];
            if (region == null) continue;

            for (int cy = 0; cy < 32; cy++)
            {
                for (int cx = 0; cx < 16; cx++)
                {
                    int screenX = (rx * 16 + cx) * cellSize;
                    int screenY = (ry * 32 + cy) * cellSize;

                    // Draw cell
                    Color color = Color.LightGreen;  // Walkable
                    if (region.Obstacles[cx, cy] != 0)
                        color = Color.Red;  // Obstacle
                    else if (region.Traps != null && region.Traps[cx, cy] != 0)
                        color = Color.Yellow;  // Trap

                    g.FillRectangle(new SolidBrush(color),
                        screenX, screenY, cellSize, cellSize);

                    // Draw grid
                    g.DrawRectangle(Pens.Gray,
                        screenX, screenY, cellSize, cellSize);
                }
            }
        }
    }
}
```

#### 2.6. Handle Click
```csharp
void MapPanel_Click(object sender, MouseEventArgs e)
{
    // Screen to World conversion
    int worldX = (e.X / cellSize) * 32;
    int worldY = (e.Y / cellSize) * 32;

    // World to Region/Cell
    int regionX = worldX / 512;
    int regionY = worldY / 1024;
    int cellX = (worldX % 512) / 32;
    int cellY = (worldY % 1024) / 32;
    int regionID = regionX | (regionY << 16);

    // Display
    txtWorldX.Text = worldX.ToString();
    txtWorldY.Text = worldY.ToString();
    txtRegionX.Text = regionX.ToString();
    txtRegionY.Text = regionY.ToString();
    txtRegionID.Text = regionID.ToString();
    txtCellX.Text = cellX.ToString();
    txtCellY.Text = cellY.ToString();

    // Generate trap entry
    string trapEntry = $"{mapId}\t{regionID}\t{cellX}\t{cellY}\t\\script\\maps\\trap\\{mapId}\\1.lua\t1";
    txtTrapEntry.Text = trapEntry;
}
```

---

## 📊 So Sánh Tool Cũ vs Tool Mới

| Feature | Tool Cũ | Tool Mới |
|---------|---------|----------|
| **Bước để load map** | 5-10 bước | 3 bước |
| **User phải biết** | File paths, region coords | Chỉ game folder + map ID |
| **Load regions** | Manual từng region | Auto load tất cả |
| **MapList.ini** | Không dùng | Tự động đọc |
| **Error handling** | Dễ sai path | Auto validate |
| **Render speed** | Manual refresh | Auto render |
| **Export** | Manual copy/paste | One-click export |

---

## 🎮 Example Usage

### Scenario 1: Tạo Trap cho Map 11 (Thành Đô)

**Old Tool:**
```
1. Mở tool
2. Click "Browse .wor" → tìm file Bin\Server\maps\场景地图\城市\成都\成都.wor
3. Click "Load"
4. Nhập RegionX: 0, RegionY: 0
5. Click "Load Region"
6. Chờ load...
7. Click trên map
8. Copy coordinates
9. Lặp lại cho từng region khác
```

**New Tool:**
```
1. Mở tool
2. Browse game folder: D:\HuyenThietKiem\Bin\Server
3. Nhập Map ID: 11
4. Click "Load Map"
5. XONG! Click anywhere → get coordinates
```

### Scenario 2: Export Trap List

**Old Tool:**
```
1. Click cell → manual copy
2. Paste vào notepad
3. Format lại
4. Lặp lại cho từng cell
5. Save file
```

**New Tool:**
```
1. Click cells muốn add trap
2. Mỗi click → auto add to list
3. Click "Export to File"
4. Chọn save location
5. XONG! File ready to use
```

---

## 🔧 Implementation Checklist

### Phase 1: Basic Auto-Load ✅
- [x] Browse game folder
- [x] Read MapList.ini
- [x] Load .wor file
- [x] Auto-detect all regions
- [x] Load all region files
- [x] Display map info

### Phase 2: Visual Rendering 🟡
- [ ] Render region grid
- [ ] Render cell grid
- [ ] Color code obstacles (red)
- [ ] Color code traps (yellow)
- [ ] Highlight selected cell (green)
- [ ] Zoom in/out
- [ ] Pan view

### Phase 3: Coordinate System 🟡
- [ ] Screen to World conversion
- [ ] World to Region/Cell conversion
- [ ] Click to select cell
- [ ] Display all coordinates
- [ ] Auto-generate trap entry

### Phase 4: Export Features ⏳
- [ ] Add to trap list
- [ ] Edit trap entries
- [ ] Remove from list
- [ ] Export to file
- [ ] Import from file
- [ ] Batch operations

### Phase 5: Advanced Features ⏳
- [ ] Show NPC positions
- [ ] Show Object positions
- [ ] Filter by type
- [ ] Search functionality
- [ ] Multiple map support
- [ ] Compare client/server data

---

## 📝 Next Steps

### Option 1: Tạo Tool Mới Từ Đầu
- Tạo C# Windows Forms app mới
- Implement theo design trên
- Đơn giản, clean code
- Focus vào auto-load workflow

### Option 2: Cải Tiến Tool Cũ
- Thêm chức năng "Auto Load from Game Folder"
- Keep existing features
- Add new simplified workflow
- Backward compatible

### Option 3: Tạo Python Tool Đơn Giản
- Command-line interface
- Fast prototyping
- Easy to modify
- Good for scripting

---

## 💡 Recommendation

**Tôi đề xuất: Tạo tool MỚI đơn giản (Option 1)**

**Lý do:**
- ✅ Workflow rõ ràng, dễ hiểu
- ✅ Code sạch, dễ maintain
- ✅ UI/UX tốt hơn
- ✅ Auto-load như game
- ✅ Ít bug hơn

**Timeline:**
- Phase 1 (Auto-Load): ~2 hours
- Phase 2 (Rendering): ~3 hours
- Phase 3 (Coordinates): ~1 hour
- Phase 4 (Export): ~1 hour
- **Total: ~7 hours coding**

---

## 🎯 Kết Luận

Tool mới sẽ:
1. **Đơn giản hơn** - Chỉ cần game folder + map ID
2. **Tự động hơn** - Load tất cả regions tự động
3. **Nhanh hơn** - Không cần manual từng bước
4. **Ít lỗi hơn** - Auto validate paths
5. **Dễ dùng hơn** - Workflow giống game

**User chỉ cần:**
- Browse to game folder
- Nhập Map ID
- Click Load
- Done! 🎉
