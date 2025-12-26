# Purple Smith UI - Hướng Dẫn Tích Hợp

## Tổng Quan
UI C++ cho hệ thống chế tạo trang bị TÍM (Purple Item Crafting System).

## Files Đã Tạo

### 1. C++ Source Files
- **UiPurpleSmith.h**
  - Location: `SwordOnline/Sources/S3Client/Ui/UiCase/UiPurpleSmith.h`
  - Class definition cho Purple Smith UI

- **UiPurpleSmith.cpp**
  - Location: `SwordOnline/Sources/S3Client/Ui/UiCase/UiPurpleSmith.cpp`
  - Implementation của Purple Smith UI

### 2. UI Layout Configuration
- **UiPurpleSmith.ini**
  - Location: `Bin/Client/Ui/Ui3/UiPurpleSmith.ini`
  - Cấu hình layout và vị trí các UI elements

## Tính Năng UI

### UI Elements
1. **10 Blue Item Slots** - Ô để đặt trang bị xanh (2 hàng x 5 ô)
2. **7 Crystal Slots** - Ô để đặt Huyền Tinh (1 hàng x 7 ô)
3. **Craft Button** - Nút "Hợp Thành" để thực hiện chế tạo
4. **Close Button** - Nút "Đóng" để đóng UI
5. **Success Rate Display** - Hiển thị tỷ lệ thành công
6. **Cost Display** - Hiển thị chi phí
7. **Effect Animation** - Hiệu ứng khi đang chế tạo

### Validation Rules
- Tối thiểu 6 trang bị xanh
- Tối đa 10 trang bị xanh
- Tối thiểu 1 Huyền Tinh
- Tối đa 7 Huyền Tinh
- Chỉ chấp nhận trang bị xanh (equip_genre)
- Không chấp nhận ngựa (horse) và mặt nạ (mask)
- Chỉ chấp nhận 4 loại Huyền Tinh:
  - Lam Thủy Tinh (detail 16)
  - Tử Thủy Tinh (detail 17)
  - Lục Thủy Tinh (detail 18)
  - Hồng Thủy Tinh (detail 100)

## Tích Hợp Vào Project

### Bước 1: Thêm Files Vào Build System
Thêm vào Visual Studio project hoặc makefile:
```
UiPurpleSmith.cpp
UiPurpleSmith.h
```

### Bước 2: Include Header
Trong file cần mở UI (ví dụ: trong Lua binding hoặc NPC handler):
```cpp
#include "Ui/UiCase/UiPurpleSmith.h"
```

### Bước 3: Mở UI Window
```cpp
// Mở UI Purple Smith
KUiPurpleSmith::OpenWindow();

// Kiểm tra xem UI có đang mở không
if (KUiPurpleSmith::GetIfVisible())
{
    // UI đang mở
}

// Đóng UI
KUiPurpleSmith::CloseWindow(true);  // true = destroy instance
```

### Bước 4: Kết Nối Với Server
UI sẽ gọi Lua function khi người chơi nhấn nút "Hợp Thành":
```cpp
// Trong OnCraft() function:
g_pCoreShell->OperationRequest(GOI_EXESCRIPT_BUTTON, "ExecuteCreatePurple", 4);
```

Server cần implement function `ExecuteCreatePurple` trong Lua script tương ứng với NPC.

### Bước 5: Kết Nối Với NPC Purple Smith
Trong file `purple_smith_v2.lua`, thêm function để mở UI:

```lua
function OpenPurpleSmithUI()
    -- C++ sẽ mở UI
    -- Người chơi sẽ thấy UI với 10 ô blue items + 7 ô crystals
    -- Server sẽ nhận callback khi player nhấn "Hợp Thành"
end

function ExecuteCreatePurple()
    -- Function này sẽ được gọi từ C++ UI
    -- Implement logic chế tạo trang bị TÍM ở đây
    local nBlueCount = 0
    local nTotalLuck = 0

    -- Đếm blue items và crystals từ build container
    -- Tính toán luck
    -- Tạo trang bị TÍM
    -- Xử lý thành công/thất bại
end
```

## UI Layout Details

### Window Size
- Width: 400px
- Height: 450px
- Moveable: Yes
- Position: (250, 80) - center-left

### Blue Item Slots Layout
```
Row 1: [  ] [  ] [  ] [  ] [  ]  (slots 1-5)
Row 2: [  ] [  ] [  ] [  ] [  ]  (slots 6-10)
```
- Each slot: 52x52 pixels
- Spacing: 8px horizontal

### Crystal Slots Layout
```
[  ] [  ] [  ] [  ] [  ] [  ] [  ]  (slots 1-7)
```
- Each slot: 40x40 pixels
- Spacing: 10px horizontal

## Success Rate Display
Tự động cập nhật dựa trên số lượng trang bị xanh:
- 6-7 items: 60% success rate | Cost: 500,000
- 8 items: 70% success rate | Cost: 800,000
- 9 items: 75% success rate | Cost: 1,000,000
- 10 items: 80% success rate | Cost: 1,500,000

## Animation Effect
- Effect plays khi đang chế tạo
- Duration: 2 loops
- Blocks user input trong lúc effect đang chạy
- Gọi `OnCraft()` khi effect kết thúc

## Error Messages
Tất cả error messages được định nghĩa trong `[ReturnInfo]` section của INI file:
- 0: Confirmation message
- 1: Need minimum 6 blue items
- 2: Need minimum 1 crystal
- 3: Only blue equipment allowed
- 4: Only crystals allowed
- 5: Crafting failed message
- 6: Crafting success message
- 7: Not enough money
- 8: Invalid equipment
- 9: Please place all materials

## Debug Logging
UI có debug logging được tích hợp:
```cpp
g_DebugLog("[CLIENT] KUiPurpleSmith::OpenWindow() called");
```

Kiểm tra log file để debug issues.

## Testing Checklist
- [ ] UI mở được khi gọi `KUiPurpleSmith::OpenWindow()`
- [ ] Có thể drag & drop items vào slots
- [ ] Validation hoạt động đúng (từ chối items không hợp lệ)
- [ ] Success rate cập nhật khi thêm/bớt items
- [ ] Craft button gọi đúng Lua function
- [ ] Animation effect hoạt động
- [ ] Close button đóng UI và trả items về
- [ ] Error messages hiển thị đúng

## Integration với Existing System
UI này sử dụng:
- `UOC_BUILD_ITEM` container (same as UiTrembleItem)
- `GOI_EXESCRIPT_BUTTON` để gọi Lua function
- `pos_builditem` position ID
- Standard UI framework (KWndShowAnimate)

## Notes
- UI này tương thích với hệ thống hiện tại vì dựa trên UiTrembleItem
- Không cần thay đổi server protocol
- Sử dụng build container system có sẵn
- Animation sprites cần exist trong game assets

## Next Steps
1. Add UiPurpleSmith.cpp/h vào Visual Studio project
2. Compile client
3. Test UI in-game
4. Integrate với purple_smith_v2.lua NPC
5. Test full crafting workflow
6. Adjust UI layout nếu cần

## Contact
Nếu có vấn đề, kiểm tra:
1. Log files cho debug messages
2. INI file có load đúng không
3. Sprites có tồn tại không
4. Build container system hoạt động không
