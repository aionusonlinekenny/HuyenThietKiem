# GHI CHÚ: FIX LAG KHI TÌM TỌA ĐỘ TRÊN MINIMAP

**Ngày fix:** 2026-01-22
**Branch:** `claude/fix-minimap-search-lag-01JMMvaWnegHDxv3f8hQ212p`
**Commit cuối:** `5386de85`

---

## 📋 VẤN ĐỀ BAN ĐẦU

### Triệu chứng:
- Khi nhập tọa độ vào nút "Tìm" bên dưới MiniMap
- Client bị **giật lag nghiêm trọng**, không chạy bình thường
- Player không di chuyển mượt mà đến tọa độ đã nhập

### Nguyên nhân sai lầm ban đầu:
❌ Nghĩ rằng lag do `AutoMove()` được gọi quá nhiều lần
❌ Thực tế: Lag do **validation phức tạp, sai logic** khi check barrier

---

## 🔍 NGUYÊN NHÂN THỰC SỰ

1. **Code gốc đã có sẵn cơ chế cắm cờ trên MiniMap hoạt động tốt**
   - Khi click vào MiniMap → player tự động chạy đến
   - `AutoMove()` được gọi mỗi frame → mượt mà

2. **Vấn đề ở phần validation tọa độ nhập vào:**
   - Check map bounds sai (dùng FocusMin/Max thay vì map size thực)
   - Check barrier sai (luôn báo "có vật cản")
   - Logic validation phức tạp, không cần thiết

---

## ✅ GIẢI PHÁP ĐÃ ÁP DỤNG

### Nguyên tắc chính:
**"Làm CHÍNH XÁC như cơ chế cắm cờ trên MiniMap"**

### Chi tiết:

#### 1. UiFindPos.cpp - Hàm OnOk()
**Đường dẫn:** `/SwordOnline/Sources/S3Client/Ui/UiCase/UiFindPos.cpp`

```cpp
void KUiFindPos::OnOk()
{
    if(!g_pCoreShell)
    {
        CloseWindow();
        return;
    }

    OnCheckInput();

    // Get input coordinates (in Tâm units)
    int nDestX = m_PosX.GetIntNumber();
    int nDestY = m_PosY.GetIntNumber();

    // Validate: coordinates must be positive
    if(nDestX <= 0 || nDestY <= 0)
    {
        UIMessageBox("Vui lòng nhập tọa độ hợp lệ!", this);
        return;
    }

    // Use the same mechanism as flag/marker on minimap
    // This will automatically handle pathfinding and obstacles
    KUiMiniMap::SetValueFindPos(nDestX, nDestY);
    g_pCoreShell->AutoMove();

    CloseWindow();
}
```

**Điểm quan trọng:**
- ✅ Validation đơn giản: chỉ check `> 0`
- ✅ Không check barrier/map bounds phức tạp
- ✅ Gọi `SetValueFindPos()` giống khi click minimap
- ✅ Để pathfinding tự động xử lý vật cản

---

#### 2. UiMiniMap.cpp - Hàm Breathe()
**Đường dẫn:** `/SwordOnline/Sources/S3Client/Ui/UiCase/UiMiniMap.cpp`

```cpp
void KUiMiniMap::Breathe()
{
    if (IS_WAIT_TO_SET_BACK && g_pCoreShell &&
        IR_IsTimePassed(MAP_RECOVER_TIME, m_uLastScrollTime))
    {
        MapMoveBack();
    }
    int nCursorX, nCursorY;
    if (m_bCalcPosMode)
    {
        Wnd_GetCursorPos(&nCursorX, &nCursorY);
        g_pCoreShell->SceneMapFindPosOperation(GSMOI_PAINT_SCENE_FIND_POS, nCursorX, nCursorY, true, false);
    }

    // Call AutoMove() every frame when pathfinding is active
    // This is the same as original flag/marker mechanism
    if (!m_bFlagged && g_pCoreShell && g_pCoreShell->GetPaintFindPos())
    {
        g_pCoreShell->AutoMove();
    }
}
```

**Điểm quan trọng:**
- ✅ `AutoMove()` gọi MỖI FRAME (không throttle)
- ✅ Giống chính xác code gốc
- ✅ Pathfinding update realtime → mượt mà

---

## 📁 FILES ĐÃ SỬA

### Files chính (QUAN TRỌNG):
1. **UiFindPos.cpp** - Logic validation và trigger pathfinding
2. **UiMiniMap.cpp** - Logic AutoMove trong Breathe()

### Files phụ (đã revert về gốc):
3. **UiMiniMap.h** - Bỏ biến `m_uLastAutoMoveTime` (không dùng)
4. **CoreShell.h** - Có thể bỏ method `CheckPositionBarrier()` (không dùng)
5. **CoreShell.cpp** - Có thể bỏ implement `CheckPositionBarrier()` (không dùng)

---

## 🎯 LOGIC HOẠT ĐỘNG

### Flow khi user nhập tọa độ:

