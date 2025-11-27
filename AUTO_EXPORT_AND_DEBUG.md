# Auto-Export All Cells + Debug Map Rendering

## ✅ Features đã thêm:

### 1. Auto-Export All Cells to Txt

Khi load map xong, tool **TỰ ĐỘNG export tất cả cells** ra file txt!

#### Logic:
- Export **TẤT CẢ cells** từ **TẤT CẢ regions** đã load
- Format: `MapId	RegionId	CellX	CellY	ScriptFile	IsLoad`
- ScriptFile column **để trống** (tab)
- IsLoad **luôn = 1**
- File name: `{MapId}.txt` (ví dụ: `1.txt`, `11.txt`)
- Saved in: **App directory** (nơi chạy MapTool.exe)

#### Code:
```csharp
private void AutoExportAllCellsToTxt()
{
    string fileName = $"{_currentMap.MapId}.txt";
    string filePath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, fileName);

    using (StreamWriter writer = new StreamWriter(filePath, false, Encoding.UTF8))
    {
        // Header
        writer.WriteLine("MapId\tRegionId\tCellX\tCellY\tScriptFile\tIsLoad");

        // Loop through all loaded regions
        foreach (var region in _currentMap.Regions.Values)
        {
            if (!region.IsLoaded) continue;

            // Loop through all cells (16x32)
            for (int cellY = 0; cellY < 32; cellY++)
            {
                for (int cellX = 0; cellX < 16; cellX++)
                {
                    // Format: MapId	RegionId	CellX	CellY	(empty)	1
                    writer.WriteLine($"{_currentMap.MapId}\t{region.RegionID}\t{cellX}\t{cellY}\t\t1");
                }
            }
        }
    }

    MessageBox.Show($"Auto-exported {totalCells} cells to:\n{fileName}");
}
```

#### Khi nào export?
- Tự động sau khi load map thành công
- Không cần click button!
- Shows message box khi xong

#### Example Output (1.txt):

```
MapId	RegionId	CellX	CellY	ScriptFile	IsLoad
1	655370	0	0		1
1	655370	1	0		1
1	655370	2	0		1
...
1	655370	15	0		1
1	655370	0	1		1
1	655370	1	1		1
...
1	655370	15	31		1
1	655371	0	0		1
...
```

- **1 region** = 16×32 = **512 cells**
- **10 regions** = **5,120 cells**
- **100 regions** = **51,200 cells**

#### Import vào Excel/Database:
- File dùng TAB-separated (TSV)
- Có thể import trực tiếp vào Excel
- Hoặc import vào SQL database
- ScriptFile column trống để fill sau

---

### 2. Debug Logging cho Map Rendering

Added extensive logging để debug vấn đề map đen.

#### Console Output khi render:
```
🎨 Render called: 12 regions loaded, Image: 1024x2048
  Drawing image at (0, 0)
  Rendering 12 regions...
```

**Nếu thấy**:
```
🎨 Render called: 12 regions loaded, Image: None
```
→ Map image KHÔNG load! Check console output khi load map.

**Nếu thấy**:
```
🎨 Render called: 0 regions loaded, Image: None
```
→ Regions KHÔNG load! Check if .wor file found.

---

## 🐛 Vấn đề Map Đen - Troubleshooting:

### Test 1: Build lại tool
```bash
cd D:\HuyenThietKiem\SwordOnline\Sources\Tool\MapTool
dotnet build -c Release
```

### Test 2: Chạy và load map
```
1. Run MapTool.exe
2. Browse → D:\HuyenThietKiem\Bin\Server
3. Map ID → 1
4. Click "Load Map"
5. CHECK CONSOLE OUTPUT!
```

### Test 3: Check console output

#### Expected (Working):
```
✓ Opened pak file: D:\...\maps.pak
✓ Pak contains 87245 files
✓ Loaded .wor from pak: \maps\西北南区\凤翔.wor
✓ Loaded region (10,20) from pak
...
🔍 Looking for map image: \maps\西北南区\凤翔24.jpg
✓ Map image file exists!
✓ Loaded map image: ...\凤翔24.jpg (245678 bytes)
🎨 Setting map image to renderer (245678 bytes)
✓ Map image loaded: 1024x2048 pixels

🎨 Render called: 12 regions loaded, Image: 1024x2048
  Drawing image at (0, 0)
  Rendering 12 regions...

📝 Auto-exporting all cells to: D:\...\1.txt
✓ Exported 6144 cells to 1.txt
```

