# GAME SERVER MANAGEMENT SCRIPTS

Các scripts tự động quản lý game server trên Windows Server 2012 R2+

---

## 📁 DANH SÁCH SCRIPTS

| File | Mô tả | Sử dụng |
|------|-------|---------|
| **Setup-Firewall.ps1** | Config Windows Firewall tự động | PowerShell với quyền Admin |
| **Start-GameServer.bat** | Khởi động tất cả game server components | Run as Administrator |
| **Stop-GameServer.bat** | Dừng tất cả game server components | Run as Administrator |
| **Check-GameServer.ps1** | Kiểm tra trạng thái server và ports | PowerShell |

---

## 🚀 HƯỚNG DẪN SỬ DỤNG

### 1. Setup Firewall (Lần đầu tiên)

**Cách 1: Với IP admin cụ thể (KHUYẾN NGHỊ)**
```powershell
# Mở PowerShell với quyền Administrator
Set-ExecutionPolicy Bypass -Scope Process -Force
cd C:\GameServer\Scripts
.\Setup-Firewall.ps1 -AdminIP "203.162.4.191"  # Thay bằng IP thật của bạn
```

**Cách 2: Mở RDP cho tất cả IPs (KHÔNG AN TOÀN)**
```powershell
.\Setup-Firewall.ps1
# Sẽ cảnh báo RDP mở cho all IPs
```

**Kết quả:**
- ✅ Port 5622 mở cho client connections
- ✅ Port 3389 (RDP) chỉ cho IP admin
- ✅ Các ports internal được bảo vệ

---

### 2. Khởi động Game Server

**Cách 1: Dùng batch file (Dễ nhất)**
```cmd
# Right-click → Run as Administrator
Start-GameServer.bat
```

**Cách 2: Thủ công**
```powershell
# Theo thứ tự:
1. Khởi động Database (MySQL/MSSQL) - nếu chưa chạy
2. C:\GameServer\Goddess.exe
3. C:\GameServer\Sword3PaySys.exe
4. C:\GameServer\Bishop.exe
5. C:\GameServer\GS1.exe, GS2.exe, GS3.exe, GS4.exe
```

**Thứ tự khởi động quan trọng:**
```
Database → Goddess → Account → Bishop → Game Servers
```

---

### 3. Dừng Game Server

```cmd
# Right-click → Run as Administrator
Stop-GameServer.bat
```

**Lưu ý:** Script này không dừng database service (MySQL/MSSQL)

---

### 4. Kiểm tra Status

```powershell
# PowerShell (không cần Admin)
cd C:\GameServer\Scripts
.\Check-GameServer.ps1
```

**Output:**
- ✓ Processes đang chạy
- ✓ Ports đang listening
- ✓ Firewall rules status
- ✓ Database service status
- ✓ Client connection test

---

## ⚙️ CUSTOMIZATION

### Thay đổi thư mục game server

**File:** `Start-GameServer.bat` và `Stop-GameServer.bat`

Sửa dòng:
```batch
set GAME_DIR=C:\GameServer
```
Thành:
```batch
set GAME_DIR=D:\MyGameServer
```

### Thay đổi Admin IP cho RDP

**Re-run script:**
```powershell
.\Setup-Firewall.ps1 -AdminIP "NEW_IP_HERE"
```

---

## 🔒 BẢO MẬT

### Ports mở ra Internet:
- ✅ **5622** - Client connection (REQUIRED)
- ⚠️ **3389** - RDP (Nên restrict IP admin)

### Ports internal only:
- 🔒 **5632** - Bishop ↔ GameServer
- 🔒 **5001** - Goddess Role Server
- 🔒 **5002** - Account Server
- 🔒 **3306** - MySQL Database
- 🔒 **1433** - MSSQL Database

---

## 📊 TROUBLESHOOTING

### Vấn đề: Port 5622 không mở

**Kiểm tra:**
```powershell
# 1. Check firewall rule
Get-NetFirewallRule -DisplayName "GameServer - Client Connection (5622)"

# 2. Check port listening
Get-NetTCPConnection -LocalPort 5622

# 3. Check Bishop process
Get-Process -Name Bishop

# 4. Test connection
Test-NetConnection -ComputerName localhost -Port 5622
```

**Giải pháp:**
1. Chạy lại `Setup-Firewall.ps1`
2. Khởi động lại Bishop.exe
3. Kiểm tra logs trong `C:\GameServer\Logs\`

---

### Vấn đề: Client không kết nối được từ bên ngoài

**Kiểm tra:**
```powershell
# From another computer
Test-NetConnection -ComputerName YOUR_VPS_IP -Port 5622
```

**Nguyên nhân có thể:**
1. VPS provider blocking port → Mở port trên VPS panel
2. Windows Firewall blocking → Check firewall rules
3. Bishop không chạy → Restart Bishop.exe
4. Wrong IP trong ServerList.ini → Check client config

---

### Vấn đề: Database connection failed

**Kiểm tra MySQL:**
```powershell
# Check service
Get-Service MySQL*

# Start service
Start-Service MySQL57

# Test connection
mysql -h 127.0.0.1 -u gameserver -p
```

**Kiểm tra MSSQL:**
```powershell
# Check service
Get-Service MSSQL*

# Start service
Start-Service MSSQLSERVER

# Test connection
sqlcmd -S localhost -U gameserver -P YourPassword
```

---

## 📝 LOGS

### Xem logs:
```powershell
# Game server logs (nếu có)
Get-Content C:\GameServer\Logs\*.log -Tail 50

# Windows Event Logs
Get-EventLog -LogName Application -Newest 50 | Where Source -like "*Game*"

# Firewall logs (nếu bật logging)
Get-Content C:\Windows\System32\LogFiles\Firewall\pfirewall.log -Tail 50
```

---

## 🔧 ADVANCED

### Tạo Windows Services (Tự động khởi động khi reboot)

**Install NSSM:**
```powershell
# Download NSSM
Invoke-WebRequest -Uri "https://nssm.cc/release/nssm-2.24.zip" -OutFile "C:\Temp\nssm.zip"
Expand-Archive "C:\Temp\nssm.zip" -DestinationPath "C:\Temp\"
Copy-Item "C:\Temp\nssm-2.24\win64\nssm.exe" -Destination "C:\Windows\System32\"
```

**Create services:**
```powershell
# Goddess Service
nssm install GameServer-Goddess "C:\GameServer\Goddess.exe"
nssm set GameServer-Goddess AppDirectory "C:\GameServer"
nssm set GameServer-Goddess Start SERVICE_AUTO_START

# Account Service
nssm install GameServer-Account "C:\GameServer\Sword3PaySys.exe"
nssm set GameServer-Account AppDirectory "C:\GameServer"
nssm set GameServer-Account Start SERVICE_AUTO_START

# Bishop Service
nssm install GameServer-Bishop "C:\GameServer\Bishop.exe"
nssm set GameServer-Bishop AppDirectory "C:\GameServer"
nssm set GameServer-Bishop Start SERVICE_AUTO_START

# Start all
Start-Service GameServer-Goddess
Start-Sleep -Seconds 5
Start-Service GameServer-Account
Start-Sleep -Seconds 5
Start-Service GameServer-Bishop
```

---

## 📞 SUPPORT

Nếu gặp vấn đề:

1. Chạy `Check-GameServer.ps1` để xem status
2. Kiểm tra logs trong `C:\GameServer\Logs\`
3. Xem Windows Event Viewer → Application Logs
4. Test ports với `Test-NetConnection`

---

**✅ Scripts created:** 2025-11-11
**✅ Tested on:** Windows Server 2012 R2, 2016, 2019, 2022
