# Test Pak File Hash Function

## Test với dữ liệu thực từ maps.pak.txt:

### Sample 1:
```
File: \maps\场景地图\城市\成都\v_095\094_region_s.dat
ID từ pak.txt: 33 (hex: 0x21) → decimal: 33
```

### Sample 2:
```
File: \maps\江南区\西山群岛\v_083\101_region_s.dat
ID từ pak.txt: 119 (decimal)
```

### Sample 3:
```
File: \maps\特殊用地\剑塔战场.wor
ID từ pak.txt: 590 (decimal)
```

## Cách test:

```csharp
using MapTool.PakFile;

// Test 1: Hash một filename
string filename = "\\maps\\场景地图\\城市\\成都\\成都.wor";
uint hashId = FileNameHasher.CalculateFileId(filename);
Console.WriteLine($"Hash of '{filename}': 0x{hashId:X8}");

// Test 2: Compare với ID từ pak.txt
// Nếu hash đúng, hashId phải match với ID trong index table
```

## Expected behavior:

Khi PakFileReader load:
1. Đọc maps.pak.txt với GB2312 encoding ✓
2. Parse ra filename và ID từ file
3. Khi lookup file, nếu không có trong _nameToId:
   - Calculate hash từ filename
   - So sánh với IDs trong _fileIndex
   - Nếu match → file found!

## Debug output cần có:

```
✓ Opened pak file: D:\...\maps.pak
✓ Pak contains 87245 files
✓ Loaded 87245 filename mappings from maps.pak.txt
→ Looking for file: \maps\场景地图\城市\成都\成都.wor
  - Direct lookup: not found
  - Calculated hash: 0x12AB34CD
  - Hash found in index: YES
✓ File found by hash!
```

## Lưu ý:

Hash function **PHẢI** dùng ANSI bytes (GB2312), không phải UTF-8!

```csharp
// ĐÚNG:
byte[] ansiBytes = Encoding.GetEncoding("GB2312").GetBytes(filename);
uint hash = CalculateFileIdFromBytes(ansiBytes);

// SAI:
byte[] utf8Bytes = Encoding.UTF8.GetBytes(filename);  // ← SAI!
```

Chinese characters trong GB2312 có byte values khác với UTF-8.
Ví dụ:
- "成" trong GB2312: 0xB3 0xC9 (2 bytes)
- "成" trong UTF-8: 0xE6 0x88 0x90 (3 bytes)

→ Hash sẽ khác hoàn toàn!
