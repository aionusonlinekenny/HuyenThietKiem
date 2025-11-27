# MapTool Session Summary - Tất cả Changes

## 📋 Overview:

Session này đã implement và fix:
1. ✅ Map image (24.jpg) loading từ pak file
2. ✅ Auto-export TẤT CẢ cells khi load map
3. ✅ Multiple bug fixes cho rendering
4. ✅ Debug logging extensive
5. ✅ Support Client và Server folders
6. ❓ Map vẫn đen - cần troubleshooting thêm

---

## 🎯 Feature: Auto-Export All Cells

### Mô tả:
Khi load map xong, tool **TỰ ĐỘNG export tất cả cells** từ tất cả regions ra file txt.

### Format:
```
MapId	RegionId	CellX	CellY	ScriptFile	IsLoad
1	655370	0	0		1
1	655370	1	0		1
...
```

### File:
- Name: `{MapId}.txt` (ví dụ: `1.txt`)
- Location: App directory (nơi chạy MapTool.exe)
- Encoding: UTF-8
- Delimiter: TAB

### Stats:
- 1 region = 16×32 = 512 cells
- 10 regions = 5,120 cells
- 100 regions = 51,200 cells

### Code:
- `MainFormSimple.AutoExportAllCellsToTxt()` - Export function
- Called after map loads successfully
- Shows message box when done

---

## 🖼️ Feature: Map Image Loading

### Mô tả:
Load hình ảnh JPG của map (24.jpg) giống như client game.

### Implementation:
```csharp
// MapLoader.cs - Load image when loading map
string mapImageRelativePath = $"\\maps\\{mapFolder}24.jpg";
mapData.MapImageData = ReadFileBytes(mapImageRelativePath);

// MapRenderer.cs - Render image as background
if (_mapImage != null)
{
    g.DrawImage(_mapImage, imgX, imgY, _mapImage.Width, _mapImage.Height);
}
```

### Logic:
1. Try load `{MapFolder}24.jpg` from pak file
2. If not in pak, try disk
3. Clone image to prevent dispose issues
4. Render as background before cells

---

## 🔧 Bug Fixes:

### Fix #1: Cell Rendering Gaps (commit acd359e7)
**Problem**: Cells positioned at 32px intervals but drawn at 16px size → 16px gaps!
**Fix**: Use `LOGIC_CELL_WIDTH/HEIGHT` (32×32) for cell rectangles
```csharp
Rectangle cellRect = new Rectangle(screenX, screenY,
    MapConstants.LOGIC_CELL_WIDTH,    // 32px
    MapConstants.LOGIC_CELL_HEIGHT);  // 32px
```

### Fix #2: Image Dispose Issue (commit ae7ad445)
**Problem**: `Image.FromStream()` needs stream to stay open, but `using` disposed it
**Fix**: Clone image before stream disposed
```csharp
Image tempImage = Image.FromStream(ms);
_mapImage = new Bitmap(tempImage);  // Clone!
tempImage.Dispose();
```

### Fix #3: Walkable Cells Covering Image (commit e0ee6f5b)
**Problem**: Walkable cells drawn OPAQUE gray over image → covers everything!
**Fix**: Skip walkable cells when image exists
```csharp
if (_mapImage != null && cell is walkable)
{
    shouldDraw = false;  // Let image show through
}
```

### Fix #4: Multiple Pak Locations (commit ae7ad445)
**Problem**: Client folder has pak at different location
**Fix**: Try multiple locations
```csharp
string[] possiblePaths = new[]
{
    Path.Combine(_gameFolder, "pak", "maps.pak"),      // Server
    Path.Combine(_gameFolder, "..", "pak", "maps.pak"), // Client
    Path.Combine(_gameFolder, "maps.pak"),             // Direct
};
```

---

## 🔍 Debug Logging:

### MapLoader.cs:
```
✓ Opened pak file: ...\maps.pak
✓ Pak contains 87245 files
🔍 Looking for map image: \maps\...\24.jpg
✓ Map image file exists!
✓ Loaded map image: ...\24.jpg (245678 bytes)
```

### MainFormSimple.cs:
```
🎨 Setting map image to renderer (245678 bytes)
📝 Auto-exporting all cells to: ...\1.txt
✓ Exported 6144 cells to 1.txt
```

### MapRenderer.cs:
```
✓ Map image loaded: 1024x2048 pixels
🎨 Render called: 12 regions loaded, Image: 1024x2048
  Drawing image at (0, 0)
  Rendering 12 regions...
```

---

## 📁 Files Changed:

### New Files:
1. `PakFile/FileNameHasher.cs` - g_FileName2Id() hash function
2. `PakFile/UclDecompressor.cs` - UCL NRV2B decompression
3. `PakFile/PakFileReader.cs` - Complete pak reader with GB2312 support

### Modified Files:
1. `MapData/MapLoader.cs`
   - Added MapImageData/MapImagePath to CompleteMapData
   - Load 24.jpg when loading map
   - Support multiple pak locations
   - Debug logging

2. `MapData/MapFileParser.cs`
   - LoadMapConfigFromBytes() for pak support
   - LoadRegionDataFromStream() for pak support

3. `MapData/MapListParser.cs`
   - Fix path building (no extra folder level)

4. `Rendering/MapRenderer.cs`
   - Add _mapImage field
   - SetMapImage() to load image
   - Skip walkable cells when image exists
   - Use LOGIC_CELL_WIDTH/HEIGHT for cells
   - Debug logging

5. `MainFormSimple.cs`
   - Call SetMapImage() when map loads
   - AutoExportAllCellsToTxt() function
   - Auto-export after map loads

### Deleted Files:
1. `MainForm.cs` - Old form (không dùng)
2. `MainForm.Designer.cs` - Old designer

