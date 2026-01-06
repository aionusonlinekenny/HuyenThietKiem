# Hướng Dẫn Sử Dụng SayImage() - Hiển Thị NPC Avatar Trong Dialog

## Tổng Quan

`SayImage()` là hàm Lua được thiết kế sẵn trong game để hiển thị dialog với hình ảnh NPC/character.

**Khác biệt với Talk():**
- `Talk()` - Dialog text đơn giản, không có hình ảnh
- `SayImage()` - Dialog text + hình ảnh NPC ở bên trái

## Cú Pháp

```lua
SayImage(
    "Dialog text",           -- [1] Nội dung hội thoại (string)
    "X/Y/ImageKey",         -- [2] Vị trí và mã ảnh (string format: "Left/Top/Key")
    OptionCount,            -- [3] Số lượng lựa chọn (number)
    "Option 1/function1",   -- [4+] Các tùy chọn trả lời (string format: "Text/Function")
    "Option 2/function2",
    ...
)
```

## Chi Tiết Tham Số

### Tham số 1: Dialog Text
- Chuỗi text hiển thị trong dialog
- Hỗ trợ tiếng Việt
- Giống như text trong Talk()

### Tham số 2: Image Position & Key **[QUAN TRỌNG]**
Format: `"X/Y/ImageKey"`

- **X**: Vị trí Left (pixels từ trái)
- **Y**: Vị trí Top (pixels từ trên)
- **ImageKey**: Số thứ tự (numeric key) trong file `\Ui\PictureList.ini`

**Ví dụ:** `"10/20/17"` có nghĩa là:
- Hiển thị ảnh tại vị trí (10, 20)
- Sử dụng sprite ở key số 17 trong PictureList.ini

### Tham số 3: Option Count
- `0` = Không có lựa chọn, tự động đóng dialog khi click
- `1+` = Số lượng lựa chọn cho người chơi

### Tham số 4+: Options
Format: `"AnswerText/FunctionName"`
- **AnswerText**: Text hiển thị cho lựa chọn
- **FunctionName**: Tên hàm Lua sẽ được gọi khi chọn

## Ví Dụ Cụ Thể

### Ví dụ 1: Dialog không có lựa chọn (auto-close)
```lua
SayImage(
    "Xin chào, ta là Tống Quan viên!",
    "10/20/17",    -- ImageKey 17 = passerby004s1.spr
    0              -- No options, auto-close when clicked
)
```

### Ví dụ 2: Dialog với 2 lựa chọn
```lua
function main()
    SayImage(
        "Ngươi cần gì từ ta?",
        "15/25/18",    -- ImageKey 18 = passerby028z.spr
        2,             -- 2 options
        "Mua vật phẩm/shop",
        "Kết thúc đối thoại/exit"
    )
end

function shop()
    -- Open shop
    Say("Đây là gian hàng của ta!", 0)
end

function exit()
    -- Close dialog
end
```

### Ví dụ 3: Dialog với NPC enemy
```lua
SayImage(
    "Ta là quái vật! Ngươi dám đến đây?",
    "20/30/32",    -- ImageKey 32 = enemy170_st.spr
    3,
    "Tấn công!/attack",
    "Chạy trốn!/run",
    "Thương lượng/negotiate"
)
```

### Ví dụ 4: Chuỗi dialog liên tiếp
```lua
function main()
    SayImage(
        "Chào mừng đến với làng!",
        "10/20/19",
        1,
        "Tiếp tục/next"
    )
end

function next()
    SayImage(
        "Ta có nhiệm vụ cho ngươi!",
        "10/20/19",    -- Same NPC image
        2,
        "Nhận nhiệm vụ/accept_quest",
        "Từ chối/decline"
    )
end

function accept_quest()
    -- Give quest
    Say("Hãy đánh bại 10 con sói!", 0)
end
```

## Quản Lý File PictureList.ini

### Vị trí file:
`\Bin\Client\Ui\PictureList.ini`

### Format:
```ini
[Picture]
Count=72
1=\spr\img.spr
17=\spr\npcres\passerby\passerby004\passerby004s1.spr
18=\spr\npcres\passerby\passerby028\passerby028z.spr
32=\spr\npcres\enemy\enemy170\enemy170_st.spr
...
```

### Thêm NPC sprite mới:

