# MapTool - Quick Start Guide

## 🚀 Cách Dùng Tool Mới (Cực Đơn Giản!)

### Chỉ cần 3 bước:

```
1️⃣ Browse đến game folder
2️⃣ Nhập Map ID
3️⃣ Click "Load Map"
   ↓
   Done! ✅
```

---

## 📋 Chi Tiết Từng Bước

### Bước 1: Build Tool

```bash
cd SwordOnline\Sources\Tool\MapTool
dotnet build
dotnet run
```

Hoặc build trong Visual Studio.

---

### Bước 2: Chọn Game Folder

Trong tool:
1. Click nút **"Browse..."**
2. Chọn folder:
   - Server: `D:\HuyenThietKiem\Bin\Server`
   - Client: `D:\HuyenThietKiem\Bin\Client`
3. Tool tự động detect Server/Client mode

**Ví dụ:**
```
Game Folder: D:\HuyenThietKiem\Bin\Server
Mode: ⦿ Server  ○ Client
```

---

### Bước 3: Nhập Map ID và Load

1. Nhập Map ID (ví dụ: `11` cho Thành Đô)
2. Click **"Load Map"**
3. Tool tự động:
   - ✅ Đọc `Settings/MapList.ini`
   - ✅ Load file `.wor`
   - ✅ Load TẤT CẢ regions
   - ✅ Render map hoàn chỉnh

**Map Info sẽ hiện:**
```
Map: Thành Đô (ID: 11)
Folder: 场景地图\城市\成都
Type: City
Region Grid: 4x4
Map Size: 2048x4096 pixels
Loaded: 16/16 regions
```

---

### Bước 4: Click Để Lấy Coordinates

**Left Click:** Chọn cell
- Tọa độ hiện ngay trong "Selected Coordinates"

**Double Click:** Add trap entry
- Tự động add vào list

**Right Click + Drag:** Pan view (di chuyển map)

**Zoom +/-:** Phóng to/thu nhỏ

---

### Bước 5: Export

1. Double-click các cells muốn đặt trap
2. Mỗi click → auto add to list
3. Click **"Export to File"**
4. Chọn location → Save
5. Done! File trap ready to use

---

## 🎯 Example Usage: Map 11 (Thành Đô)

### Step-by-Step:

```
1. Open MapTool

2. Game Folder: D:\HuyenThietKiem\Bin\Server
   Mode: Server

3. Map ID: 11
   Click "Load Map"

   Result:
   ✅ Map: Thành Đô (ID: 11)
   ✅ Loaded: 16/16 regions
   ✅ Map rendered with obstacles

4. Click on map at position you want

   Display:
   World: (5000, 10000)
   Region: (9, 9)
   RegionID: 589833
   Cell: (12, 24)

5. Double-click → Added to list:
   11	589833	12	24	\script\maps\trap\11\1.lua	1

6. Export to File
   Save as: D:\HuyenThietKiem\Bin\Server\library\maps\Trap\11.txt

7. Done! ✅
```

---

## 📊 So Sánh Tool Cũ vs Tool Mới

### Tool Cũ (Phức tạp):
```
1. Click "Load .wor"
2. Browse to: Bin\Server\maps\场景地图\城市\成都\成都.wor
3. Wait...
4. Enter RegionX: 0
5. Enter RegionY: 0
6. Click "Load Region"
7. Wait...
8. Repeat steps 4-7 for EVERY region (16 times!)
9. Finally can click...
```
**Total: 10+ bước, manual paths, dễ sai**

### Tool Mới (Đơn giản):
```
1. Browse: D:\HuyenThietKiem\Bin\Server
2. Map ID: 11
3. Click "Load Map"
4. Done! Click anywhere → coordinates
```
**Total: 3 bước, tự động hết**

---

## 🔧 Troubleshooting

### "Failed to load map: MapList.ini not found"
→ Sai game folder. Phải chọn `Bin/Server` hoặc `Bin/Client`, không phải thư mục khác.

### "Map ID X not found in MapList.ini"
→ Map ID không tồn tại. Check lại ID trong `Settings/MapList.ini`

### "Loaded: 0/16 regions"
→ Không tìm thấy region files. Check thư mục `maps/<mapfolder>/v_XXX/`

### Map render toàn màu đen
→ Bình thường nếu map không có obstacles. Click để test coordinates.

---

## 🎨 UI Layout

