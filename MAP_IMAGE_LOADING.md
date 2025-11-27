# Map Image Loading - Hiển thị hình ảnh map thực sự!

## ✅ Đã implement!

Tool giờ có thể load và hiển thị **hình ảnh JPG** của map giống như client game!

## 🎨 Cách hoạt động:

### 1. Client game render map như thế nào?

```cpp
// ScenePlaceMapC.cpp (game client)
#define PLACE_MAP_FILE_NAME_APPEND "24.jpg"

// Load map image: \maps\{MapFolder}24.jpg
sprintf(m_szEntireMapFile, "%s"PLACE_MAP_FILE_NAME_APPEND, pszScenePlaceRootPath);
m_bHavePicMap = g_FileExists(m_szEntireMapFile);

// Example paths:
// \maps\西北南区\凤翔24.jpg
// \maps\西南北区\成都\成都24.jpg
```

Client load file JPG này làm background map, sau đó vẽ NPCs/players/effects lên trên.

### 2. Tool của chúng ta giờ làm gì?

**MapLoader.cs** - Auto-load JPG khi load map:
```csharp
// Step 5: Try to load map image (24.jpg)
string mapImageRelativePath = $"\\maps\\{mapEntry.FolderPath}24.jpg";
if (FileExists(mapImageRelativePath))
{
    mapData.MapImageData = ReadFileBytes(mapImageRelativePath);
    mapData.MapImagePath = mapImageRelativePath;
    Console.WriteLine($"✓ Loaded map image: {mapImageRelativePath}");
}
```

**MapRenderer.cs** - Vẽ JPG as background:
```csharp
// Draw map background image if available
if (_mapImage != null)
{
    int imgX = _mapImageOffsetX - _viewOffsetX;
    int imgY = _mapImageOffsetY - _viewOffsetY;
    g.DrawImage(_mapImage, imgX, imgY, _mapImage.Width, _mapImage.Height);
}

// Draw loaded regions (overlay on top of map image)
foreach (var region in _loadedRegions.Values)
{
    // Vẽ cells với màu semi-transparent lên trên
}
```

**MainFormSimple.cs** - Set image khi load:
```csharp
if (_currentMap.MapImageData != null)
{
    _renderer.SetMapImage(_currentMap.MapImageData);
    lblStatus.Text = $"Map loaded with image! {_currentMap.LoadedRegionCount} regions.";
}
```

## 📊 Rendering layers (từ dưới lên):

```
Layer 1 (Bottom):   Background color (dark gray #141414)
Layer 2:            Map JPG image (24.jpg) - Hình ảnh map thực sự!
Layer 3:            Cell overlays (walkable/obstacles/traps với alpha)
Layer 4:            Grid lines (semi-transparent)
Layer 5:            Region borders (blue)
Layer 6:            Selected cell highlight (green)
Layer 7 (Top):      Coordinate info box
```

## 🎯 Kết quả:

### Trước (chỉ có màu cells):
```
████████████████████████  ← Walkable cells (gray)
██▓▓██████████▓▓████████  ← Obstacles (red)
████░░██████████░░██████  ← Traps (yellow)
```
→ Functional nhưng không đẹp!

### Sau (có hình JPG):
```
🏞️ [Beautiful map image background]
    ├─ Trees, buildings, terrain visible
    ├─ Semi-transparent red overlay on obstacles
    ├─ Semi-transparent yellow overlay on traps
    └─ Grid and borders on top
```
→ Giống client game!

## 📁 Map image files:

### Vị trí files:

**Option 1: Trong pak file** (khuyên dùng)
- File: `Bin/Server/pak/maps.pak`
- Path trong pak: `\maps\{MapFolder}24.jpg`
- Tool tự động load từ pak bằng PakFileReader

**Option 2: Trên disk** (nếu đã extract)
- Path: `Bin/Server/maps/{MapFolder}24.jpg`
- Ví dụ: `Bin/Server/maps/西北南区/凤翔24.jpg`

### Format:
- JPEG image (.jpg)
- Size khác nhau tùy map
- Thường 1024x2048 hoặc lớn hơn

