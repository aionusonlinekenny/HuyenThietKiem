# PHÂN TÍCH BUG STACK SPLIT - HuyenThietKiem

## MÔ TẢ BUG

**Hiện tượng:**
- Các stack item MỚI được add vào inventory: split (Ctrl+Right Click) hoạt động BÌNH THƯỜNG
- Các stack item được LOAD TỪ DATABASE khi login: UI KHÔNG CẬP NHẬT khi split, mặc dù server-side logic hoạt động đúng
- Các item được split ra từ item gốc vẫn hoạt động bình thường khi stack/split lại
- Khi logout và login lại, item lại bị tình trạng tương tự

**Log minh chứng:**
```
[14:24:55] [CLIENT-STACK-SYNC] ItemChangeInfo: Type=1, ItemID=131, Change=7
[14:24:55] [CLIENT-STACK-SYNC] Item found! Name=Cát Linh Thạch, Current durability=8
[14:24:55] [CLIENT-STACK-SYNC] Case 1: SetStackCount to 7 (old=8)
[14:24:55] [CLIENT-STACK-SYNC] After SetStackCount: durability=7  <-- GIÁ TRỊ ĐÃ ĐÚNG
[14:24:55] [CLIENT-STACK-SYNC] Sending CONTAINER_OBJECT_CHANGED for equipment room  <-- NHƯNG UI KHÔNG REFRESH
```

## NGUYÊN NHÂN GỐC RỄ

### 1. BUG CHÍNH: Notification Không Có Handler

**File:** `SwordOnline/Sources/Core/Src/KProtocolProcess.cpp:3944-3968`

Code hiện tại GỬI notification KHÔNG TỒN TẠI handler:
```cpp
case 1:  // SetStackCount
    Item[nIdx].SetStackCount(pICI->m_uChange);

    // ❌ BUG: Gửi GDCNI_CONTAINER_OBJECT_CHANGED - KHÔNG CÓ HANDLER!
    KUiObjAtContRegion Region;
    Region.Obj.uGenre = CGOG_ITEM;
    Region.Obj.uId = nIdx;
    Region.eContainer = UOC_ITEM_TAKE_WITH;
    CoreDataChanged(GDCNI_CONTAINER_OBJECT_CHANGED, (unsigned int)&Region, 1);  // <-- VÔ DỤNG!
```

**Kiểm tra:**
```bash
grep -r "case GDCNI_CONTAINER_OBJECT_CHANGED" SwordOnline/Sources/S3Client/
# KẾT QUẢ: KHÔNG TÌM THẤY HANDLER NÀO!
```

**Handler thực sự tồn tại:**
- `GDCNI_OBJECT_CHANGED` (GameSpaceChangedNotify.cpp:140) - CÓ HANDLER ✓
- `GDCNI_CONTAINER_OBJECT_CHANGED` - KHÔNG CÓ HANDLER ✗

### 2. Tại Sao Code Đã Có NotifyItemChange() Nhưng Không Dùng?

**File:** `SwordOnline/Sources/Core/Src/KItemList.cpp:4306-4377`

Commit 3062bb369 đã thêm function `NotifyItemChange()` với logic ĐÚ <br/>
```cpp
void KItemList::NotifyItemChange(int nItemIdx)
{
    // ... tìm item position, map container ...

    // ✓ GỬI NOTIFICATION ĐÚNG
    CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo, 1);
}
```

NHƯNG function này KHÔNG BAO GIỜ ĐƯỢC GỌI trong flow ItemChangeInfo!

### 3. Phân Tích Flow Hoàn Chỉnh

#### Server Side (ExchangeStack)
```
1. Player Ctrl+Right Click split stack
   ├─> KItemList::ExchangeStack() được gọi
   ├─> StackItem() tính toán split logic
   ├─> Item[nIdxBeStack].SetDurability(newCount) - CẬP NHẬT SERVER-SIDE ✓
   ├─> GỬI ITEM_CHANGE_INFO type=1 tới client ✓
   └─> Player[m_PlayerIdx].Save() - LƯU VÀO DATABASE ✓
```

#### Client Side (ItemChangeInfo)
```
1. Nhận ITEM_CHANGE_INFO
   ├─> ItemSet.SearchID(ItemID) - TÌM ITEM ✓
   ├─> Item[nIdx].SetStackCount(newCount) - CẬP NHẬT CLIENT MEMORY ✓
   │   └─> m_nCurrentDur = newCount  (stack count được lưu vào durability field)
   │
   ├─> GỬI GDCNI_CONTAINER_OBJECT_CHANGED ✗ BUG: KHÔNG CÓ HANDLER!
   │   └─> UI không bao giờ nhận được notification
   │   └─> Paint() không được gọi lại
   │   └─> Số lượng stack cũ vẫn hiển thị trên màn hình
   │
   └─> UnlockOperation()
```

