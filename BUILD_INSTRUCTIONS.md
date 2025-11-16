# Build Instructions - NPC Follow Implementation

## Vấn đề hiện tại

~~Tiêu xa spawn được nhưng **không di chuyển theo player**~~ ✅ FIXED

**Update mới nhất (2025-11-16 - Commit 1fcfc2ce):**
- ✅ SetNpcOwner C++ function đã được implement
- ✅ ProcessAIType08() AI mode đã được viết
- ✅ **BUG FIX**: Fixed cart freezing when spawned at player position
  - **Triệu chứng**: Tiêu xa spawn nhưng kẹt dính với player, không di chuyển được
  - **Nguyên nhân**: AI gửi lệnh `do_stand` khi distance = 0, làm NPC đứng yên mãi mãi
  - **Sửa**: Loại bỏ lệnh `do_stand`, để NPC idle tự nhiên khi ở rất gần player

**YÊU CẦU**: Server phải được rebuild với code C++ mới để fix hoạt động!

## Files đã thay đổi (cần rebuild)

### C++ Code (Core.dll / GameServer.exe):
1. `SwordOnline/Sources/Core/Src/ScriptFuns.cpp` - Added LuaSetNpcOwner()
2. `SwordOnline/Sources/Core/Src/KNpcAI.cpp` - Added ProcessAIType08() + **FIXED stuck bug**
3. `SwordOnline/Sources/Core/Src/KNpcAI.h` - Added ProcessAIType08() declaration

**Chi tiết fix mới nhất (KNpcAI.cpp:1784-1795):**
- Threshold cũ: 2 tiles (4096 pixels²) → Mới: 1.5 tiles (2304 pixels²)
- Loại bỏ lệnh `SendCommand(do_stand)` khi ở gần
- Logic mới:
  - Distance > 5 tiles (25600): Walk to player
  - Distance 1.5-5 tiles (2304-25600): Keep following
  - Distance < 1.5 tiles: Idle naturally (không force stand)

## Cách Build (Windows)

### Option 1: Visual Studio 6.0 (Recommended)

```batch
1. Mở Visual Studio 6.0
2. File > Open Workspace
3. Chọn: HuyenThietKiem\SwordOnline\Sources\MultiServer\GameServer\GameServer.dsw
4. Build > Set Active Configuration > "GameServer - Win32 ServerRelease"
5. Build > Rebuild All (F7)
6. Copy file: GameServer\ServerRelease\GameServer.exe
   → HuyenThietKiem\Bin\Server\GameServer.exe
7. Restart server
```

### Option 2: Build Core.dll riêng

```batch
1. Mở: HuyenThietKiem\SwordOnline\Sources\Core\Core.dsw
2. Build > Set Active Configuration > "Core - Win32 ServerRelease"
3. Build > Rebuild All
4. Copy: Core\ServerRelease\CoreServer.dll
   → HuyenThietKiem\Bin\Server\CoreServer.dll
5. Restart server
```

## Kiểm tra sau khi build

### Test 1: Check function tồn tại
Khi nhận tiêu xa, xem log có message:
```
Debug: Calling SetNpcOwner with NpcIdx=xxx, Name=xxx
Debug: After SetNpcOwner call
```

Nếu **KHÔNG** thấy "After SetNpcOwner call" → Function chưa được build

### Test 2: Check AI mode
Xe phải **tự động đi theo player** khi di chuyển

### Test 3: Reset function
Chọn option "Reset tiêu xa (Test - Miễn phí)" phải spawn xe mới ở vị trí player

## Troubleshooting

### Lỗi: Build failed - missing dependencies
→ Cài Visual C++ 6.0 Service Pack 6
→ Kiểm tra stlport đã compile chưa

### Lỗi: LNK2001 unresolved external symbol
→ Clean solution và Rebuild All
→ Kiểm tra file .cpp có trong project không

### Lỗi: Xe vẫn không di chuyển sau khi build
→ Kiểm tra file GameServer.exe có timestamp mới không
→ Stop server hoàn toàn trước khi copy file
→ Restart tất cả server processes

### Lỗi: Xe spawn được nhưng kẹt dính với player (FIXED trong commit 1fcfc2ce)
**Triệu chứng**: Tiêu xa xuất hiện nhưng player không thể di chuyển, NPC dính chặt
**Nguyên nhân**: ProcessAIType08() gửi lệnh do_stand khi distance = 0
**Fix**: Rebuild với code mới nhất (sau commit 1fcfc2ce)
**Test**: Sau khi rebuild, thử nhận tiêu xa và đi xa → xe phải tự follow

## Alternative: Build trên Linux với Wine

```bash
# Cài Wine và VC6
sudo apt install wine wine64

# Build (nếu có VC6 setup)
wine "C:\Program Files\Microsoft Visual Studio\VC98\Bin\vcvars32.bat"
wine "C:\Program Files\Microsoft Visual Studio\VC98\Bin\nmake.exe" /f GameServer.mak

# Copy file build xong
cp GameServer.exe /path/to/Bin/Server/
```

## Summary

**Quan trọng**: Phải rebuild GameServer.exe hoặc CoreServer.dll với code C++ mới thì SetNpcOwner mới hoạt động!

Nếu không build được, hãy gửi code cho developer có Visual Studio 6.0 để build giúp.

---
Created: 2025-11-16
For: HuyenThietKiem VanTieu Event Implementation
