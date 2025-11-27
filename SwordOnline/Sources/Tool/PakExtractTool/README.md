# PakExtractTool - Công cụ giải nén maps.pak

## Mục đích

Tool này giải nén tất cả files từ `maps.pak` ra thư mục `Bin/Server/maps/` để:
- Lấy được data map đúng với định dạng server
- Sửa vấn đề data mismatch (RegionID không khớp)
- MapTool có thể load map từ disk thay vì pak

## Cách sử dụng

### Option 1: Chạy trực tiếp (Đơn giản nhất)

1. **Build tool** (chỉ cần làm 1 lần):
   ```
   - Mở Visual Studio
   - File → Open → Project/Solution
   - Chọn: SwordOnline/Sources/Tool/PakExtractTool.sln
   - Build → Build Solution (hoặc Ctrl+Shift+B)
   - Tool sẽ được tạo tại: Bin/Server/PakExtractTool.exe
   ```

2. **Chạy tool**:
   ```
   - Double-click vào: Bin/Server/PakExtractTool.exe
   - Nhấn Enter 2 lần (dùng path mặc định)
   - Chờ tool extract (có thể mất vài phút)
   - Xong!
   ```

### Option 2: Chạy với tham số

```bash
cd D:\HuyenThietKiemMobile\Bin\Server

# Extract maps.pak vào thư mục maps/
PakExtractTool.exe pak/maps.pak maps

# Hoặc dùng đường dẫn đầy đủ
PakExtractTool.exe "D:\HuyenThietKiemMobile\Bin\Server\pak\maps.pak" "D:\HuyenThietKiemMobile\Bin\Server\maps"
```

## Kết quả

Sau khi extract xong, bạn sẽ có:

```
Bin/Server/maps/
├── MapList.ini
├── 西北南区/
│   ├── 成都/
│   │   ├── 成都.wor
│   │   ├── v_000/
│   │   │   ├── 092_Region_S.dat
│   │   │   ├── 143_Region_S.dat
│   │   │   └── ...
│   │   ├── v_001/
│   │   └── ...
│   └── ...
└── ...
```

## Lưu ý quan trọng

### ⚠️ Files bị nén (UCL Compression)

Một số files trong pak dùng UCL compression và **KHÔNG THỂ extract** bằng tool này. Tool sẽ báo:

```
⚠ Skipped (compressed): \maps\西北南区\成都\成都.wor
  Reason: UCL decompression not implemented
```

**Giải pháp:**
1. Dùng `unpack.exe` gốc trong `Bin/Client/unpack.exe` để extract những files này
2. Hoặc implement UCL decompression (cần `ucl.dll` và PInvoke)

### ✅ Files không nén

Hầu hết **Region_S.dat** files đều KHÔNG NÉN và sẽ extract thành công!

## Kiểm tra kết quả

Sau khi extract, chạy lại MapTool và xem log file:

```
✓ Loaded .wor from disk: D:\...\Bin\Server\maps\西北南区\成都\成都.wor
🔍 SCANNING FOR REGION FILES
   Looking for regions from (92,0) to (255,5)
   ✓ Found region ( 92,  0) → RegionID=   92
   ✓ Found region (143,  0) → RegionID=  143
   ...
📊 REGION SCAN SUMMARY
   Attempted: 984 regions
   Loaded: 150 regions
   Missing: 834 regions
```

Bây giờ RegionID sẽ đúng: **92, 143, 367...** thay vì **36557**!

## Troubleshooting

### Tool báo lỗi "Pak file not found"
- Kiểm tra đường dẫn đến maps.pak
- Đảm bảo file tồn tại: `Bin/Server/pak/maps.pak`

### Extract xong nhưng MapTool vẫn load sai
- Kiểm tra MapTool đang chọn đúng game folder: `D:\HuyenThietKiemMobile\Bin\Server`
- Xóa folder `maps/` cũ trước khi extract lại

### Một số files không extract được
- Đó là files bị nén UCL, dùng `unpack.exe` gốc để extract
- Hoặc bỏ qua nếu chỉ cần Region_S.dat files

## Liên hệ

Nếu gặp lỗi, tạo issue tại: https://github.com/aionusonlinekenny/HuyenThietKiem/issues