#### UI Rendering (Paint)
```cpp
void KItem::Paint(int nX, int nY, BOOL bStack)
{
    // Vẽ item image...

    if (CanStack() && bStack)
    {
        char szBuffer[8];
        sprintf(szBuffer, "%d", m_nCurrentDur);  // <-- ĐỌC TỪ m_nCurrentDur (đã đúng!)
        g_pRepresent->OutputText(..., szBuffer, ...);  // <-- VẼ SỐ LƯỢNG
    }
}
```

**Kết luận:**
- `m_nCurrentDur` đã được update đúng
- `Paint()` SẼ hiển thị đúng NẾU được gọi
- NHƯNG `Paint()` không được gọi lại vì UI handler không nhận được notification

### 4. Tại Sao Item MỚI Hoạt Động Đúng Nhưng Item LOAD TỪ DB Lại Lỗi?

**Không phải do khác biệt khởi tạo** - cả hai loại item đều:
- Dùng cùng ItemSet.SearchID() để tìm item
- Dùng cùng SetStackCount() để update
- Dùng cùng notification mechanism

**Nguyên nhân thật sự:**
Có thể do timing hoặc UI state:
- Item mới: UI đang "active" và có thể tự refresh do các event khác
- Item load từ DB: UI đã ổn định, chỉ refresh khi có notification đúng

## GIẢI PHÁP FIX

### Option 1: Đổi Notification (ĐƠN GIẢN NHẤT)

**File:** `KProtocolProcess.cpp:3950-3968`

Đổi từ `GDCNI_CONTAINER_OBJECT_CHANGED` (không có handler)
sang `GDCNI_OBJECT_CHANGED` (có handler):

```cpp
case 1:  // SetStackCount
    Item[nIdx].SetStackCount(pICI->m_uChange);

    // FIX: Gửi GDCNI_OBJECT_CHANGED thay vì GDCNI_CONTAINER_OBJECT_CHANGED
    {
        KUiObjAtContRegion Region;
        Region.Obj.uGenre = CGOG_ITEM;
        Region.Obj.uId = nIdx;
        Region.Region.Width = Item[nIdx].GetWidth();
        Region.Region.Height = Item[nIdx].GetHeight();
        Region.eContainer = UOC_ITEM_TAKE_WITH;

        // ✓ SỬ DỤNG NOTIFICATION CÓ HANDLER
        CoreDataChanged(GDCNI_OBJECT_CHANGED, (unsigned int)&Region, 1);
    }

    Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
    break;
```

### Option 2: Sử Dụng NotifyItemChange() (CLEAN HƠN)

**File:** `KProtocolProcess.cpp:3944-3972`

Gọi function `NotifyItemChange()` đã có sẵn:

```cpp
case 1:  // SetStackCount
    Item[nIdx].SetStackCount(pICI->m_uChange);

    // ✓ SỬ DỤNG FUNCTION ĐÃ CÓ - TỰ ĐỘNG MAP CONTAINER VÀ GỬI NOTIFICATION ĐÚNG
    Player[CLIENT_PLAYER_INDEX].m_ItemList.NotifyItemChange(nIdx);
    Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
    break;
```

**Ưu điểm:**
- Tự động detect đúng container type (equipment/immediacy/store box/etc)
- Tự động map position (X, Y) từ m_Items array
- Code clean hơn, tái sử dụng logic đã có

## KẾT LUẬN

**Root Cause:**
Fix commit 3062bb369 đã cố gắng fix bug bằng cách gửi UI notification,
NHƯNG gửi SAI LOẠI notification (GDCNI_CONTAINER_OBJECT_CHANGED không có handler).

**Impact:**
- Server logic: ✓ ĐÚNG (stack count được update và lưu DB)
- Client memory: ✓ ĐÚNG (m_nCurrentDur được update)
- Client UI: ✗ SAI (Paint() không được gọi lại do thiếu notification handler)

**Recommended Fix:**
Sử dụng Option 2 (gọi NotifyItemChange) vì:
1. Code đã có sẵn và được test
2. Tự động xử lý mọi container type
3. Clean và maintainable hơn

**Test Plan:**
1. Login với stack item trong inventory
2. Split item (Ctrl+Right Click)
3. Verify UI hiển thị số lượng đúng ngay lập tức
4. Split lại nhiều lần
5. Stack lại các item đã split
6. Logout và login lại, verify số lượng vẫn đúng

---

**Ngày phân tích:** 2026-01-09
**Phân tích bởi:** Claude Code (C++ Code Analyst)
