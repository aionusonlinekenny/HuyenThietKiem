# Map Coordinate Tool - Hướng dẫn sử dụng C++ version

Sau khi xem xét, tôi đã tạo 2 versions:

## Version 1: C# Windows Forms (Đã hoàn thành)
- Location: `SwordOnline/Sources/Tool/MapTool/`
- Pros: UI đẹp, dễ develop, nhiều features
- Cons: Cần .NET Framework, không tích hợp trực tiếp với Core

## Version 2: C++ Standalone (Đang tạo)
- Location: `SwordOnline/Sources/Tool/MapToolCpp/`
- Pros: Native code, nhỏ gọn, có thể link với Core sau
- Cons: UI đơn giản hơn (Win32 API)

---

## Khuyến nghị sử dụng

### Nếu bạn muốn tool hoàn chỉnh ngay:
→ Dùng **C# version** trong `MapTool/`
- Build bằng Visual Studio hoặc `dotnet build`
- Chạy ngay được
- Đầy đủ tính năng

### Nếu bạn muốn integrate sâu với project:
→ Tôi sẽ tạo **C++ version** mới
- Có thể link trực tiếp với `Core` library
- Reuse KSubWorld, KRegion classes
- Nhưng cần thời gian setup thêm

---

Bạn muốn tôi:
1. ✅ Giữ C# version như hiện tại (đã xong)
2. 🔄 Tạo C++ standalone version (đơn giản, Win32 API)
3. 🚀 Tạo C++ integrated version (link với Core lib, phức tạp)

Chọn option nào bạn?
