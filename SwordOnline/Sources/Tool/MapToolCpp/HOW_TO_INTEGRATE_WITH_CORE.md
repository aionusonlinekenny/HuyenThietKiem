# Hướng dẫn tạo C++ MapTool integrate với Core library

## Vấn đề hiện tại

Bạn muốn tạo MapTool bằng C++ để tận dụng code có sẵn trong project (KSubWorld, KRegion, etc.).

Hiện có 3 options:

### Option 1: Python Tools ✅ (Đã hoàn thành)
- Location: `tools/` directory
- Scripts: `analyze_map.py`, `parse_obj_npc_files.py`, `scan_region_files.py`
- **Pros:** Hoàn chỉnh, dễ dùng, nhiều features
- **Cons:** Python scripts, không có UI

### Option 2: C# MapTool ✅ (Đã hoàn thành)
- Location: `SwordOnline/Sources/Tool/MapTool/`
- **Pros:** UI đẹp, đầy đủ features, visual map renderer
- **Cons:** Cần .NET Framework, không tích hợp với Core C++ code

### Option 3: C++ Integrated Tool ⏸️ (Cần thời gian)
- **Ưu điểm:**
  - Tích hợp trực tiếp với KSubWorld, KRegion classes
  - Reuse toàn bộ map loading logic
  - Native performance
  - Có thể extend game engine sau này

- **Nhược điểm:**
  - Phức tạp hơn nhiều
  - Cần setup dependencies
  - Cần understand game engine architecture
  - Thời gian develop lâu

---

## Cách integrate với Core library (cho Option 3)

Nếu bạn muốn tạo C++ tool integrate với Core, follow các bước sau:

### Bước 1: Understand Project Structure

```
SwordOnline/Sources/
├── Core/               # Game engine core
│   ├── Src/
│   │   ├── KSubWorld.cpp/h
│   │   ├── KRegion.cpp/h
│   │   ├── KLittleMap.cpp/h
│   │   └── Scene/
│   │       ├── KScenePlaceC.cpp/h
│   │       └── SceneDataDef.h
│   └── [Build output: Core.lib or Core.dll]
│
└── Tool/
    └── MapToolCpp/     # Tool mới cần tạo
```

### Bước 2: Setup Visual C++ Project

**Tạo file MapToolCpp.dsp:**

```dsp
# Microsoft Developer Studio Project File
# TARGTYPE "Win32 (x86) Application" 0x0101

# Begin Project
CPP=cl.exe
LINK32=link.exe

# Include paths
# ADD CPP /I"../../Core/Src"
# ADD CPP /I"../../Core/Src/Scene"

# Library paths và dependencies
# ADD LINK32 ../../Core/Release/Core.lib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib

# Source files
SOURCE=.\Main.cpp
SOURCE=.\MapToolUI.cpp
# End Project
```

### Bước 3: Create Main Classes

**File: MapToolApp.h**

```cpp
#include "KSubWorld.h"
#include "KRegion.h"
#include "Scene/KScenePlaceC.h"

class CMapToolApp
{
private:
    KSubWorld* m_pSubWorld;
    int m_nCurrentMapId;

public:
    CMapToolApp();
    ~CMapToolApp();

    // Load map using existing engine code
    bool LoadMap(int nMapId, const char* szMapPath);

    // Get region data
    KRegion* GetRegion(int nRegionX, int nRegionY);

    // Coordinate conversions (using KSubWorld methods)
    void WorldToRegionCell(int worldX, int worldY,
                          int& regionX, int& regionY,
                          int& cellX, int& cellY);

    void RegionCellToWorld(int regionX, int regionY,
                          int cellX, int cellY,
                          int& worldX, int& worldY);
};
```

**File: MapToolApp.cpp**

```cpp
#include "MapToolApp.h"

CMapToolApp::CMapToolApp()
{
    m_pSubWorld = new KSubWorld();
    m_nCurrentMapId = 0;
}

CMapToolApp::~CMapToolApp()
{
    if (m_pSubWorld)
    {
        delete m_pSubWorld;
        m_pSubWorld = NULL;
    }
}

bool CMapToolApp::LoadMap(int nMapId, const char* szMapPath)
{
    m_nCurrentMapId = nMapId;

    // Use KSubWorld::LoadMap directly!
    if (m_pSubWorld->LoadMap(szMapPath, nMapId))
    {
        return true;
    }
    return false;
}

KRegion* CMapToolApp::GetRegion(int nRegionX, int nRegionY)
{
    int nRegionID = MAKELONG(nRegionX, nRegionY);
    int nIndex = m_pSubWorld->FindRegion(nRegionID);

    if (nIndex >= 0)
    {
        return &m_pSubWorld->m_Region[nIndex];
    }
    return NULL;
}

void CMapToolApp::WorldToRegionCell(int worldX, int worldY,
                                    int& regionX, int& regionY,
                                    int& cellX, int& cellY)
{
    int nR, nX, nY, nDx, nDy;

    // Use KSubWorld::Mps2Map directly!
    m_pSubWorld->Mps2Map(worldX, worldY, &nR, &nX, &nY, &nDx, &nDy);

    // Get region coordinates from region index
    if (nR >= 0)
    {
        int nRegionID = m_pSubWorld->m_Region[nR].m_RegionID;
        regionX = LOWORD(nRegionID);
        regionY = HIWORD(nRegionID);
        cellX = nX;
        cellY = nY;
    }
}

void CMapToolApp::RegionCellToWorld(int regionX, int regionY,
                                    int cellX, int cellY,
                                    int& worldX, int& worldY)
{
    // Use static KSubWorld::Map2Mps variant
    KSubWorld::Map2Mps(regionX, regionY, cellX, cellY, 0, 0,
                       &worldX, &worldY);
}
```

