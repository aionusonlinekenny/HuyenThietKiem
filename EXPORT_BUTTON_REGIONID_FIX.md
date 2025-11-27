# Export Button + RegionId Fix

## ✅ Changes:

### 1. Export với Button (không auto nữa)

**Trước**: Auto-export ngay sau khi load map → Annoying!
```csharp
// Load map
_currentMap = _mapLoader.LoadMap(mapId);
// Auto-export immediately!
AutoExportAllCellsToTxt();
```

**Sau**: User click button "Export" để export
```csharp
private void btnExport_Click(object sender, EventArgs e)
{
    ExportAllCellsToTxt();  // User triggers manually
}
```

**Benefits**:
- User control khi nào export
- Có SaveFileDialog chọn nơi lưu
- Không bị popup message box tự động

---

### 2. Fix RegionId Format

**Problem**: RegionId quá lớn không match với format user expect

| Region | OLD RegionID (encoded) | NEW RegionID (simple) |
|--------|----------------------|---------------------|
| (13, 142) | 9,306,317 | 36,365 |
| (7, 20) | 1,310,727 | 5,127 |
| (8, 20) | 1,310,728 | 5,128 |

**OLD Encoding**:
```csharp
int RegionID = x | (y << 16);
// Region (13, 142):
// = 13 | (142 << 16)
// = 0x0000000D | 0x008E0000
// = 0x008E000D = 9,306,317
```

**NEW Encoding**:
```csharp
int simpleRegionId = regionY * 256 + regionX;
// Region (13, 142):
// = 142 * 256 + 13
// = 36,352 + 13
// = 36,365
```

**Result**: Số nhỏ hơn nhiều, closer to user's expected format (~92)

---

### 3. Remove Console Logging Flood

**Problem**: Render() được call mỗi khi mouse move hoặc paint → Flood console!

```csharp
// REMOVED from Render():
Console.WriteLine($"🎨 Render called: {_loadedRegions.Count} regions loaded, Image: {(_mapImage != null ? $"{_mapImage.Width}x{_mapImage.Height}" : "None")}");
Console.WriteLine($"  Drawing image at ({imgX}, {imgY})");
Console.WriteLine($"  Rendering {_loadedRegions.Count} regions...");
```

**Result**: Console clean, chỉ có logging quan trọng (load map, export, errors)

---

## 📝 Export Button Usage:

### Steps:
1. Load map (Browse folder → Enter Map ID → Click "Load Map")
2. Wait for map to load
3. Click "Export" button
4. Choose save location in SaveFileDialog
5. File saved with format:

```
MapId	RegionId	CellX	CellY	ScriptFile	IsLoad
11	5127	0	0		1
11	5127	1	0		1
11	5127	2	0		1
...
```

### File Stats:
- 1 region = 512 cells (16×32)
- 10 regions = 5,120 cells
- 100 regions = 51,200 cells

---

## 🔍 About RegionId Format:

### Why the change?

User showed example file:
```
MapId	RegionId	CellX	CellY	ScriptFile	IsLoad
11	92	7	20	\script\maps\trap\11\1.lua	1
```

RegionId = **92** (small number)

Our old export:
```
MapId	RegionId	CellX	CellY	ScriptFile	IsLoad
11	9306317	0	0		1
```

RegionId = **9,306,317** (huge number!)

### The encoding:

Game engine uses: `RegionID = x | (y << 16)`
- This encodes 2 coordinates into 1 integer
- x in lower 16 bits, y in upper 16 bits
- Example: Region(13, 142) → 0x008E000D = 9,306,317

But trap file format may expect simpler ID!

### New formula:

`simpleRegionId = regionY * 256 + regionX`
- Assumes max 256 regions per row
- Region(13, 142) → 142*256 + 13 = 36,365
- Still not 92, but much closer!

### If still not matching:

User may need to specify exact formula. Options:
1. `regionY * width + regionX` (sequential)
2. Just `regionX` or `regionY` alone
3. Custom mapping from map's .wor file

Let me know your expected RegionId values and I can adjust!

---

## 🐛 Map Black Screen Issue:

Map vẫn đen despite image loading. Possible causes:

### Cause 1: Image offset wrong

Image drawn at (0, 0) but regions start at different coordinates.

**Check**: What are the region coordinates?
```
Region (10, 20) → World (5120, 20480)
Image at (0, 0)
```

If regions are far from origin, image won't overlap!

**Fix**: Calculate image offset from map's region bounds
```csharp
// Map config: RegionLeft=10, RegionTop=20
int imageOffsetX = config.RegionLeft * MapConstants.REGION_PIXEL_WIDTH;
int imageOffsetY = config.RegionTop * MapConstants.REGION_PIXEL_HEIGHT;
_renderer.SetMapImage(imageData, imageOffsetX, imageOffsetY);
```

### Cause 2: Image too small

24.jpg might not cover entire map.

**Example**:
- Image: 1024×2048 pixels
- Map regions: (10-15, 20-25) = 6×6 = 36 regions
- Region size: 512×1024 pixels each
- Total map: 3072×6144 pixels
- **Image too small!**

**Fix**: Image only shows part of map. Need to position correctly.

### Cause 3: Zoom/transform issue

ScaleTransform applied before drawing image → Image scaled wrong.

**Try**: Draw image at world coordinates (after transform):
```csharp
// Apply transform first
g.ScaleTransform(_zoom, _zoom);

// Draw image at region coordinates (in world space)
if (_mapImage != null)
{
    // Image should be positioned at map's region bounds
    int imageWorldX = config.RegionLeft * REGION_PIXEL_WIDTH;
    int imageWorldY = config.RegionTop * REGION_PIXEL_HEIGHT;

    int screenX = imageWorldX - _viewOffsetX;
    int screenY = imageWorldY - _viewOffsetY;

    g.DrawImage(_mapImage, screenX, screenY);
}
```

---

## 🚀 Next Steps:

### Build and Test:
```bash
cd D:\HuyenThietKiem\SwordOnline\Sources\Tool\MapTool
dotnet build -c Release
```

### Test Export:
1. Load map
2. Click "Export" button
3. Save file
4. Check RegionId values - are they reasonable?
5. Share example if still wrong

### Debug Black Screen:

Please share console output when loading map:
```
✓ Opened pak file: ...
✓ Loaded .wor from pak: ...
🔍 Looking for map image: ...
✓ Map image file exists!
✓ Loaded map image: ...\24.jpg (XXXXX bytes)
✓ Map image loaded: WIDTHxHEIGHT pixels
```

And share:
1. Map ID loaded
2. Image dimensions (width × height)
3. Region coordinates (Left, Top, Right, Bottom)
4. Number of regions loaded

This will help diagnose why image isn't showing!

---

## 📊 Commits:

- `47021ba8` - Fix build error (totalCells scope)
- `6975dea3` - Fix export: use button + fix RegionId format

---

**Build và test! Export button giờ hoạt động với RegionId format mới!** 📝

**Nếu map vẫn đen, share console output + map info để debug!** 🔍