```
┌──────────────────────────────────────────────────────────────┐
│ Simple Map Tool - Auto Load                                  │
├──────────────────────────────────────────────────────────────┤
│ ┌─────────────────────────┐  ┌──────────────────────────────┐│
│ │                         │  │ 1. Select Game Folder        ││
│ │                         │  │ Game Folder:                 ││
│ │                         │  │ [_________________] [Browse] ││
│ │                         │  │ ⦿ Server  ○ Client           ││
│ │      Map View           │  │                              ││
│ │    (900 x 700)          │  ├──────────────────────────────┤│
│ │                         │  │ 2. Load Map                  ││
│ │  Click to select        │  │ Map ID: [11_] [Load Map]     ││
│ │  Double-click to add    │  │                              ││
│ │  Right-drag to pan      │  ├──────────────────────────────┤│
│ │                         │  │ Map Information              ││
│ │                         │  │ Name: Thành Đô               ││
│ │                         │  │ Folder: 场景地图\城市\成都   ││
│ │                         │  │ Type: City                   ││
│ │                         │  │ Region Grid: 4x4             ││
│ │                         │  │ Loaded: 16/16 regions        ││
│ └─────────────────────────┘  │                              ││
│ [Zoom +] [Zoom -]            ├──────────────────────────────┤│
│                              │ Selected Coordinates         ││
│                              │ World X:  [5000_]            ││
│                              │ World Y:  [10000]            ││
│                              │ Region X: [9____]            ││
│                              │ Region Y: [9____]            ││
│                              │ RegionID: [589833]           ││
│                              │ Cell X:   [12___]            ││
│                              │ Cell Y:   [24___]            ││
│                              │ Script:   [\script\...\1.lua]││
│                              │                              ││
│                              ├──────────────────────────────┤│
│                              │ Trap Entries                 ││
│                              │ ┌──────────────────────────┐ ││
│                              │ │11	589833	12	24	...│ ││
│                              │ │11	589833	13	24	...│ ││
│                              │ └──────────────────────────┘ ││
│                              │ [Export] [Remove] [Clear]    ││
│                              └──────────────────────────────┘│
├──────────────────────────────────────────────────────────────┤
│ Status: Map loaded successfully! 16 regions.                 │
└──────────────────────────────────────────────────────────────┘
```

---

## 💡 Tips & Tricks

### Tip 1: Default Folder
Tool nhớ folder path. Lần sau mở không cần browse lại.

### Tip 2: Keyboard Shortcuts
- `Ctrl+Mouse Wheel`: Zoom in/out (if supported)
- `Right-drag`: Pan view nhanh

### Tip 3: Script File Pattern
Thay đổi script path trong textbox trước khi double-click:
```
\script\maps\trap\11\myTrap.lua
```

### Tip 4: Batch Export
Double-click nhiều cells → tất cả add vào list → Export 1 lần

### Tip 5: Check Loaded Regions
Nếu "Loaded: 5/16" → Map thiếu files, check thư mục maps

---

## 🎯 Common Maps

| ID | Name | Folder | Type |
|----|------|--------|------|
| 1 | Phượng Tường | 场景地图\城市\凤翔 | City |
| 11 | Thành Đô | 场景地图\城市\成都 | City |
| 21 | ? | 场景地图\...  | ? |
| 74+ | Various | ... | ... |

(Check `Settings/MapList.ini` for complete list)

---

## 📚 Technical Details

### Auto-Load Workflow:
```
1. Read: <GameFolder>\Settings\MapList.ini
   → Get: Map ID → Folder path

2. Read: <GameFolder>\maps\<folder>\<name>.wor
   → Get: Region grid bounds (rect)

3. For each region (X, Y) in grid:
   Load: <GameFolder>\maps\<folder>\v_Y\X_Region_S.dat
   → Parse combined file format
   → Extract obstacle/trap grids (16x32)

4. Render complete map
   → Each region: 16x32 cells
   → Each cell: 32x32 pixels
   → Display obstacles, traps, selected cell

5. User clicks → Calculate coordinates:
   World X/Y → Region X/Y → Cell X/Y → RegionID
```

### File Structure Accessed:
```
GameFolder/
├── Settings/
│   └── MapList.ini         ← Step 1: Map ID lookup
└── maps/
    └── <mapfolder>/        ← From MapList.ini
        ├── <name>.wor      ← Step 2: Region grid
        └── v_YYY/          ← Step 3: Region files
            └── XXX_Region_S.dat
```

---

## ✅ Checklist

**Before using:**
- [ ] Game folder exists (Bin/Server or Bin/Client)
- [ ] MapList.ini exists in Settings/
- [ ] Map folder exists in maps/
- [ ] .wor file exists
- [ ] Region files exist in v_XXX/

**When using:**
- [ ] Browse to correct game folder
- [ ] Select Server/Client mode
- [ ] Enter valid Map ID
- [ ] Click "Load Map"
- [ ] Wait for load complete
- [ ] Check "Loaded: X/Y regions"
- [ ] Click map to test coordinates

**When exporting:**
- [ ] Add entries via double-click
- [ ] Check script path is correct
- [ ] Export to appropriate folder
- [ ] Verify file format

---

## 🎉 Done!

Tool mới CỰC ĐƠN GIẢN so với trước!

**3 bước:**
1. Browse folder
2. Map ID
3. Load

**Tự động:**
- ✅ Đọc MapList.ini
- ✅ Load .wor
- ✅ Load ALL regions
- ✅ Render map
- ✅ Calculate coordinates

Không còn phải nhập paths thủ công! 🚀