## 🔧 API mới:

### CompleteMapData
```csharp
public class CompleteMapData
{
    // ... existing properties ...

    public byte[] MapImageData { get; set; }  // JPG image bytes
    public string MapImagePath { get; set; }  // Relative path
}
```

### MapRenderer
```csharp
// Set map background image
public void SetMapImage(byte[] imageData, int offsetX = 0, int offsetY = 0)

// Clear map image
public void ClearMapImage()
```

## 🚀 Sử dụng:

```csharp
// Load map (auto-loads image)
MapLoader loader = new MapLoader(gameFolder, isServer: true);
CompleteMapData map = loader.LoadMap(mapId: 1);

// Check if image loaded
if (map.MapImageData != null)
{
    Console.WriteLine($"Map image loaded: {map.MapImagePath}");
    Console.WriteLine($"Image size: {map.MapImageData.Length} bytes");

    // Set to renderer
    renderer.SetMapImage(map.MapImageData);
}
else
{
    Console.WriteLine("No map image available (will show colored cells only)");
}

// Render
renderer.Render(graphics, width, height);
```

## ⚙️ Fallback behavior:

Tool vẫn hoạt động **nếu không có file JPG**:

1. **Có 24.jpg**: Hiển thị hình ảnh map đẹp + cell overlays
2. **Không có 24.jpg**: Hiển thị colored cells như trước (gray/red/yellow)

→ Backward compatible!

## 🐛 Troubleshooting:

### Map vẫn đen?

**Check 1**: Console output khi load map
```
✓ Loaded map image: \maps\西北南区\凤翔24.jpg
```
→ Image loaded OK

```
ℹ No map image found: \maps\西北南区\凤翔24.jpg
```
→ File không tồn tại, sẽ dùng colored cells

**Check 2**: File có trong pak không?
```bash
# Trên Windows
findstr "24.jpg" "Bin\Server\pak\maps.pak.txt"
```

**Check 3**: Build lại tool với code mới
```bash
cd SwordOnline\Sources\Tool\MapTool
dotnet build -c Release
```

### Image bị lệch vị trí?

Map image có thể có offset so với region grid. Hiện tại tool assume offset = (0, 0).

Nếu bị lệch, cần parse từ .wor file:
```ini
[MAIN]
MapLTRegionIndex=x,y  ; Left-Top region offset
```

→ TODO: Parse MapLTRegionIndex nếu cần

## 📝 Files đã sửa:

1. **MapLoader.cs**
   - Added `MapImageData` and `MapImagePath` to `CompleteMapData`
   - Load 24.jpg in `LoadMap()` method
   - Try pak first, fallback to disk

2. **MapRenderer.cs**
   - Added `_mapImage`, `_mapImageOffsetX/Y` fields
   - `SetMapImage()` to load from byte array
   - `ClearMapImage()` to dispose
   - Render image before cells in `Render()`

3. **MainFormSimple.cs**
   - Call `SetMapImage()` when map loads
   - Status text shows if image loaded

4. **Deleted unused files**
   - MainForm.cs (old form không dùng)
   - MainForm.Designer.cs (designer file)

## 🎊 Testing:

```bash
# Build
cd D:\HuyenThietKiem\SwordOnline\Sources\Tool\MapTool
dotnet build -c Release

# Run
MapTool.exe

# Test
1. Browse → D:\HuyenThietKiem\Bin\Server
2. Map ID → 1 (Phượng Tường)
3. Click "Load Map"
4. Check console output:
   ✓ Loaded map image: \maps\西北南区\凤翔24.jpg
5. See beautiful map background!
```

## ✨ Kết luận:

- ✅ Tool giờ hiển thị **hình ảnh map thực sự** giống client
- ✅ Auto-load từ pak hoặc disk
- ✅ Fallback to colored cells nếu không có image
- ✅ Maintain tất cả features (click cells, coordinates, export)
- ✅ Đã xóa MainForm không dùng

**Map bây giờ ĐẸP và FUNCTIONAL!** 🚀
