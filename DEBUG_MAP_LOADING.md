# Debug Map Loading - Troubleshooting Guide

## 🐛 Vấn đề hiện tại:

1. Map vẫn đen thui (không thấy hình)
2. Map image (24.jpg) không load được
3. Client folder báo lỗi ".wor file not found in pak"

## 🔍 Debug Steps:

### Step 1: Rebuild tool với debug logging

```bash
cd D:\HuyenThietKiem\SwordOnline\Sources\Tool\MapTool
dotnet build -c Release
```

### Step 2: Chạy tool và xem Console Output

Tool bây giờ có extensive logging. Khi load map, bạn sẽ thấy:

```
✓ Opened pak file: D:\...\Bin\Server\pak\maps.pak
✓ Pak contains 87245 files
🔍 Looking for map image: \maps\西北南区\凤翔24.jpg
✓ Map image file exists!
✓ Loaded map image: \maps\西北南区\凤翔24.jpg (245678 bytes)
🎨 Setting map image to renderer (245678 bytes)
✓ Map image loaded: 1024x2048 pixels
```

### Step 3: Kiểm tra Console Output

**Test 1: Server folder**
```
1. Browse → D:\HuyenThietKiem\Bin\Server
2. Map ID → 1
3. Click "Load Map"
4. Check console output
```

**Nếu thấy**:
```
✓ Opened pak file: ...
✓ Loaded map image: ...
✓ Map image loaded: 1024x2048 pixels
```
→ Image should be visible! Nếu vẫn đen → Check rendering code

**Nếu thấy**:
```
❌ No map image found at: \maps\...
```
→ File không có trong pak hoặc trên disk

**Test 2: Client folder**
```
1. Browse → D:\HuyenThietKiem\Bin\Client
2. Map ID → 1
3. Click "Load Map"
4. Check console output
```

**Nếu thấy**:
```
ℹ No pak file found at any location, will read from disk
  Tried paths:
    - D:\...\Bin\Client\pak\maps.pak
    - D:\...\Bin\pak\maps.pak
    - D:\...\Bin\Client\maps.pak
```
→ Pak file không tồn tại ở tất cả locations

## 🔧 Possible Issues & Fixes:

### Issue 1: Image loaded nhưng vẫn đen

**Nguyên nhân**: Image bị dispose hoặc offset sai

**Fix đã implement**:
```csharp
// OLD (BUG):
using (MemoryStream ms = new MemoryStream(imageData))
{
    _mapImage = Image.FromStream(ms);
    // Stream disposed → image invalid!
}

// NEW (FIXED):
using (MemoryStream ms = new MemoryStream(imageData))
{
    Image tempImage = Image.FromStream(ms);
    _mapImage = new Bitmap(tempImage); // Clone!
    tempImage.Dispose();
}
```

**Test**: Rebuild và load lại. Should work now!

### Issue 2: Map image không tồn tại trong pak

**Check files trong pak**:
```bash
# Trên Windows, mở file:
D:\HuyenThietKiem\Bin\Server\pak\maps.pak.txt

# Search for "24.jpg"
Ctrl+F → "24.jpg"
```

**Expected to see**:
```
5678    0x12345678    245678    \maps\西北南区\凤翔24.jpg
```

**Nếu KHÔNG thấy**:
- File 24.jpg không có trong pak
- Cần extract từ original game pak
- Hoặc copy từ client files

### Issue 3: Client folder không có pak

**Kiểm tra Client folder structure**:
```
Bin/
├── Client/
│   ├── game.exe
│   └── maps/           ← Extracted files?
├── Server/
│   ├── gameserver.exe
│   └── pak/
│       └── maps.pak    ← Pak file here
└── pak/
    └── maps.pak        ← Or here?
```

**Solutions**:

**Option A**: Point to Server folder instead
```
Browse → D:\HuyenThietKiem\Bin\Server
```

**Option B**: Extract pak to Client folder
```bash
# Use UnpackTool to extract maps.pak
UnpackTool.exe D:\...\Bin\Server\pak\maps.pak D:\...\Bin\Client\maps\
```