### Bước 4: Create Win32 UI

**Simple Win32 Window với GDI:**

```cpp
#include <windows.h>
#include "MapToolApp.h"

CMapToolApp* g_pApp = NULL;

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
            g_pApp = new CMapToolApp();
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Render map using region data
            RenderMap(hdc, g_pApp);

            EndPaint(hwnd, &ps);
            break;
        }

        case WM_LBUTTONDOWN:
        {
            int screenX = LOWORD(lParam);
            int screenY = HIWORD(lParam);

            // Convert screen → world → region/cell
            int worldX, worldY;
            ScreenToWorld(screenX, screenY, worldX, worldY);

            int regionX, regionY, cellX, cellY;
            g_pApp->WorldToRegionCell(worldX, worldY,
                                     regionX, regionY,
                                     cellX, cellY);

            // Display or export
            char buf[256];
            sprintf(buf, "Region(%d,%d) Cell(%d,%d)",
                   regionX, regionY, cellX, cellY);
            MessageBox(hwnd, buf, "Coordinates", MB_OK);
            break;
        }

        case WM_DESTROY:
            if (g_pApp)
            {
                delete g_pApp;
                g_pApp = NULL;
            }
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void RenderMap(HDC hdc, CMapToolApp* pApp)
{
    // Get region data
    for (int ry = 0; ry < 10; ry++)
    {
        for (int rx = 0; rx < 10; rx++)
        {
            KRegion* pRegion = pApp->GetRegion(rx, ry);
            if (!pRegion)
                continue;

            // Render region grid
            for (int cy = 0; cy < REGION_GRID_HEIGHT; cy++)
            {
                for (int cx = 0; cx < REGION_GRID_WIDTH; cx++)
                {
                    // Draw cell
                    int screenX = rx * 256 + cx * 16;
                    int screenY = ry * 512 + cy * 16;

                    // Check obstacle
                    if (pRegion->m_Obstacle[cx][cy] != 0)
                    {
                        // Draw red for obstacle
                        HBRUSH hBrush = CreateSolidBrush(RGB(255,0,0));
                        RECT rect = {screenX, screenY, screenX+16, screenY+16};
                        FillRect(hdc, &rect, hBrush);
                        DeleteObject(hBrush);
                    }

                    // Draw grid
                    MoveToEx(hdc, screenX, screenY, NULL);
                    LineTo(hdc, screenX+16, screenY);
                    LineTo(hdc, screenX+16, screenY+16);
                    LineTo(hdc, screenX, screenY+16);
                    LineTo(hdc, screenX, screenY);
                }
            }
        }
    }
}
```

### Bước 5: Build & Link

**Compile commands:**

```batch
REM Compile
cl.exe /c /I"..\..\Core\Src" MapToolApp.cpp Main.cpp

REM Link
link.exe MapToolApp.obj Main.obj /LIBPATH:"..\..\Core\Release" Core.lib kernel32.lib user32.lib gdi32.lib /OUT:MapTool.exe
```

**Hoặc dùng Visual Studio:**
- Project → Properties
- C/C++ → General → Additional Include Directories: `../../Core/Src`
- Linker → Input → Additional Dependencies: `Core.lib`
- Linker → General → Additional Library Directories: `../../Core/Release`

---

## Challenges & Solutions

### Challenge 1: Core.lib chưa được build

**Solution:**
- Build Core project trước
- Hoặc extract code cần thiết ra standalone

### Challenge 2: Dependencies phức tạp

**Solution:**
- KSubWorld depends on nhiều classes khác
- Có thể cần copy nhiều files
- Hoặc chỉ dùng coordinate conversion logic (đơn giản hơn)

### Challenge 3: Header conflicts

**Solution:**
- Use proper include guards
- Namespace nếu cần
- Minimal includes

---

## Recommendation

Vì complexity cao, tôi khuyến nghị:

**Cho việc sử dụng ngay:**
→ Dùng **C# MapTool** (đã hoàn chỉnh trong `Tool/MapTool/`)
  - Visual UI
  - Đầy đủ features
  - Build và run ngay

**Cho việc integrate engine:**
→ Làm từng bước:
  1. Dùng C# tool để familiar với workflow
  2. Sau đó tạo C++ version khi cần customize sâu
  3. Integrate với Core library khi muốn extend game engine

**Cho việc script automation:**
→ Dùng **Python tools** (đã có trong `tools/`)
  - Batch processing
  - Command-line friendly
  - Easy to extend

---

## Tóm lại

Bạn đã có **3 công cụ hoàn chỉnh**:

1. ✅ **Python Tools** - Command-line, batch processing
2. ✅ **C# MapTool** - Visual UI, interactive
3. 📝 **C++ Integration Guide** - Hướng dẫn tích hợp với Core (document này)

Nếu bạn vẫn muốn tool C++ integrated, follow guide trên và cho tôi biết nếu gặp vấn đề!
