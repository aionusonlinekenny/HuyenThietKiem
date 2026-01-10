# PakExtractTool - PAK File Browser & Extractor

## Tổng quan

Tool GUI Windows Forms để browse và extract files từ PAK files của Huyền Thiết Kiếm.

## Tính năng

### 📂 Browse Files trong PAK
- **Tree view hierarchical**: Hiển thị cấu trúc thư mục và files
- **File details panel**: Xem thông tin chi tiết của mỗi file:
  - File ID (hash)
  - Size (original và compressed)
  - Compression method và ratio

### ✅ Selective Extraction
- **Checkbox selection**: Check/uncheck files và folders để chọn
- **Smart parent/child selection**: Check folder → auto-check tất cả files bên trong
- **Extract selected**: Chỉ extract files đã được check
- **Extract all**: Extract toàn bộ PAK

### 📊 File Information
- Tổng số files trong PAK
- File có tên (từ .pak.txt) và unknown files
- Size thống kê (compressed vs uncompressed)
- Compression ratio

## Cách sử dụng

### 1. Open PAK File

```
1. Click "📂 Open PAK File"
2. Browse đến file PAK (ví dụ: Bin/Server/pak/maps.pak)
3. Tool sẽ load và hiển thị tree structure
```

### 2. Browse Files

```
- Click vào folder/file trong tree để xem details
- Expand/collapse folders bằng [+] [-]
- Scroll để xem tất cả files
```

### 3. Select Files để Extract

**Option A: Extract specific files**
```
1. Check các files/folders muốn extract
2. Click "📤 Extract Selected (X)"
3. Chọn output folder
4. Chờ extraction complete
```

**Option B: Extract toàn bộ**
```
1. Click "📦 Extract All"
2. Chọn output folder
3. Chờ extraction complete
```

## UI Layout

```
┌─────────────────────────────────────────────────────────┐
│  [📂 Open PAK] [📤 Extract Selected] [📦 Extract All]   │
│  📦 maps.pak - 150,000 files (450 MB) - 85,000 named    │
├─────────────────────────────────────────────────────────┤
│  Files in PAK (check to extract):                       │
│  ┌──────────────────────────────────────────────────┐   │
│  │ ☐ Root                                           │   │
│  │   ☐ maps/                                        │   │
│  │     ☐ 西南北区/                                   │   │
│  │       ☐ 成都/                                    │   │
│  │         ☑ 成都.wor                               │   │
│  │         ☐ v_000/                                 │   │
│  │           ☑ 092_Region_S.dat                     │   │
│  │           ☑ 143_Region_S.dat                     │   │
│  └──────────────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────┤
│  File Details:                                           │
│  ┌──────────────────────────────────────────────────┐   │
│  │ Property         │ Value                         │   │
│  ├─────────────────┼──────────────────────────────│   │
│  │ Type             │ File                         │   │
│  │ Path             │ \maps\...\092_Region_S.dat   │   │
│  │ File ID          │ 0xABCD1234                   │   │
│  │ Size             │ 15,234 bytes (14.88 KB)      │   │
│  │ Compressed Size  │ 15,234 bytes (14.88 KB)      │   │
│  │ Compression      │ None                         │   │
│  └──────────────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────┤
│  Status: Loaded 85,000 files from maps.pak              │
└─────────────────────────────────────────────────────────┘
```

## Lưu ý quan trọng

### ⚠️ Compressed Files (UCL)

Một số files dùng UCL compression và **không thể extract** được:
- Tool sẽ skip những files này
- Hiển thị "⚠ Skipped (compressed)" trong extraction summary

**Giải pháp:**
- Dùng `unpack.exe` gốc từ Client folder
- Hoặc implement UCL decompression (cần ucl.dll)

### ✅ Uncompressed Files

Hầu hết files (đặc biệt là Region_S.dat) đều **không nén** và extract được bình thường!

## Extraction Summary

Sau khi extract xong, tool hiển thị summary:

```
Extraction complete!

✓ Extracted: 42,500 files
⚠ Skipped: 1,200 files (compressed)
❌ Errors: 0 files

Output: D:\HuyenThietKiem\Bin\Server\maps
```

## So sánh với Console version

| Feature | Console Tool | GUI Tool |
|---------|--------------|----------|
| Browse files | ❌ No | ✅ Tree view |
| File details | ❌ No | ✅ Yes |
| Select files | ❌ Extract all only | ✅ Checkbox selection |
| Preview | ❌ No | ✅ File info panel |
| Progress | ✅ Console output | ✅ Progress dialog |
| User-friendly | ⚠️ Command line | ✅ Visual UI |

## Keyboard Shortcuts

- **Space**: Check/uncheck selected item
- **Arrow keys**: Navigate tree
- **Enter**: Expand/collapse folder
- **Ctrl+A**: Select all (not implemented yet)

## Technical Details

### PAK File Format

Tool sử dụng `PakFileReader` từ MapTool project để đọc PAK files:
- Header: 32 bytes (signature, file count, offsets)
- Index table: 16 bytes per file (ID, offset, size, compression)
- Data section: Compressed/uncompressed file data

### File ID (Hash) Calculation

- PAK files dùng hash của filename (GB2312 encoded) làm file ID
- Tool tự động tính hash để lookup files
- Nếu có .pak.txt index file, dùng direct lookup (nhanh hơn)

## Troubleshooting

### Tool không mở được PAK file
- Kiểm tra file có tồn tại không
- Ensure file không bị corrupt
- Check file permissions

### Một số files không extract được
- Files bị nén UCL → dùng unpack.exe
- Files có path quá dài → shorten output path

### Tree view hiển thị sai encoding
- Tool dùng UTF-8 encoding
- Chinese characters should display correctly
- Nếu lỗi, check Windows locale settings

## Build Instructions

```bash
# Build từ Visual Studio
cd SwordOnline/Sources/Tool
dotnet build PakExtractTool/PakExtractTool.csproj -c Release

# Output
Bin/Server/PakExtractTool.exe
```

## Requirements

- .NET Framework 4.8
- Windows 7 hoặc mới hơn
- Windows Forms support

## Credits

Based on XPackFile format from Kingsoft Sword3 Engine.
