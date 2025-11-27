# Vấn Đề: Map Files Trong PAK File

## 🔍 Phân Tích Vấn Đề

### Tool Hiện Tại
```csharp
// MapLoader.cs - Line 84
string regionPath = Path.Combine(
    mapFolderPath,
    $"v_{y:D3}",
    $"{x:D3}_{regionSuffix}"
);

if (File.Exists(regionPath))  // ❌ Luôn return FALSE!
{
    // Never executes vì file nằm trong maps.pak
}
```

### Tại Sao Không Load Được?

**Bin/Server/maps/** chỉ có vài files lẻ:
```
Bin/Server/maps/
├── WorldSet.ini
├── WorldSet1.ini
└── (một vài thư mục map cũ)
```

**Phần lớn map files** nằm TRONG **maps.pak**:
```
Bin/Server/pak/maps.pak  (14 MB)
├── \maps\场景地图\城市\成都\成都.wor
├── \maps\场景地图\城市\成都\v_000\000_region_s.dat
├── \maps\场景地图\城市\成都\v_000\001_region_s.dat
└── ... (87,245 files total!)
```

---

## 📊 Cách Game Load Map

### Game Engine Workflow:

```cpp
// 1. KPakFile::Open() tự động tìm file
KPakFile file;
file.Open("\\maps\\场景地图\\城市\\成都\\成都.wor");

// 2. Hệ thống tự động check:
if (g_pPakList->FindElemFile(filename, pakRef))
{
    // Tìm thấy trong maps.pak → Đọc từ pak
    ReadFromPak(pakRef);
}
else
{
    // Không có trong pak → Đọc từ disk
    ReadFromDisk(filename);
}
```

### KPakFile System:
- **KPakList**: Quản lý danh sách .pak files
- **XPackFile**: Class đọc file từ .pak
- **XPackElemFileRef**: Reference đến file trong pak
  - `uId`: File ID (hash)
  - `nPackIndex`: Pak file index
  - `nElemIndex`: File index trong pak
  - `nOffset`: Offset trong pak
  - `nSize`: File size

### Pak File Structure:

```
maps.pak (Binary format)
├── Header
│   ├── Magic number
│   ├── File count: 87245
│   └── Index table offset
├── Index Table
│   ├── File 0: ID=3431, Offset=..., Size=2295
│   ├── File 1: ID=8f9f, Offset=..., Size=2100
│   └── ...
└── Data Section
    ├── File 0 data (compressed/uncompressed)
    ├── File 1 data
    └── ...
```

### maps.pak.txt (Index List):
```
TotalFile:87245	PakTime:2011-12-8 10:57:19
Index	ID	Time	FileName	Size	InPakSize	ComprFlag	CRC
0	3431	...	\maps\场景地图\城市\成都\v_095\094_region_s.dat	2295	274	4	7e9aba3
1	8f9f	...	\maps\场景地图\城市\成都\v_083\101_region_s.dat	2100	132	4	c1f9e9ec
...
```

---

## 💡 Giải Pháp

### ⭐ Option 1: Unpack Maps.pak (Khuyến Nghị - Nhanh Nhất)

**Ưu điểm:**
- ✅ Đơn giản, không cần sửa code
- ✅ Tool C# hiện tại hoạt động NGAY
- ✅ Dễ debug, dễ xem files

**Nhược điểm:**
- ❌ Tốn disk space (~14 MB unpacked)
- ❌ Phải unpack một lần

**Cách làm:**

#### Bước 1: Tạo Unpack Tool

```cpp
// UnpackMapsPak.cpp - Tool đơn giản để unpack
#include "XPackFile.h"

int main()
{
    XPackFile pak;
    pak.Open("Bin/Server/pak/maps.pak", 0);

    // Đọc maps.pak.txt để lấy file list
    FILE* indexFile = fopen("Bin/Server/pak/maps.pak.txt", "r");
    char line[1024];
    fgets(line, sizeof(line), indexFile); // Skip header
    fgets(line, sizeof(line), indexFile); // Skip column names

    while (fgets(line, sizeof(line), indexFile))
    {
        unsigned long fileId;
        char fileName[512];
        int size, inPakSize;

        sscanf(line, "%*d\t%lx\t%*s\t%s\t%d\t%d", &fileId, fileName, &size, &inPakSize);

        // Extract file
        XPackElemFileRef ref;
        if (pak.FindElemFile(fileId, ref))
        {
            void* buffer = malloc(size);
            pak.ElemFileRead(ref, buffer, size);

            // Write to disk
            CreateDirectories(fileName);
            FILE* outFile = fopen(fileName, "wb");
            fwrite(buffer, 1, size, outFile);
            fclose(outFile);
            free(buffer);
        }
    }

    fclose(indexFile);
    pak.Close();
    return 0;
}
```

#### Bước 2: Hoặc Dùng Tool Có Sẵn

Nếu project có tool unpack, chạy:
```bash
cd Bin/Server/pak
UnpackTool.exe maps.pak
# hoặc
MapUnpacker.exe maps.pak ../maps/
```

#### Bước 3: Test MapTool

Sau khi unpack, files sẽ ở:
```
Bin/Server/maps/
├── 场景地图/
│   └── 城市/
│       └── 成都/
│           ├── 成都.wor
│           ├── v_000/
│           │   ├── 000_region_s.dat
│           │   └── ...
│           └── ...
```

Tool C# sẽ hoạt động NGAY!

---

### Option 2: Tạo C# Pak Reader

**Ưu điểm:**
- ✅ Không cần unpack
- ✅ Tiết kiệm disk space
- ✅ Load trực tiếp từ pak

**Nhược điểm:**
- ❌ Phức tạp, nhiều code
- ❌ Phải implement pak format
- ❌ Cần test kỹ

**Implementation:**

```csharp
// PakFileReader.cs
public class PakFileReader
{
    private string _pakPath;
    private Dictionary<string, PakFileEntry> _fileIndex;

    public class PakFileEntry
    {
        public uint Id;
        public string FileName;
        public int Offset;
        public int Size;
        public int CompressedSize;
        public int CompressionFlag;
    }

    public PakFileReader(string pakPath)
    {
        _pakPath = pakPath;
        _fileIndex = new Dictionary<string, PakFileEntry>();
        LoadIndex();
    }

    private void LoadIndex()
    {
        // Read .pak.txt index file
        string indexPath = _pakPath + ".txt";
        string[] lines = File.ReadAllLines(indexPath, Encoding.GetEncoding("GB2312"));

        foreach (string line in lines.Skip(2)) // Skip header
        {
            string[] parts = line.Split('\t');
            if (parts.Length < 8) continue;

            var entry = new PakFileEntry
            {
                Id = Convert.ToUInt32(parts[1], 16),
                FileName = parts[3].Trim(),
                Size = int.Parse(parts[4]),
                CompressedSize = int.Parse(parts[5]),
                CompressionFlag = int.Parse(parts[6])
            };

            _fileIndex[entry.FileName.ToLower()] = entry;
        }
    }

    public bool FileExists(string fileName)
    {
        return _fileIndex.ContainsKey(fileName.ToLower());
    }

    public byte[] ReadFile(string fileName)
    {
        if (!_fileIndex.ContainsKey(fileName.ToLower()))
            return null;

        var entry = _fileIndex[fileName.ToLower()];

        using (FileStream pakFile = new FileStream(_pakPath, FileMode.Open, FileAccess.Read))
        {
            // Need to parse pak file header to find actual offset
            // This requires understanding full pak format...
            // TODO: Implement pak file parsing

            // For now, would need offset from actual pak structure
            pakFile.Seek(entry.Offset, SeekOrigin.Begin);

            byte[] compressedData = new byte[entry.CompressedSize];
            pakFile.Read(compressedData, 0, entry.CompressedSize);

            // Decompress if needed
            if (entry.CompressionFlag == 4)
            {
                return Decompress(compressedData, entry.Size);
            }

            return compressedData;
        }
    }

    private byte[] Decompress(byte[] compressed, int uncompressedSize)
    {
        // TODO: Implement decompression
        // Pak uses custom compression (likely UCL or similar)
        throw new NotImplementedException("Decompression not implemented");
    }
}

// Update MapLoader.cs
public class MapLoader
{
    private PakFileReader _pakReader;

    public MapLoader(string gameFolder, bool isServerMode = true)
    {
        // ...

        // Check for pak file
        string pakPath = Path.Combine(gameFolder, "pak", "maps.pak");
        if (File.Exists(pakPath))
        {
            _pakReader = new PakFileReader(pakPath);
        }
    }

    private bool FileExists(string path)
    {
        // Check pak first
        if (_pakReader != null && _pakReader.FileExists(path))
            return true;

        // Then check disk
        return File.Exists(path);
    }

    private byte[] ReadFile(string path)
    {
        // Try pak first
        if (_pakReader != null)
        {
            byte[] data = _pakReader.ReadFile(path);
            if (data != null) return data;
        }

        // Fallback to disk
        if (File.Exists(path))
            return File.ReadAllBytes(path);

        return null;
    }
}
```

**Vấn đề với Option 2:**
- Cần hiểu CHÍNH XÁC pak file binary format
- Cần implement decompression algorithm (UCL/zlib?)
- maps.pak.txt KHÔNG có offset thực, chỉ có metadata
- Phải parse pak file header để tìm offset thật

---

### Option 3: Dùng Engine Library (Ideal nhưng Phức Tạp)

**Link với Engine.dll:**

```csharp
// PInvoke to Engine.dll
[DllImport("Engine.dll", CallingConvention = CallingConvention.Cdecl)]
private static extern void g_SetRootPath(string path);

[DllImport("Engine.dll", CallingConvention = CallingConvention.Cdecl)]
private static extern void g_SetFilePath(string path);

[DllImport("Engine.dll", CallingConvention = CallingConvention.Cdecl)]
private static extern bool KPakFile_Open(IntPtr pakFile, string fileName);

[DllImport("Engine.dll", CallingConvention = CallingConvention.Cdecl)]
private static extern int KPakFile_Read(IntPtr pakFile, byte[] buffer, int size);

// Use native pak file system
public byte[] ReadFileFromPak(string fileName)
{
    g_SetRootPath(gameFolder);
    g_SetFilePath("\\maps");

    IntPtr pakFile = Marshal.AllocHGlobal(1024); // Allocate KPakFile struct

    if (KPakFile_Open(pakFile, fileName))
    {
        int size = KPakFile_Size(pakFile);
        byte[] buffer = new byte[size];
        KPakFile_Read(pakFile, buffer, size);
        KPakFile_Close(pakFile);
        Marshal.FreeHGlobal(pakFile);
        return buffer;
    }

    Marshal.FreeHGlobal(pakFile);
    return null;
}
```

**Vấn đề:**
- Cần build Engine.dll compatible với C#
- Cần hiểu memory layout của KPakFile struct
- Platform dependency (x86 vs x64)

---

## 🎯 Khuyến Nghị

### Giải Pháp Tốt Nhất: **Option 1 - Unpack maps.pak**

**Lý do:**
1. ✅ **Nhanh nhất** - Tool hoạt động ngay sau unpack
2. ✅ **Đơn giản nhất** - Không cần sửa code
3. ✅ **Ít lỗi nhất** - Dùng existing file system API
4. ✅ **Dễ debug** - Xem được files trực tiếp

**Cách thực hiện:**

#### A. Tìm tool unpack trong project:
```bash
cd SwordOnline/Sources/Tool
dir /s UnPack*.exe
dir /s *Unpack*.exe
```

#### B. Hoặc tạo tool unpack đơn giản bằng C++:
```cpp
// QuickUnpack.cpp - Link với Engine.lib
#include "XPackFile.h"
#include <stdio.h>

void UnpackFile(XPackFile& pak, unsigned long fileId, const char* fileName, int size)
{
    XPackElemFileRef ref;
    if (pak.FindElemFile(fileId, ref))
    {
        void* buffer = malloc(size);
        pak.ElemFileRead(ref, buffer, size);

        // Create directories
        char dirPath[512];
        strcpy(dirPath, fileName);
        char* lastSlash = strrchr(dirPath, '\\');
        if (lastSlash)
        {
            *lastSlash = '\0';
            CreateDirectoryRecursive(dirPath);
        }

        // Write file
        FILE* f = fopen(fileName, "wb");
        if (f)
        {
            fwrite(buffer, 1, size, f);
            fclose(f);
            printf("Extracted: %s\\n", fileName);
        }

        free(buffer);
    }
}

int main()
{
    XPackFile pak;
    pak.Open("Bin/Server/pak/maps.pak", 0);

    FILE* idx = fopen("Bin/Server/pak/maps.pak.txt", "r");
    // ... parse and extract all files

    pak.Close();
    return 0;
}
```

#### C. Hoặc unpack manual một số maps cần thiết:

Chỉ extract maps thường dùng (Map 11, 21, etc.)

---

## 📝 Action Items

### Immediate (Để tool hoạt động NGAY):

1. **Check xem có tool unpack không:**
   ```bash
   find SwordOnline/Sources -name "*Unpack*" -o -name "*Extract*"
   ```

2. **Nếu không có, tạo simple unpacker** (30 phút code)

3. **Unpack maps.pak:**
   ```bash
   cd Bin/Server/pak
   UnpackTool.exe maps.pak ../maps/
   ```

4. **Test MapTool** - Sẽ hoạt động ngay!

### Future (Nếu cần optimization):

1. Implement PakFileReader trong C#
2. Support cả pak file và disk files
3. Cache extracted files

---

## 🔧 Code Changes Needed (If Using Pak Reader)

```csharp
// MapLoader.cs - Modifications needed
public class MapLoader
{
    private PakFileReader _pakReader;

    public MapLoader(string gameFolder, bool isServerMode = true)
    {
        _gameFolder = gameFolder;
        _isServerMode = isServerMode;
        _mapListParser = new MapListParser(gameFolder);

        // Initialize pak reader
        string pakPath = Path.Combine(gameFolder, "pak", "maps.pak");
        if (File.Exists(pakPath))
        {
            _pakReader = new PakFileReader(pakPath);
        }
    }

    private bool FileExistsInPakOrDisk(string relativePath)
    {
        // Try pak first
        if (_pakReader != null && _pakReader.FileExists(relativePath))
            return true;

        // Then try disk
        string diskPath = Path.Combine(_gameFolder, relativePath.TrimStart('\\'));
        return File.Exists(diskPath);
    }

    private byte[] ReadFileFromPakOrDisk(string relativePath)
    {
        // Try pak first
        if (_pakReader != null)
        {
            byte[] data = _pakReader.ReadFile(relativePath);
            if (data != null) return data;
        }

        // Fallback to disk
        string diskPath = Path.Combine(_gameFolder, relativePath.TrimStart('\\'));
        if (File.Exists(diskPath))
            return File.ReadAllBytes(diskPath);

        return null;
    }
}
```

---

## ✅ Summary

| Solution | Time | Complexity | Recommended |
|----------|------|------------|-------------|
| **Option 1: Unpack** | 10 min | ⭐ Low | ✅ YES |
| **Option 2: C# Reader** | 4+ hours | ⭐⭐⭐⭐ High | ❌ No |
| **Option 3: PInvoke** | 2+ hours | ⭐⭐⭐ Medium | 🟡 Maybe |

**Best Choice: Option 1 - Unpack maps.pak**

Bạn muốn tôi giúp tạo unpacker tool không?
