# PAK Index Generator

Công cụ tự động tạo file `.pak.txt` index cho PAK files bằng cách scan folders và match file paths với hashes trong PAK.

## Vấn Đề

PAK files chỉ lưu **hash ID** của filenames, không lưu tên file trực tiếp:
- PAK chứa: `0xDF3DE3EF` → file data
- Không biết `0xDF3DE3EF` là file gì!

Để biết tên file cần file **`.pak.txt`** index:
```
Index   ID          Time    FileName                        Size    CompressedSize  Ratio
0       DF3DE3EF    0       \spr\npc\001.spr               12345   6789            55.0%
```

## Giải Pháp

Tool này sẽ:
1. **Scan** tất cả files trong Client/Server folders
2. **Tính hash** cho mỗi file path (giống như PAK)
3. **Match** hash với PAK index
4. **Generate** file `.pak.txt` với filename mappings

## Cách Sử Dụng

### Build Tool
```bash
cd SwordOnline/Sources/Tool/PakIndexGenerator
dotnet build
```

### Chạy Tool

**Cú pháp:**
```bash
PakIndexGenerator.exe <pak-file> <scan-folder>
```

**Ví dụ 1: Generate index cho jxhangnga1.pak**
```bash
cd Bin\Server
PakIndexGenerator.exe ..\Client\data\jxhangnga1.pak ..\Client

# Output: ..\Client\data\jxhangnga1.pak.txt
```

**Ví dụ 2: Generate index cho settings.pak**
```bash
PakIndexGenerator.exe ..\Client\data\settings.pak ..\Client

# Output: ..\Client\data\settings.pak.txt
```

**Ví dụ 3: Generate cho tất cả PAK files**
```bash
# Batch script để process tất cả PAK files
for %%F in (..\Client\data\*.pak) do (
    if not exist "%%F.txt" (
        PakIndexGenerator.exe "%%F" "..\Client"
    )
)
```

## Output

Tool sẽ tạo file `.pak.txt` với format:

```
Total Files: 1234
Index	ID	Time	FileName	Size	CompressedSize	Ratio
0	000A1B2C	0	\spr\npc\001.spr	0	0	0.00%
1	000D4E5F	0	\spr\npc\002.spr	0	0	0.00%
2	001G6H7I	0	\maps\11\24.jpg	0	0	0.00%
...
```

## Output Example

```
═══════════════════════════════════════════════════════
  PAK Index Generator - Huyền Thiết Kiếm
  Generates .pak.txt index files by scanning folders
═══════════════════════════════════════════════════════

📂 PAK File: jxhangnga1.pak
📁 Scan Folder: C:\Game\Bin\Client

Step 1: Loading PAK file...
  ✓ Loaded PAK: 1234 files, 456.7 MB
  ✓ Found 1234 file hashes in PAK

Step 2: Scanning folder for files...
  ✓ Found 5678 files in folder

Step 3: Matching file paths with PAK hashes...
  ✓ Matched: 987/1234 files (80.0%)

Step 4: Generating .pak.txt index file...
  ✓ Generated: jxhangnga1.pak.txt
  ✓ Contains 987 filename mappings

═══════════════════════════════════════════════════════
✓ Index generation complete!
  Total PAK files: 1234
  Matched files: 987
  Unmatched files: 247
  Match rate: 80.0%
═══════════════════════════════════════════════════════
```

## Sau Khi Generate Index

Sau khi có file `.pak.txt`, các tools khác sẽ tự động load:

**PakExtractTool:**
```bash
# Giờ sẽ thấy tên files thay vì chỉ hash!
PakExtractTool.exe
# Open: jxhangnga1.pak
# TreeView sẽ hiện: \spr\npc\001.spr, \maps\11\24.jpg, etc.
```

**MapTool:**
```csharp
// PakFileReader tự động load .pak.txt
var reader = new PakFileReader("jxhangnga1.pak");
var files = reader.GetAllFileNames(); // Có tên files!
```

## Tips

### Tăng Match Rate

Nếu match rate thấp, thử:

1. **Scan nhiều folders hơn:**
```bash
# Scan cả Client và Server
PakIndexGenerator.exe jxhangnga1.pak ..\

# Hoặc scan toàn bộ game folder
PakIndexGenerator.exe jxhangnga1.pak C:\Game
```

2. **Check path format:**
   - PAK lưu paths với backslash: `\spr\npc\001.spr`
   - Tool tự động normalize, nhưng check log nếu có vấn đề

3. **Xem unmatched hashes:**
   - Unmatched files có thể là:
     - Files đã bị xóa khỏi disk
     - Files với tên đã đổi
     - Files ở folder khác

### Performance

- **Scan speed:** ~10,000 files/giây
- **Hash calculation:** ~50,000 paths/giây
- **Typical time:** 1-2 phút cho 1 PAK file

### Troubleshooting

**"Matched: 0/1234 files (0.0%)"**
- Check scan folder có đúng không
- PAK files có thể chứa files từ nhiều folders khác nhau
- Thử scan parent folder cao hơn

**"Error: PAK file not found"**
- Check đường dẫn PAK file
- Dùng absolute path nếu cần

**Encoding errors**
- Tool dùng GB2312 encoding cho Chinese filenames
- Nếu có lỗi encoding, check file paths

## Technical Details

### Hash Algorithm

Tool dùng **FileNameHasher.CalculateFileId()** (từ MapTool):
```csharp
uint hash = FileNameHasher.CalculateFileId(@"\spr\npc\001.spr");
// Returns: 0xDF3DE3EF
```

Algorithm:
1. Convert filename to GB2312 bytes
2. Calculate hash: `Id = (Id + (i + 1) * c) % 0x8000000b * 0xffffffef`
3. XOR with magic: `0x12345678`

### File Format

`.pak.txt` format:
```
Line 1: Total Files: <count>
Line 2: Index\tID\tTime\tFileName\tSize\tCompressedSize\tRatio
Line 3+: <data rows>
```

Encoding: **GB2312** (for Chinese characters)

## See Also

- **PakExtractTool** - Extract files from PAK with GUI
- **MapTool** - View and export map data
- **FileNameHasher** - Calculate PAK file hashes