1. **Tìm sprite NPC** trong thư mục `\Bin\Client\Spr\npcres\`
2. **Mở PictureList.ini** và thêm dòng mới:
   ```ini
   73=\spr\npcres\passerby\passerby200\passerby200_st.spr
   ```
3. **Update Count** (không bắt buộc, nhưng nên làm):
   ```ini
   Count=73
   ```
4. **Sử dụng trong Lua** với key mới:
   ```lua
   SayImage("Test NPC mới!", "10/20/73", 0)
   ```

### Sprite NPC có sẵn (một số examples):

| ImageKey | Sprite Path | Mô tả |
|----------|-------------|-------|
| 17 | passerby004s1.spr | Người thường #1 |
| 18 | passerby028z.spr | Người thường #2 |
| 19 | passerby007s.spr | Người thường #3 |
| 32 | enemy170_st.spr | Quái vật #1 |
| 33 | enemy165_st.spr | Quái vật #2 |
| 34 | enemy158_st.spr | Quái vật #3 |

## Kỹ Thuật Implementation (Backend)

### Luồng xử lý:
1. **Lua Script** gọi `SayImage(...)`
2. **ScriptFuns.cpp** - `LuaSelectUI2()` xử lý tham số
3. **KPlayer.cpp** - `OnScriptAction()` với `m_bParam2 = 2` (flag cho ShowImage)
4. **GameSpaceChangedNotify.cpp** - Dispatch đến `KUiMsgSel2::OpenWindow()`
5. **UiMsgSel2.cpp** - Hiển thị UI với image

### Cách image được load:
```cpp
// UiMsgSel2.cpp:237-280
void KUiMsgSel2::InitImage(const char* pszKey)
{
    // Parse "X/Y/ImageKey" format
    // pszKey = "10/20/17" -> nPos[0]=10, nPos[1]=20, ImageKey="17"

    // Load from PictureList.ini
    KIniFile Ini;
    Ini.Load("\\Ui\\PictureList.ini");

    char szImage[80];
    Ini.GetString("Picture", "17", "", szImage, sizeof(szImage));

    // Set sprite and position
    m_Image.SetImage(ISI_T_SPR, szImage);
    m_Image.SetPosition(nPos[0], nPos[1]);
    m_Image.SetFrame(0);
}
```

### Animation:
Sprite sẽ tự động animate nếu có nhiều frames:
```cpp
// UiMsgSel2.cpp:229-235
void KUiMsgSel2::Breathe()
{
    if(m_nImgNumFrames > 1)
    {
        if(m_Image.GetCurFrame() >= m_nImgNumFrames)
            m_Image.SetFrame(0);
        m_Image.NextFrame();
    }
}
```

## So Sánh: Talk() vs SayImage()

| Feature | Talk() | SayImage() |
|---------|--------|------------|
| Dialog text | ✅ Yes | ✅ Yes |
| NPC avatar | ❌ No | ✅ Yes |
| Multiple options | ✅ Yes | ✅ Yes |
| UI Component | UiInformation2 | UiMsgSel2 |
| Image source | N/A | PictureList.ini |
| Position control | Fixed | Customizable (X/Y) |

## Best Practices

### 1. Vị trí image hợp lý:
```lua
-- Tốt: Image ở góc trái, không che text
SayImage("Text", "10/20/17", 0)   -- Good position

-- Xấu: Image ở giữa màn hình, che text
SayImage("Text", "300/200/17", 0) -- Bad position
```

### 2. Sử dụng sprite phù hợp:
```lua
-- Tốt: NPC thương gia -> dùng passerby sprite
SayImage("Mua gì nào?", "10/20/17", 1, "Mua/shop")

-- Xấu: NPC thương gia -> dùng enemy sprite
SayImage("Mua gì nào?", "10/20/32", 1, "Mua/shop") -- Enemy sprite for merchant?
```

### 3. Nhất quán trong chuỗi dialog:
```lua
function main()
    SayImage("Chào!", "10/20/17", 1, "Tiếp/next")
end

function next()
    -- Tốt: Giữ nguyên ImageKey 17 (cùng NPC)
    SayImage("Tạm biệt!", "10/20/17", 0)

    -- Xấu: Đổi sang ImageKey khác (NPC khác)
    -- SayImage("Tạm biệt!", "10/20/32", 0)
end
```

## Troubleshooting

### Vấn đề 1: Không hiển thị ảnh
**Nguyên nhân:**
- ImageKey không tồn tại trong PictureList.ini
- Sprite file không tồn tại trong thư mục

**Giải pháp:**
1. Kiểm tra file `\Bin\Client\Ui\PictureList.ini`
2. Verify ImageKey tồn tại: `17=\spr\npcres\...`
3. Kiểm tra sprite file tồn tại: `\Bin\Client\Spr\npcres\...`

### Vấn đề 2: Ảnh hiển thị sai vị trí
**Nguyên nhân:**
- X/Y parameters sai

**Giải pháp:**
- Điều chỉnh X/Y cho phù hợp với UI layout
- Khuyến nghị: X=10-50, Y=20-50 cho hầu hết cases

### Vấn đề 3: Format string sai
**Sai:**
```lua
SayImage("Text", 10, 20, 17, 0)      -- ❌ Wrong: separate numbers
SayImage("Text", "10-20-17", 0)      -- ❌ Wrong: wrong separator
SayImage("Text", "10,20,17", 0)      -- ❌ Wrong: comma instead of slash
```

**Đúng:**
```lua
SayImage("Text", "10/20/17", 0)      -- ✅ Correct: slash separator
```

## Tài Liệu Tham Khảo

### Source Files:
- `/Core/Src/ScriptFuns.cpp` - Line 487-636: LuaSelectUI2() implementation
- `/Core/Src/KPlayer.cpp` - Line 6665-6785: UI_SELECTDIALOG handler
- `/S3Client/Ui/UiCase/UiMsgSel2.cpp` - Complete UI implementation
- `/S3Client/Ui/UiCase/UiMsgSel2.h` - Class definition
- `/S3Client/Ui/GameSpaceChangedNotify.cpp` - Line 320-327: GDCNI_QUESTION_CHOOSE

### Config Files:
- `/Bin/Client/Ui/PictureList.ini` - Image registry
- `/Bin/Client/Ui/Ui3/UiMsgSel2.ini` - UI layout configuration

## Kết Luận

`SayImage()` là cách **chính thức và được hỗ trợ sẵn** để hiển thị NPC avatar trong dialog. Không cần modify code C++, chỉ cần:

1. ✅ Thêm sprite vào `PictureList.ini` (nếu chưa có)
2. ✅ Sử dụng `SayImage()` thay vì `Talk()`
3. ✅ Format đúng: `"X/Y/ImageKey"`

**Lưu ý:** Không nên modify `Talk()` (UiInformation2) để thêm avatar vì đã có `SayImage()` được design sẵn cho mục đích này!
