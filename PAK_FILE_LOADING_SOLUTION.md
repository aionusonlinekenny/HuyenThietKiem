# Giải pháp đọc map files từ pak

## Vấn đề hiện tại:

### 1. Encoding:
- Game dùng **ANSI (GB2312)** encoding cho tất cả file paths
- Tool của tôi đang dùng UTF-8/Unicode
- Khi đọc `maps.pak.txt`, Chinese characters bị garbled

### 2. File path hashing:
Game sử dụng hash function để convert filename → ID:

```cpp
ENGINE_API DWORD g_FileName2Id(LPSTR lpFileName)
{
    DWORD Id = 0;
    char c = 0;
    for (int i = 0; lpFileName[i]; i++)
    {
        c = lpFileName[i];
        #ifndef WIN32
        if ('/' == c)
            c = '\\';
        #endif
        Id = (Id + (i + 1) * c) % 0x8000000b * 0xffffffef;
    }
    return (Id ^ 0x12345678);
}
```

### 3. Cách game load files:

```cpp
// Step 1: Load từ MapList.ini (ANSI encoding!)
IniFile.GetString("List", szKeyName, "", szPathName, sizeof(szPathName));
// szPathName = "场景地图\城市\成都" (ANSI bytes!)

// Step 2: Load .wor file
g_SetFilePath("\\maps");  // Set base path
sprintf(szFileName, "%s.wor", szPathName);
// szFileName = "场景地图\城市\成都.wor"

// Step 3: KPakFile::Open() logic
// 3a. Try open file on disk first
bOk = m_File.Open(szFileName);
// 3b. If not found on disk, search in pak
if (!bOk)
    bOk = g_pPakList->FindElemFile(pszFileName, m_PackRef);
    // FindElemFile uses: uId = g_FileName2Id(pszFileName)
```

## Phát hiện quan trọng:

Trong pak file, có 2 loại paths:

1. **2 cấp**: `特殊用地\剑塔战场.wor`
   → Pak path: `\maps\特殊用地\剑塔战场.wor`

2. **3+ cấp**: `场景地图\城市\成都`
   → Pak path: `\maps\场景地图\城市\成都\成都.wor` (tên cuối lặp lại!)

**Logic**: MapList.ini có thể chứa:
- Full path: `场景地图\城市\成都` (3 parts)
- Game append `.wor` → `场景地图\城市\成都.wor`
- Nhưng trong pak: `场景地图\城市\成都\成都.wor` (có thêm folder!)

**Giả thuyết**:
- Nếu MapList path có 3+ parts → file thực tế trong folder cuối
- Path hashing sẽ match với actual path trong pak

## Giải pháp:

### Option A: Đọc maps.pak.txt với encoding đúng

```csharp
// Đọc maps.pak.txt với GB2312 encoding
var lines = File.ReadAllLines(pakTxtPath, Encoding.GetEncoding("GB2312"));

// Parse và hash filename
foreach (var line in lines)
{
    var parts = line.Split('\t');
    if (parts.Length < 4) continue;

    string filename = parts[3];  // \maps\场景地图\城市\成都\成都.wor

    // Convert to ANSI bytes
    byte[] ansiBytes = Encoding.GetEncoding("GB2312").GetBytes(filename);

    // Calculate hash (port g_FileName2Id)
    uint id = CalculateFileId(ansiBytes);

    // Store in dictionary
    _fileIndex[id] = new PakIndexEntry { ... };
}
```

### Option B: Sử dụng logic game để build path

Thay vì đọc từ maps.pak.txt, tự build paths như game:

```csharp
// From MapList entry: "场景地图\城市\成都"
string mapPath = mapEntry.FolderPath;
string[] parts = mapPath.Split('\\', '/');

// Quy luật:
if (parts.Length >= 3)
{
    // 3+ cấp: thêm folder cuối
    string mapName = parts[parts.Length - 1];
    worPath = $"\\maps\\{mapPath}\\{mapName}.wor";
    regionBasePath = $"\\maps\\{mapPath}";
}
else
{
    // 2 cấp: không thêm folder
    worPath = $"\\maps\\{mapPath}.wor";
    regionBasePath = $"\\maps\\{mapPath}";
}

// Convert path to ANSI bytes
byte[] ansiBytes = Encoding.GetEncoding("GB2312").GetBytes(worPath);

// Hash to get ID
uint fileId = CalculateFileId(ansiBytes);

// Look up in pak index
if (_pakIndex.ContainsKey(fileId))
{
    // Read from pak
}
```

### Option C: Implement g_FileName2Id trong C#

```csharp
public static uint CalculateFileId(byte[] ansiFilename)
{
    uint id = 0;
    for (int i = 0; i < ansiFilename.Length; i++)
    {
        byte c = ansiFilename[i];

        // Convert / to \\ (if needed)
        if (c == (byte)'/')
            c = (byte)'\\\\';

        id = (id + (uint)(i + 1) * c) % 0x8000000b * 0xffffffef;
    }
    return id ^ 0x12345678;
}
```

## Khuyến nghị:

**Làm theo Option A + C:**

1. **Đọc maps.pak.txt với GB2312** encoding
2. **Parse index table** với paths đúng (không bị garbled)
3. **Implement hash function** chính xác như game
4. **Store filename → hash mapping**
5. **Lookup files** bằng hash ID

Điều này sẽ match exactly với cách game làm!

## Testing:

Sau khi implement, test với:
1. Map ID 1 (Phượng Tường) - 2-level path
2. Map ID 11 (Thành Đô) - 3-level path
3. Verify paths match với pak file

## Lưu ý quan trọng:

⚠️ **ANSI encoding is critical!**
- Tất cả file operations trong game dùng ANSI
- Hash function hoạt động trên ANSI bytes, không phải Unicode
- Pak file index được build từ ANSI filenames
- Tool PHẢI dùng GB2312/ANSI để match game logic

Nếu dùng UTF-8/Unicode, hash sẽ khác → không tìm thấy file!