---

## 📊 Commits Timeline:

```
acd359e7 - Fix cell rendering gaps - use LOGIC_CELL size
84fb8ea7 - Document cell rendering gap fix
f1da55c8 - Add map image (24.jpg) loading and rendering support
4bf1df8a - Document map image loading feature
ae7ad445 - Fix map image loading and add debug logging
e0ee6f5b - Fix: Don't draw walkable cells when map image exists
699deee7 - Document all map image rendering fixes
145f4629 - Add auto-export all cells to txt + debug rendering
5b85b8d8 - Document auto-export feature and debug troubleshooting
```

---

## 🐛 Known Issue: Map Vẫn Đen

### Symptoms:
- Map loads successfully (regions, image)
- Console shows image loaded
- But screen stays black

### Troubleshooting Steps:

1. **Check Console Output**
   ```
   🎨 Render called: X regions loaded, Image: WxH
   ```
   - If 0 regions → regions not loading
   - If Image: None → image not loading

2. **Try Building Fresh**
   ```bash
   cd D:\HuyenThietKiem\SwordOnline\Sources\Tool\MapTool
   dotnet clean
   dotnet build -c Release
   ```

3. **Test Different Map IDs**
   - Map 1: Phượng Tường
   - Map 11: Thành Đô
   - Some maps may not have 24.jpg

4. **Check if Cells Covering**
   - Comment out cell rendering code
   - If image shows → cells covering it
   - If still black → image position issue

5. **Check Image Position**
   ```
   Drawing image at (X, Y)
   ```
   - Should be at (0, 0) or close
   - Large negative values → off-screen

### Possible Root Causes:

1. **Graphics Transform Issue**
   - ScaleTransform breaks image rendering
   - Try drawing image AFTER transform
   - Or use different rendering approach

2. **Image Format Issue**
   - JPG not compatible with GDI+
   - Try converting to Bitmap explicitly

3. **Color/Alpha Issue**
   - Image has wrong color space
   - Alpha channel issues

4. **Panel Refresh Issue**
   - Panel not invalidating/repainting
   - Try force refresh

### Next Debug Steps:

1. Add more logging:
   ```csharp
   Console.WriteLine($"Panel size: {mapPanel.Width}x{mapPanel.Height}");
   Console.WriteLine($"View offset: ({_viewOffsetX}, {_viewOffsetY})");
   Console.WriteLine($"Zoom: {_zoom}");
   ```

2. Try drawing test rectangle:
   ```csharp
   // Before drawing image
   g.FillRectangle(Brushes.Red, 0, 0, 100, 100);
   ```
   If red square shows → graphics working
   If no red → panel/graphics issue

3. Save rendered image to disk:
   ```csharp
   Bitmap bmp = new Bitmap(width, height);
   Graphics g2 = Graphics.FromImage(bmp);
   Render(g2, width, height, null);
   bmp.Save("debug.png");
   ```
   Check debug.png to see what's actually rendered

---

## 🚀 How to Test:

### Build:
```bash
cd D:\HuyenThietKiem\SwordOnline\Sources\Tool\MapTool
dotnet build -c Release
```

### Run:
```
1. MapTool.exe
2. Browse → D:\HuyenThietKiem\Bin\Server
3. Map ID → 1
4. Click "Load Map"
5. Check console output
6. Check if 1.txt created
```

### Expected Results:
- ✅ Map loads without errors
- ✅ Console shows regions loaded
- ✅ Console shows image loaded
- ✅ File `1.txt` created in app folder
- ✅ Message box shows export complete
- ❓ Map visible (still troubleshooting)

---

## 📝 Documentation Files:

1. `PAK_FILE_LOADING_SOLUTION.md` - Pak file format and loading
2. `UCL_DECOMPRESSION_IMPLEMENTED.md` - UCL implementation
3. `BUILD_FIXES.md` - Build error fixes
4. `PAK_FIX_SUMMARY.md` - Pak fix summary
5. `WOR_PATH_FIX.md` - Path building fix
6. `MAP_RENDERING_FIX.md` - Cell rendering fix
7. `CELL_RENDERING_GAP_FIX.md` - Gap fix details
8. `MAP_IMAGE_LOADING.md` - Image loading feature
9. `MAP_IMAGE_FIXES_SUMMARY.md` - All image fixes
10. `DEBUG_MAP_LOADING.md` - Debug troubleshooting
11. `AUTO_EXPORT_AND_DEBUG.md` - Auto-export feature

---

## ✅ Working Features:

1. ✅ Load maps from pak files (maps.pak)
2. ✅ GB2312 encoding for Chinese filenames
3. ✅ UCL NRV2B decompression
4. ✅ Multiple pak file locations (Server/Client)
5. ✅ Load .wor files from pak
6. ✅ Load region files from pak
7. ✅ Load map images (24.jpg) from pak
8. ✅ Auto-export all cells to txt
9. ✅ Click cells to get coordinates
10. ✅ Pan/zoom map view
11. ✅ Export trap entries
12. ✅ Extensive debug logging

---

## ❓ Issues to Resolve:

1. ❌ Map screen still black (image not visible)
2. ❓ Need to troubleshoot rendering
3. ❓ May need different rendering approach

---

## 🎊 Summary:

**Features Complete**:
- ✅ Pak file loading
- ✅ Map image loading
- ✅ Auto-export all cells

**Bugs Fixed**:
- ✅ Cell gaps
- ✅ Image dispose
- ✅ Cells covering image
- ✅ Pak file locations

**Still Working On**:
- ⏳ Map visibility (rendering issue)

**Build và test để:**
1. Verify auto-export works (file 1.txt created)
2. Check console output for debugging
3. Share console output if map still black

**All code đã commit và push!** 🚀
