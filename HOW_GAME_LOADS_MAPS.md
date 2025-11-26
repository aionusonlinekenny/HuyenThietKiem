# Cách Game Load Map - Workflow Thực Tế

## 📋 Workflow Load Map Trong Game

### 1️⃣ Server Side (KSubWorld::LoadMap)

```cpp
BOOL KSubWorld::LoadMap(int nId)
{
    // Bước 1: Đọc MapList.ini để lấy tên folder
    g_SetFilePath("\\settings");
    IniFile.Load("MapList.ini");
    sprintf(szKeyName, "%d", nId);
    IniFile.GetString("List", szKeyName, "", szPathName, sizeof(szPathName));
    // Ví dụ: MapID 11 → szPathName = "场景地图\\城市\\成都"

    // Bước 2: Load file .wor để lấy thông tin map
    g_SetFilePath("\\maps");
    sprintf(szFileName, "%s.wor", szPathName);
    IniFile.Load(szFileName);
    // File path: \maps\场景地图\城市\成都\成都.wor

    // Bước 3: Đọc rect để biết region grid
    RECT sRect;
    IniFile.GetRect("MAIN", "rect", &sRect);
    m_nRegionBeginX = sRect.left;      // Ví dụ: 0
    m_nRegionBeginY = sRect.top;       // Ví dụ: 0
    m_nWorldRegionWidth = sRect.right - sRect.left + 1;   // Ví dụ: 4
    m_nWorldRegionHeight = sRect.bottom - sRect.top + 1;  // Ví dụ: 4

    // Bước 4: Load từng region
    for (nY = 0; nY < m_nWorldRegionHeight; nY++)
    {
        for (nX = 0; nX < m_nWorldRegionWidth; nX++)
        {
            // Server load từ file:
            // \maps\场景地图\城市\成都\v_YYY\XXX_Region_S.dat
            m_Region[nIdx].Load(nX + m_nRegionBeginX, nY + m_nRegionBeginY);
            m_Region[nIdx].LoadObject(nSubWorld, nX + m_nRegionBeginX, nY + m_nRegionBeginY);
        }
    }
}
```

### 2️⃣ Client Side (Similar workflow)

```cpp
// Client load từ:
// \maps\<mapfolder>\v_YYY\XXX_Region_C.dat
BOOL KRegion::LoadObject(int nSubWorld, int nX, int nY, char *lpszPath)
{
    sprintf(szPath, "\\%s\\v_%03d", lpszPath, nY);
    sprintf(szFile, "%s\\%03d_%s", szPath, nX, REGION_COMBIN_FILE_NAME_CLIENT);
    // Ví dụ: \maps\场景地图\城市\成都\v_000\000_Region_C.dat

    if (cData.Open(szFile))
    {
        // Đọc combined file format:
        // - Header: DWORD (số section) + Array of KCombinFileSection
        // - Data: Obstacle, Trap, NPC, Object, Ground, Building
        cData.Read(&dwMaxElemFile, sizeof(DWORD));
        cData.Read(sElemFile, sizeof(sElemFile));

        // Load NPC data
        cData.Seek(dwHeadSize + sElemFile[REGION_NPC_FILE_INDEX].uOffset, FILE_BEGIN);
        LoadClientNpc(&cData, sElemFile[REGION_NPC_FILE_INDEX].uLength);

        // Load Object data
        cData.Seek(dwHeadSize + sElemFile[REGION_OBJ_FILE_INDEX].uOffset, FILE_BEGIN);
        LoadClientObj(&cData, sElemFile[REGION_OBJ_FILE_INDEX].uLength);
    }
}
```

---

## 📁 Cấu trúc Thư mục

### Server:
```
Bin/Server/
├── Settings/
│   └── MapList.ini          ← Map ID → Folder mapping
└── maps/
    └── 场景地图/            ← Category folder
        └── 城市/            ← Type folder
            └── 成都/        ← Map folder
                ├── 成都.wor ← Map info (rect, settings)
                ├── v_000/   ← Region Y = 0
                │   ├── 000_Region_S.dat  ← Region (0,0) server data
                │   ├── 001_Region_S.dat  ← Region (1,0) server data
                │   └── ...
                ├── v_001/   ← Region Y = 1
                │   ├── 000_Region_S.dat  ← Region (0,1) server data
                │   └── ...
                └── ...
```

### Client:
```
Bin/Client/
├── Settings/
│   └── MapList.ini          ← Map ID → Folder mapping
└── maps/
    └── 场景地图/            ← Category folder
        └── 城市/            ← Type folder
            └── 成都/        ← Map folder
                ├── 成都.wor ← Map info (rect, settings)
                ├── v_000/   ← Region Y = 0
                │   ├── 000_Region_C.dat  ← Region (0,0) client data
                │   ├── 001_Region_C.dat  ← Region (1,0) client data
                │   └── ...
                └── ...
```

---

## 🔧 Region Combined File Format