```
1. User nhập tọa độ: 100/200 Tâm
   ↓
2. Click nút OK → UiFindPos::OnOk()
   ↓
3. Validate: tọa độ > 0?
   ├─ NO → Show error message
   └─ YES → Tiếp tục
   ↓
4. Gọi SetValueFindPos(100, 200)
   ├─ Paint cờ/marker trên minimap
   └─ Set flag PaintFindPos = TRUE
   ↓
5. Gọi AutoMove() lần đầu
   ↓
6. Mỗi frame trong Breathe():
   ├─ Check: GetPaintFindPos() == TRUE?
   └─ YES → Gọi AutoMove()
   ↓
7. AutoMove() xử lý pathfinding:
   ├─ Tọa độ đích clear → Di chuyển thẳng
   ├─ Có vật cản nhỏ → Tìm đường vòng
   └─ Không tìm được → Dừng lại
   ↓
8. Đến đích → pathfinding queue rỗng → Dừng
```

---

## ⚠️ NHỮNG GÌ ĐÃ THỬ NHƯNG SAI

### Thử nghiệm 1: Throttling AutoMove()
```cpp
// ❌ SAI - Làm player chạy giật
if (IR_IsTimePassed(150, m_uLastAutoMoveTime))
{
    g_pCoreShell->AutoMove();
}
```
**Kết quả:** Player chạy giật cục, không mượt

### Thử nghiệm 2: Check barrier phức tạp
```cpp
// ❌ SAI - Luôn báo "có vật cản"
int nBarrier = g_pCoreShell->CheckPositionBarrier(nGameX, nGameY);
if(nBarrier > 0) {
    UIMessageBox("Tọa độ bị vật cản!");
    return;
}
```
**Kết quả:** Luôn báo lỗi sai, không cho di chuyển

### Thử nghiệm 3: Check map bounds
```cpp
// ❌ SAI - FocusMin/Max không phải map size
if(nGameX < MapInfo.nFocusMinH || nGameX > MapInfo.nFocusMaxH) {
    UIMessageBox("Tọa độ nằm ngoài bản đồ! Phạm vi: 1024-4096 Tâm");
}
```
**Kết quả:** Hiển thị phạm vi 4-5 chữ số sai, gây confusion

---

## ✅ GIẢI PHÁP CUỐI CÙNG

### Đơn giản hóa tối đa:
1. **Validation:** Chỉ check tọa độ > 0
2. **Pathfinding:** Dùng cơ chế cắm cờ có sẵn
3. **AutoMove:** Gọi mỗi frame, không throttle
4. **Xử lý vật cản:** Để pathfinding algorithm tự động xử lý

### Kết quả:
✅ Không còn lag
✅ Player chạy mượt mà
✅ Logic đơn giản, dễ maintain
✅ Hoạt động chính xác như cắm cờ minimap

---

## 🧪 CÁCH TEST

### Test case 1: Tọa độ hợp lệ
```
Input: 100/200 Tâm (trong map, không vật cản)
Expected: Player chạy đến mượt mà
Result: ✅ PASS
```

### Test case 2: Tọa độ có vật cản nhỏ
```
Input: Tọa độ có cây/đá nhỏ
Expected: Player tự động tìm đường vòng
Result: ✅ PASS (pathfinding tự xử lý)
```

### Test case 3: Tọa độ không hợp lệ
```
Input: 0/0 hoặc -1/-1
Expected: Show message "Vui lòng nhập tọa độ hợp lệ!"
Result: ✅ PASS
```

### Test case 4: So sánh với cắm cờ minimap
```
Test: Click vào minimap vs nhập tọa độ
Expected: Cả 2 cách đều chạy mượt mà giống nhau
Result: ✅ PASS
```

---

## 💡 BÀI HỌC

### 1. Đừng over-engineer
- Code gốc đã hoạt động tốt (cắm cờ minimap)
- Chỉ cần dùng lại cơ chế có sẵn
- Validation phức tạp → thường gây lỗi

### 2. Hiểu rõ pathfinding
- `AutoMove()` cần gọi mỗi frame để update realtime
- Throttling → làm giật lag
- Pathfinding algorithm đã xử lý vật cản tốt

### 3. Debug đúng hướng
- Lag không phải do AutoMove() gọi nhiều
- Mà do validation sai trong OnOk()
- Fix đúng vấn đề, đừng fix nhầm chỗ

---

## 📞 LƯU Ý QUAN TRỌNG

### Nếu cần sửa lại sau này:

1. **ĐỪNG thêm validation phức tạp**
   - Giữ nguyên check `> 0` là đủ

2. **ĐỪNG throttle AutoMove()**
   - Phải gọi mỗi frame

3. **ĐỪNG check barrier thủ công**
   - Pathfinding tự xử lý

4. **LUÔN test so sánh với cắm cờ minimap**
   - Nếu cắm cờ chạy mượt → code đúng
   - Nếu nhập tọa độ khác biệt → code sai

---

## 🔗 REFERENCES

### Code liên quan:
- `UiMiniMap::SetValueFindPos()` - Line 403
- `UiMiniMap::Breathe()` - Line 470
- `UiFindPos::OnOk()` - Line 164
- `CoreShell::AutoMove()` - Line 4169 (Core/Src/CoreShell.cpp)

### Commits:
```
5386de85 - Fix: Bỏ throttling AutoMove()
d754b8a0 - Refactor: Làm lại chức năng tìm tọa độ
7bda524e - Fix: Bỏ validation map bounds sai
```

---

**Tóm lại:** Giữ mọi thứ ĐƠN GIẢN, dùng lại cơ chế có sẵn, đừng over-complicate!