#### If Black Screen (Not Working):
```
🎨 Render called: 12 regions loaded, Image: 1024x2048
  Drawing image at (0, 0)
  Rendering 12 regions...
```

→ Image loads nhưng vẫn đen!

**Possible causes**:

1. **Image vẽ off-screen**
   - Image position (0, 0) nhưng view offset khác
   - Fix: Reset view offset = 0

2. **Cells vẫn đè lên image**
   - Walkable cells vẫn draw opaque
   - Fix: Check shouldDraw logic

3. **Graphics transform issue**
   - ScaleTransform breaks image rendering
   - Fix: Draw image before transform

---

## 🔧 Quick Fixes to Try:

### Fix 1: Don't draw ANY cells when image exists

Edit `MapRenderer.cs:186-203`:
```csharp
// Skip drawing ALL cells when we have image
if (_mapImage != null)
{
    shouldDraw = false;  // NEVER draw cells when image exists
}
else
{
    // Only draw cells if NO image
    shouldDraw = true;
}
```

### Fix 2: Draw image AFTER transform

Edit `MapRenderer.cs:124-141`:
```csharp
// Move image drawing AFTER ScaleTransform
// So image scales with zoom

// Apply zoom transform FIRST
g.ScaleTransform(_zoom, _zoom);

// Then draw image (will be scaled)
if (_mapImage != null)
{
    int imgX = _mapImageOffsetX - _viewOffsetX;
    int imgY = _mapImageOffsetY - _viewOffsetY;
    g.DrawImage(_mapImage, imgX, imgY);  // Remove width/height (auto-scale)
}
```

### Fix 3: Test without cells at all

Temporarily comment out cell rendering:
```csharp
// TEMPORARY: Skip cell rendering to test if image shows
/*
foreach (var region in _loadedRegions.Values)
{
    ...
    RenderRegion(g, region, selectedCoord);
}
*/
```

If map shows → Cells are covering it!
If map still black → Image loading or positioning issue!

---

## 📊 Export File Stats:

### File Size Examples:

| Regions | Cells | File Size (approx) |
|---------|-------|-------------------|
| 1 | 512 | 20 KB |
| 10 | 5,120 | 200 KB |
| 50 | 25,600 | 1 MB |
| 100 | 51,200 | 2 MB |

### Format Details:
- **Encoding**: UTF-8 with BOM
- **Delimiter**: TAB (`\t`)
- **Line ending**: CRLF (`\r\n`)
- **Header**: Yes (first line)

### Import to SQL:
```sql
CREATE TABLE MapCells (
    MapId INT,
    RegionId INT,
    CellX INT,
    CellY INT,
    ScriptFile VARCHAR(255),
    IsLoad INT
);

BULK INSERT MapCells
FROM 'C:\path\to\1.txt'
WITH (
    FIELDTERMINATOR = '\t',
    ROWTERMINATOR = '\n',
    FIRSTROW = 2  -- Skip header
);
```

---

## 🎯 Next Steps:

1. **Build lại tool**
   ```bash
   dotnet build -c Release
   ```

2. **Load map ID 1**
   - Check console output
   - See if regions load
   - See if image loads
   - Check render logs

3. **Check exported file**
   - File: `{MapId}.txt` in app folder
   - Verify format is correct
   - Check cell count matches regions

4. **Share console output** if still black!
   - Full console log from load to render
   - Helps identify exact issue

---

## ✅ Summary:

- ✅ Auto-export TẤT CẢ cells khi load map
- ✅ File format: TSV (tab-separated)
- ✅ ScriptFile để trống
- ✅ File name = Map ID
- ✅ Debug logging extensive
- 🔍 Vẫn cần troubleshoot map đen

**Build và test ngay! Sẽ có file txt tự động export!** 📝