### Issue 4: Cells vẽ đè lên image (không thấy image)

**Check cell colors**:
```csharp
// Cells should have ALPHA for transparency
private Color _walkableCellColor = Color.FromArgb(255, 60, 60, 60);  // Opaque!
private Color _obstacleColor = Color.FromArgb(180, 255, 0, 0);       // Semi-transparent
```

**Problem**: Walkable cells (gray) are OPAQUE → Cover image completely!

**Fix**: Make walkable cells transparent or remove them when image exists:

```csharp
// Option 1: Make walkable transparent
private Color _walkableCellColor = Color.FromArgb(50, 60, 60, 60);  // Very transparent

// Option 2: Don't draw walkable cells when image exists
if (_mapImage == null || region.Obstacles[cx, cy] != 0 || region.Traps[cx, cy] != 0)
{
    // Only draw if no image OR cell has data
    using (SolidBrush brush = new SolidBrush(cellColor))
    {
        g.FillRectangle(brush, cellRect);
    }
}
```

## 🎯 Expected Console Output (Working):

```
═══════════════════════════════════════
LOADING MAP ID: 1
═══════════════════════════════════════
✓ Opened pak file: D:\HuyenThietKiem\Bin\Server\pak\maps.pak
✓ Pak contains 87245 files
✓ Loaded .wor from pak: \maps\西北南区\凤翔.wor

Loading regions...
✓ Loaded region (10,20) from pak
✓ Loaded region (10,21) from pak
... (more regions)
Total: 12/12 regions loaded

🔍 Looking for map image: \maps\西北南区\凤翔24.jpg
✓ Map image file exists!
✓ Loaded map image: \maps\西北南区\凤翔24.jpg (245678 bytes)

🎨 Setting map image to renderer (245678 bytes)
✓ Map image loaded: 1024x2048 pixels

═══════════════════════════════════════
MAP LOAD COMPLETE!
═══════════════════════════════════════
```

## 🚨 Common Error Messages:

### Error 1: "Failed to load map: .wor file not found in pak"

**Console shows**:
```
❌ No map image found at: \maps\...
  Pak reader: Not available
  Disk path: D:\...\Bin\Client\maps\...24.jpg
  Disk exists: False
```

**Solution**:
- Use Server folder (has pak file)
- Or extract pak to Client folder

### Error 2: "Map loaded (no image)"

**Console shows**:
```
❌ No map image found at: \maps\西北南区\凤翔24.jpg
```

**Solution**:
- Check if 24.jpg exists in pak.txt
- Extract pak if file is missing
- Use different map (some maps may not have 24.jpg)

### Error 3: Map loads but stays black

**Console shows**:
```
✓ Map image loaded: 1024x2048 pixels
⚠ No map image data available
```

**Solution**: Image loaded in MapLoader but not passed to MainForm
- Check CompleteMapData.MapImageData is not null
- Check MainFormSimple calls SetMapImage()

## 📝 Quick Checklist:

- [ ] Tool rebuilt with latest code
- [ ] Server folder selected (not Client)
- [ ] Console shows "✓ Opened pak file"
- [ ] Console shows "✓ Loaded map image"
- [ ] Console shows "✓ Map image loaded: WxH pixels"
- [ ] Map panel refreshes after load
- [ ] No exceptions in console

## 🎊 If all else fails:

**Test with known-good map**:
```
Map ID: 1 (Phượng Tường)
Folder: \maps\西北南区\凤翔
Image: \maps\西北南区\凤翔24.jpg
```

**Verify pak contents**:
```bash
# Open pak.txt and search
findstr "凤翔24.jpg" "D:\HuyenThietKiem\Bin\Server\pak\maps.pak.txt"
```

**Should see**:
```
12345   0x12345678   245678   \maps\西北南区\凤翔24.jpg
```

If this file exists in pak.txt but tool can't load it:
- Check GB2312 encoding in PakFileReader
- Check filename hash calculation
- Verify UCL decompression

---

**Share console output** nếu vẫn không work! 🔍