### Header Structure:
```cpp
struct KCombinFileSection {
    DWORD uOffset;  // Offset from header end
    DWORD uLength;  // Data length
};

// File layout:
// [DWORD: NumSections]
// [Array of KCombinFileSection: Sections info]
// [Obstacle data]
// [Trap data]
// [NPC data]
// [Object data]
// [Ground data]
// [Building data]
```

### Section Indices:
```cpp
#define REGION_OBSTACLE_FILE_INDEX  0  // Obstacle grid (16x32)
#define REGION_TRAP_FILE_INDEX      1  // Trap grid (16x32)
#define REGION_NPC_FILE_INDEX       2  // NPC spawn data
#define REGION_OBJ_FILE_INDEX       3  // Object data
#define REGION_GROUND_FILE_INDEX    4  // Ground texture
#define REGION_BUILDING_FILE_INDEX  5  // Building data
```

---

## 🎯 Tool Nên Làm Gì

### Input Đơn Giản:
1. **Game Folder Path** (Browse button)
   - Server: `D:\HuyenThietKiem\Bin\Server`
   - Client: `D:\HuyenThietKiem\Bin\Client`

2. **Map ID** (Textbox)
   - Ví dụ: `11` (Thành Đô), `21`, `74`, etc.

### Auto Process:
```
1. Đọc <GameFolder>\Settings\MapList.ini
   → Lấy map folder path (ví dụ: "场景地图\城市\成都")

2. Load <GameFolder>\maps\<mapfolder>\<mapname>.wor
   → Lấy rect (region grid bounds)
   → Biết map có bao nhiêu regions (width x height)

3. For each region (X, Y) trong grid:
   → Load <GameFolder>\maps\<mapfolder>\v_Y\X_Region_C.dat (client)
   → hoặc <GameFolder>\maps\<mapfolder>\v_Y\X_Region_S.dat (server)
   → Parse combined file format
   → Extract Obstacle grid (16x32 cells)
   → Extract Trap grid (16x32 cells) nếu cần

4. Render map:
   → Mỗi region = 16x32 cells
   → Mỗi cell = 32x32 pixels
   → Show grid visual
   → Highlight obstacles (red)
   → Highlight traps (yellow)

5. Click on map:
   → Calculate World X, Y
   → Calculate Region X, Y
   → Calculate Cell X, Y
   → Calculate RegionID = MAKELONG(RegionX, RegionY)
   → Display coordinates

6. Export:
   → Generate trap entry format:
   MapId\tRegionId\tCellX\tCellY\tScriptFile\tIsLoad
```

---

## ✅ Ví dụ Cụ Thể

### Map 11 (Thành Đô):

**Step 1: Read MapList.ini**
```ini
[List]
11=场景地图\城市\成都
11_name=Thành Đô
```
→ Map folder = `场景地图\城市\成都`

**Step 2: Read .wor file**
```
File: Bin\Server\maps\场景地图\城市\成都\成都.wor

[MAIN]
rect=0,0,3,3    ← Region grid: 4x4 (0-3, 0-3)
```
→ Map có 4x4 = 16 regions

**Step 3: Load regions**
```
Region (0,0): Bin\Server\maps\场景地图\城市\成都\v_000\000_Region_S.dat
Region (1,0): Bin\Server\maps\场景地图\城市\成都\v_000\001_Region_S.dat
Region (0,1): Bin\Server\maps\场景地图\城市\成都\v_001\000_Region_S.dat
...
Region (3,3): Bin\Server\maps\场景地图\城市\成都\v_003\003_Region_S.dat
```

**Step 4: Parse each region file**
```cpp
// File format:
[DWORD: 6]  // Number of sections
[KCombinFileSection: Obstacle]  { uOffset=0,    uLength=1024 }
[KCombinFileSection: Trap]      { uOffset=1024, uLength=1024 }
[KCombinFileSection: NPC]       { uOffset=2048, uLength=500 }
[KCombinFileSection: Object]    { uOffset=2548, uLength=300 }
[KCombinFileSection: Ground]    { uOffset=2848, uLength=2048 }
[KCombinFileSection: Building]  { uOffset=4896, uLength=1000 }
[Obstacle grid data: 16x32 bytes = 512 bytes]
[Trap grid data: 16x32 bytes = 512 bytes]
[NPC data: ...]
[Object data: ...]
[Ground data: ...]
[Building data: ...]
```

**Step 5: User clicks on map**
```
User clicks at: Screen position (250, 180)
→ Convert to World: (5000, 10000)
→ Calculate:
   RegionX = 5000 / 512 = 9
   RegionY = 10000 / 1024 = 9
   CellX = (5000 % 512) / 32 = 12
   CellY = (10000 % 1024) / 32 = 24
   RegionID = MAKELONG(9, 9) = 589833

→ Display:
   World: (5000, 10000)
   Region: (9, 9)
   RegionID: 589833
   Cell: (12, 24)

→ Generate trap entry:
   11\t589833\t12\t24\t\script\maps\trap\11\1.lua\t1
```

---

## 🚀 Tool Implementation Plan

### UI Simple:
```
┌─────────────────────────────────────────┐
│ Map Coordinate Tool                      │
├─────────────────────────────────────────┤
│ Game Folder: [___________________] [📁]  │  ← Browse to Bin/Server hoặc Bin/Client
│ Map ID:      [11_] [Load Map]            │  ← Nhập ID, click Load
├─────────────────────────────────────────┤
│ Map Info:                                │
│  Name: Thành Đô                          │
│  Folder: 场景地图\城市\成都              │
│  Regions: 4x4 (16 total)                 │
├─────────────────────────────────────────┤
│ ┌─────────────────────────────────────┐ │
│ │                                     │ │
│ │      [Visual Map Grid]              │ │  ← Render regions & cells
│ │                                     │ │  ← Red = obstacles
│ │                                     │ │  ← Yellow = traps
│ │                                     │ │  ← Green = selected
│ └─────────────────────────────────────┘ │
│ Coordinates:                             │
│  World: (5000, 10000)                    │
│  Region: (9, 9) [RegionID: 589833]       │
│  Cell: (12, 24)                          │
├─────────────────────────────────────────┤
│ [Add to Trap List] [Export to File]     │
└─────────────────────────────────────────┘
```

### Code Logic:
```csharp
class SimpleMapTool
{
    string gameFolderPath;  // D:\HuyenThietKiem\Bin\Server
    int mapId;

    void LoadMap()
    {
        // 1. Read MapList.ini
        string mapListPath = Path.Combine(gameFolderPath, "Settings", "MapList.ini");
        IniFile ini = new IniFile(mapListPath);
        string mapFolder = ini.GetString("List", mapId.ToString(), "");

        // 2. Load .wor file
        string worPath = Path.Combine(gameFolderPath, "maps", mapFolder, GetMapName(mapFolder) + ".wor");
        IniFile wor = new IniFile(worPath);
        RECT rect = wor.GetRect("MAIN", "rect");

        // 3. Load all regions
        for (int y = rect.top; y <= rect.bottom; y++)
        {
            for (int x = rect.left; x <= rect.right; x++)
            {
                string regionFile = Path.Combine(
                    gameFolderPath, "maps", mapFolder,
                    $"v_{y:D3}", $"{x:D3}_Region_S.dat"
                );
                LoadRegionFile(regionFile, x, y);
            }
        }

        // 4. Render map
        RenderMap();
    }

    void LoadRegionFile(string path, int x, int y)
    {
        using (BinaryReader reader = new BinaryReader(File.OpenRead(path)))
        {
            // Read header
            uint numSections = reader.ReadUInt32();
            KCombinFileSection[] sections = new KCombinFileSection[numSections];
            for (int i = 0; i < numSections; i++)
            {
                sections[i].uOffset = reader.ReadUInt32();
                sections[i].uLength = reader.ReadUInt32();
            }

            // Read obstacle grid
            reader.BaseStream.Seek(headerSize + sections[0].uOffset, SeekOrigin.Begin);
            byte[,] obstacles = new byte[16, 32];
            for (int cy = 0; cy < 32; cy++)
                for (int cx = 0; cx < 16; cx++)
                    obstacles[cx, cy] = reader.ReadByte();

            // Read trap grid if exists
            if (numSections > 1)
            {
                reader.BaseStream.Seek(headerSize + sections[1].uOffset, SeekOrigin.Begin);
                byte[,] traps = new byte[16, 32];
                for (int cy = 0; cy < 32; cy++)
                    for (int cx = 0; cx < 16; cx++)
                        traps[cx, cy] = reader.ReadByte();
            }

            // Store region data
            regions[x, y] = new RegionData { obstacles, traps };
        }
    }
}
```

---

## 💡 Điểm Khác Biệt So Với Tool Cũ

| Feature | Tool Cũ | Tool Mới |
|---------|---------|----------|
| **Input** | Manual load .wor, manual load regions | Chỉ cần: Game folder + Map ID |
| **Auto-load** | ❌ User phải tự chọn files | ✅ Tool tự động load tất cả |
| **MapList.ini** | ❌ Không dùng | ✅ Đọc tự động |
| **Region files** | ❌ User phải biết path | ✅ Tool tự tìm |
| **Workflow** | Phức tạp, manual | Đơn giản như game |
| **Error-prone** | ✅ Dễ sai path | ❌ Auto validation |

---

## 🎯 Tóm tắt

**Tool cần:**
1. Browse đến `Bin/Server` hoặc `Bin/Client`
2. Nhập Map ID
3. Click "Load Map"
4. Tool TỰ ĐỘNG:
   - Đọc MapList.ini
   - Load .wor file
   - Load TẤT CẢ region files
   - Parse data
   - Render map
5. User click trên map → lấy coordinates
6. Export trap entries

**Đơn giản như vậy thôi!** 🎉
