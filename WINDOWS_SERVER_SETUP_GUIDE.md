# HƯỚNG DẪN CONFIG VPS WINDOWS SERVER 2012 R2 CHO GAME SERVER

> **Dựa trên phân tích codebase:** Huyền Thiết Kiếm game server architecture
> **Ngày tạo:** 2025-11-11
> **Môi trường:** Windows Server 2012 R2 (64-bit)

---

## 📋 MỤC LỤC

1. [Yêu cầu hệ thống](#1-yêu-cầu-hệ-thống)
2. [Kiến trúc server và ports](#2-kiến-trúc-server-và-ports)
3. [Cài đặt Windows Server](#3-cài-đặt-windows-server)
4. [Config Windows Firewall](#4-config-windows-firewall)
5. [Cài đặt database](#5-cài-đặt-database)
6. [Cài đặt dependencies](#6-cài-đặt-dependencies)
7. [Deploy game server](#7-deploy-game-server)
8. [Troubleshooting](#8-troubleshooting)

---

## 1. YÊU CẦU HỆ THỐNG

### 1.1. Phần cứng tối thiểu (VPS)

| Thành phần | Tối thiểu | Khuyến nghị |
|------------|-----------|-------------|
| **CPU** | 2 cores | 4 cores hoặc hơn |
| **RAM** | 4 GB | 8 GB - 16 GB |
| **HDD** | 50 GB | 100 GB SSD |
| **Network** | 10 Mbps | 100 Mbps |

### 1.2. Phần mềm

- **OS:** Windows Server 2012 R2 Standard/Datacenter (64-bit)
- **Database:** MySQL 5.7+ hoặc Microsoft SQL Server 2012+
- **Runtime:** Visual C++ Redistributable 2010, 2013, 2015-2022
- **.NET Framework:** 4.5+ (có sẵn trong Windows Server 2012 R2)

---

## 2. KIẾN TRÚC SERVER VÀ PORTS

### 2.1. Sơ đồ kiến trúc

```
                    ┌──────────────┐
                    │   INTERNET   │
                    └──────┬───────┘
                           │
                           │ Port 5622 (TCP)
                           ▼
                  ┌────────────────────┐
                  │  Bishop.exe        │◄─── Gateway/Login Server
                  │  (Login Server)    │
                  └─────┬──────┬───────┘
                        │      │
          Port 5002 ────┤      └──── Port 5632
                        │                │
                ┌───────▼──────┐    ┌────▼─────────────┐
                │ Sword3PaySys │    │ GameServer (GS1) │
                │ (Account Srv)│    │ GameServer (GS2) │
                └──────────────┘    │ GameServer (GS3) │
                                    │ GameServer (GS4) │
                                    └──────┬───────────┘
                                           │
                                  Port 5001│
                                           ▼
                                  ┌────────────────┐
                                  │  Goddess.exe   │
                                  │  (Role Server) │
                                  └───────┬────────┘
                                          │
                                 Port 3306/1433
                                          ▼
                                  ┌────────────────┐
                                  │    Database    │
                                  │  MySQL/MSSQL   │
                                  └────────────────┘
```

### 2.2. Danh sách ports cần mở

| Port | Protocol | Service | Direction | Mô tả |
|------|----------|---------|-----------|-------|
| **5622** | TCP | Bishop | **INBOUND** | **Client connection (QUAN TRỌNG - MỞ RA INTERNET)** |
| **5632** | TCP | Bishop | OUTBOUND | Bishop → GameServer admin connection |
| **5002** | TCP | Sword3PaySys | LOCAL | Bishop → Account Server |
| **5001** | TCP | Goddess | LOCAL | Bishop/GameServer → Role/Database Server |
| **3306** | TCP | MySQL | LOCAL | Database connection (nếu dùng MySQL) |
| **1433** | TCP | MSSQL | LOCAL | Database connection (nếu dùng SQL Server) |
| **3389** | TCP | RDP | **INBOUND** | Remote Desktop (admin access) |

> **CHÚ Ý:**
> - ✅ **CHỈ MỞ PORT 5622** ra Internet cho clients kết nối
> - ✅ **PORT 3389** (RDP) chỉ mở cho IP admin (whitelist)
> - ✅ **TẤT CẢ PORTS KHÁC** chỉ listen trên localhost (127.0.0.1) hoặc internal network

---

## 3. CÀI ĐẶT WINDOWS SERVER

### 3.1. Kết nối VPS qua RDP

1. Mở **Remote Desktop Connection** (mstsc.exe)
2. Nhập **IP VPS** và **Port 3389**
3. Đăng nhập với **Administrator** credentials

### 3.2. Cập nhật Windows

```powershell
# Mở PowerShell với quyền Administrator
# Check for updates
sconfig
# → Chọn option 6: Download and Install Updates
# → Chọn A: All Updates
# → Khởi động lại sau khi cập nhật xong
```

### 3.3. Tắt User Account Control (UAC)

1. **Control Panel** → **User Accounts** → **Change User Account Control settings**
2. Kéo thanh trượt xuống **"Never notify"**
3. Click **OK** → **Restart**

### 3.4. Config Network Adapter

1. **Control Panel** → **Network and Sharing Center**
2. Click **Change adapter settings**
3. Right-click **Ethernet** → **Properties**
4. Chọn **Internet Protocol Version 4 (TCP/IPv4)** → **Properties**
5. Thiết lập:
   ```
   IP address: [IP tĩnh VPS]
   Subnet mask: 255.255.255.0
   Default gateway: [Gateway của VPS provider]
   Preferred DNS: 8.8.8.8
   Alternate DNS: 8.8.4.4
   ```
6. Click **OK** → **Close**

---

## 4. CONFIG WINDOWS FIREWALL

### 4.1. Phương pháp 1: Sử dụng PowerShell (KHUYẾN NGHỊ)

**Tạo file script:** `C:\GameServer\Setup-Firewall.ps1`

```powershell
# ============================================================
# GAME SERVER FIREWALL CONFIGURATION SCRIPT
# Windows Server 2012 R2
# ============================================================

# BƯỚC 1: Enable Windows Firewall
Write-Host "Enabling Windows Firewall..." -ForegroundColor Yellow
Set-NetFirewallProfile -Profile Domain,Public,Private -Enabled True

# BƯỚC 2: Thiết lập Default Policy (Block tất cả, chỉ Allow những gì cần)
Write-Host "Setting default firewall policy..." -ForegroundColor Yellow
Set-NetFirewallProfile -DefaultInboundAction Block -DefaultOutboundAction Allow

# BƯỚC 3: XÓA CÁC RULES CŨ (nếu có)
Write-Host "Removing old game server firewall rules..." -ForegroundColor Yellow
Remove-NetFirewallRule -DisplayName "GameServer - Client Connection (5622)" -ErrorAction SilentlyContinue
Remove-NetFirewallRule -DisplayName "GameServer - RDP Access" -ErrorAction SilentlyContinue
Remove-NetFirewallRule -DisplayName "GameServer - Internal Bishop-GameServer" -ErrorAction SilentlyContinue
Remove-NetFirewallRule -DisplayName "GameServer - Internal Goddess" -ErrorAction SilentlyContinue
Remove-NetFirewallRule -DisplayName "GameServer - Internal Account" -ErrorAction SilentlyContinue
Remove-NetFirewallRule -DisplayName "GameServer - MySQL Database" -ErrorAction SilentlyContinue
Remove-NetFirewallRule -DisplayName "GameServer - MSSQL Database" -ErrorAction SilentlyContinue

# BƯỚC 4: MỞ PORT 5622 - CLIENT CONNECTION (PUBLIC)
Write-Host "Opening port 5622 for client connections..." -ForegroundColor Green
New-NetFirewallRule -DisplayName "GameServer - Client Connection (5622)" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 5622 `
    -Action Allow `
    -Profile Any `
    -Description "Allow game clients to connect to Bishop login server"

# BƯỚC 5: MỞ PORT 3389 - RDP (RESTRICTED TO ADMIN IP)
# *** THAY ĐỔI "YOUR_ADMIN_IP" BẰNG IP ADMIN THẬT ***
$AdminIP = "0.0.0.0/0"  # ← THAY ĐỔI NÀY! Ví dụ: "203.162.4.191"

Write-Host "Opening port 3389 for RDP (restricted to $AdminIP)..." -ForegroundColor Green
New-NetFirewallRule -DisplayName "GameServer - RDP Access" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 3389 `
    -RemoteAddress $AdminIP `
    -Action Allow `
    -Profile Any `
    -Description "Allow RDP from admin IP only"

# BƯỚC 6: MỞ PORTS INTERNAL (Chỉ localhost hoặc internal network)
# Port 5632: Bishop → GameServer
Write-Host "Opening internal port 5632 (Bishop-GameServer)..." -ForegroundColor Cyan
New-NetFirewallRule -DisplayName "GameServer - Internal Bishop-GameServer" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 5632 `
    -RemoteAddress LocalSubnet `
    -Action Allow `
    -Profile Private `
    -Description "Internal communication between Bishop and GameServers"

# Port 5001: Goddess Role Server
Write-Host "Opening internal port 5001 (Goddess)..." -ForegroundColor Cyan
New-NetFirewallRule -DisplayName "GameServer - Internal Goddess" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 5001 `
    -RemoteAddress LocalSubnet `
    -Action Allow `
    -Profile Private `
    -Description "Internal Goddess role server"

# Port 5002: Account Server
Write-Host "Opening internal port 5002 (Account)..." -ForegroundColor Cyan
New-NetFirewallRule -DisplayName "GameServer - Internal Account" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 5002 `
    -RemoteAddress LocalSubnet `
    -Action Allow `
    -Profile Private `
    -Description "Internal account authentication server"

# Port 3306: MySQL Database (nếu dùng)
Write-Host "Opening internal port 3306 (MySQL)..." -ForegroundColor Cyan
New-NetFirewallRule -DisplayName "GameServer - MySQL Database" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 3306 `
    -RemoteAddress LocalSubnet `
    -Action Allow `
    -Profile Private `
    -Description "MySQL database access (localhost only)"

# Port 1433: MSSQL Database (nếu dùng)
Write-Host "Opening internal port 1433 (MSSQL)..." -ForegroundColor Cyan
New-NetFirewallRule -DisplayName "GameServer - MSSQL Database" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 1433 `
    -RemoteAddress LocalSubnet `
    -Action Allow `
    -Profile Private `
    -Description "Microsoft SQL Server database access (localhost only)"

# BƯỚC 7: BLOCK ICMP PING (Tùy chọn - tăng bảo mật)
# Uncomment dòng dưới nếu muốn block ping
# New-NetFirewallRule -DisplayName "Block ICMP Ping" -Direction Inbound -Protocol ICMPv4 -Action Block

# BƯỚC 8: HIỂN THỊ CÁC RULES VỪA TẠO
Write-Host "`n============================================" -ForegroundColor Yellow
Write-Host "FIREWALL RULES CREATED:" -ForegroundColor Yellow
Write-Host "============================================`n" -ForegroundColor Yellow
Get-NetFirewallRule | Where-Object {$_.DisplayName -like "GameServer*"} | Select-Object DisplayName, Direction, Action, Enabled | Format-Table -AutoSize

Write-Host "`n✅ Firewall configuration completed!" -ForegroundColor Green
Write-Host "⚠️  IMPORTANT: Verify rules and restart server if needed.`n" -ForegroundColor Yellow
```

**Chạy script:**

```powershell
# Mở PowerShell với quyền Administrator
Set-ExecutionPolicy Bypass -Scope Process -Force
cd C:\GameServer
.\Setup-Firewall.ps1
```

### 4.2. Phương pháp 2: Sử dụng GUI (Windows Firewall with Advanced Security)

#### Bước 1: Mở Windows Firewall

1. **Start** → **Windows Firewall with Advanced Security**
2. Hoặc gõ `wf.msc` trong Run (Win+R)

#### Bước 2: Tạo Inbound Rule cho Port 5622 (Client Connection)

1. Click **Inbound Rules** ở panel bên trái
2. Click **New Rule...** ở panel bên phải
3. **Rule Type:** Chọn **Port** → Next
4. **Protocol and Ports:**
   - Chọn **TCP**
   - **Specific local ports:** Nhập `5622`
   - Next
5. **Action:** Chọn **Allow the connection** → Next
6. **Profile:** Chọn **Domain, Private, Public** → Next
7. **Name:** Nhập `GameServer - Client Connection (5622)`
8. **Description:** `Allow game clients to connect to Bishop login server`
9. Click **Finish**

#### Bước 3: Tạo Inbound Rule cho Port 3389 (RDP - Restricted)

1. New Rule → **Port** → Next
2. **TCP** → **Specific local ports:** `3389` → Next
3. **Allow the connection** → Next
4. **Domain, Private, Public** → Next
5. **Name:** `GameServer - RDP Access`
6. **Finish**
7. **Right-click** rule vừa tạo → **Properties**
8. Tab **Scope** → **Remote IP address:** Chọn **These IP addresses**
9. Click **Add...** → Nhập IP admin (ví dụ: `203.162.4.191`) → OK
10. **Apply** → **OK**

#### Bước 4: Tạo Inbound Rules cho Internal Ports (5632, 5001, 5002, 3306, 1433)

Lặp lại các bước trên cho từng port:

| Port | Name | Scope - Remote IP |
|------|------|-------------------|
| 5632 | GameServer - Internal Bishop-GameServer | LocalSubnet |
| 5001 | GameServer - Internal Goddess | LocalSubnet |
| 5002 | GameServer - Internal Account | LocalSubnet |
| 3306 | GameServer - MySQL Database | LocalSubnet |
| 1433 | GameServer - MSSQL Database | LocalSubnet |

**CHÚ Ý:** Trong tab **Scope**, chọn **Remote IP address:** → **These IP addresses** → Add: **LocalSubnet**

### 4.3. Kiểm tra Firewall Rules

```powershell
# Xem tất cả Inbound rules đã tạo
Get-NetFirewallRule | Where-Object {$_.DisplayName -like "GameServer*"} | Format-Table DisplayName, Direction, Action, Enabled

# Kiểm tra port 5622 có mở không
Test-NetConnection -ComputerName localhost -Port 5622

# Kiểm tra từ máy khác (thay YOUR_VPS_IP)
Test-NetConnection -ComputerName YOUR_VPS_IP -Port 5622
```

---

## 5. CÀI ĐẶT DATABASE

### 5.1. Option 1: MySQL Server 5.7

#### Download và cài đặt:

1. Download MySQL 5.7 từ: https://dev.mysql.com/downloads/mysql/5.7.html
2. Chọn **Windows (x86, 64-bit), MSI Installer**
3. Chạy `mysql-installer-community-5.7.x.x.msi`
4. **Setup Type:** Chọn **Server only** → Next
5. **Installation** → Execute
6. **Product Configuration:**
   - **Config Type:** Development Computer
   - **Port:** `3306`
   - **Root Password:** Đặt password mạnh (ví dụ: `GameServer@2024!`)
   - Bỏ tick **"Open Windows Firewall port for network access"** (đã config ở trên)
7. **Apply Configuration** → Finish

#### Config MySQL:

```sql
-- Kết nối MySQL
mysql -u root -p

-- Tạo database
CREATE DATABASE IF NOT EXISTS `jxonline` CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

-- Tạo user cho game server
CREATE USER 'gameserver'@'localhost' IDENTIFIED BY 'GamePass@2024!';
GRANT ALL PRIVILEGES ON jxonline.* TO 'gameserver'@'localhost';
FLUSH PRIVILEGES;

-- Kiểm tra
SHOW DATABASES;
SELECT user, host FROM mysql.user;
EXIT;
```

#### Tối ưu hóa MySQL cho game server:

Sửa file `C:\ProgramData\MySQL\MySQL Server 5.7\my.ini`:

```ini
[mysqld]
# Performance Tuning
max_connections=500
max_allowed_packet=64M
innodb_buffer_pool_size=2G  # 50% RAM nếu có 4GB
innodb_log_file_size=256M
innodb_flush_log_at_trx_commit=2
query_cache_size=128M

# Character Set
character-set-server=utf8mb4
collation-server=utf8mb4_unicode_ci

# Bind to localhost only (security)
bind-address=127.0.0.1
```

Restart MySQL service:
```powershell
Restart-Service MySQL57
```

### 5.2. Option 2: Microsoft SQL Server 2012+

#### Download và cài đặt:

1. Download SQL Server 2012/2014/2016/2019 Express từ Microsoft
2. Chạy installer → **New SQL Server stand-alone installation**
3. **Feature Selection:** Chọn **Database Engine Services**
4. **Instance Configuration:** Chọn **Default instance**
5. **Server Configuration:**
   - SQL Server Database Engine: Set to **Automatic**
6. **Database Engine Configuration:**
   - **Authentication Mode:** Chọn **Mixed Mode**
   - **SA Password:** Đặt password mạnh
   - **Add Current User**
7. Install

#### Config SQL Server:

```sql
-- Kết nối SQL Server
sqlcmd -S localhost -U sa -P YourSAPassword

-- Tạo database
CREATE DATABASE jxonline;
GO

-- Tạo login cho game server
CREATE LOGIN gameserver WITH PASSWORD = 'GamePass@2024!';
GO

USE jxonline;
CREATE USER gameserver FOR LOGIN gameserver;
ALTER ROLE db_owner ADD MEMBER gameserver;
GO

EXIT
```

#### Enable TCP/IP Protocol:

1. **SQL Server Configuration Manager**
2. **SQL Server Network Configuration** → **Protocols for MSSQLSERVER**
3. Right-click **TCP/IP** → **Enable**
4. **Properties** → Tab **IP Addresses**
5. **IPAll** → **TCP Port:** `1433`
6. Restart **SQL Server (MSSQLSERVER)** service

---

## 6. CÀI ĐẶT DEPENDENCIES

### 6.1. Visual C++ Redistributables

Download và cài đặt tất cả các phiên bản:

```powershell
# Tạo thư mục tạm
mkdir C:\Temp\VCRedist
cd C:\Temp\VCRedist

# Download VC++ 2010 x86 + x64
Invoke-WebRequest -Uri "https://download.microsoft.com/download/1/6/5/165255E7-1014-4D0A-B094-B6A430A6BFFC/vcredist_x86.exe" -OutFile "vcredist_2010_x86.exe"
Invoke-WebRequest -Uri "https://download.microsoft.com/download/1/6/5/165255E7-1014-4D0A-B094-B6A430A6BFFC/vcredist_x64.exe" -OutFile "vcredist_2010_x64.exe"

# Download VC++ 2013 x86 + x64
Invoke-WebRequest -Uri "https://download.microsoft.com/download/2/E/6/2E61CFA4-993B-4DD4-91DA-3737CD5CD6E3/vcredist_x86.exe" -OutFile "vcredist_2013_x86.exe"
Invoke-WebRequest -Uri "https://download.microsoft.com/download/2/E/6/2E61CFA4-993B-4DD4-91DA-3737CD5CD6E3/vcredist_x64.exe" -OutFile "vcredist_2013_x64.exe"

# Download VC++ 2015-2022 x86 + x64
Invoke-WebRequest -Uri "https://aka.ms/vs/17/release/vc_redist.x86.exe" -OutFile "vcredist_2015_2022_x86.exe"
Invoke-WebRequest -Uri "https://aka.ms/vs/17/release/vc_redist.x64.exe" -OutFile "vcredist_2015_2022_x64.exe"

# Cài đặt tất cả (silent mode)
.\vcredist_2010_x86.exe /q /norestart
.\vcredist_2010_x64.exe /q /norestart
.\vcredist_2013_x86.exe /q /norestart
.\vcredist_2013_x64.exe /q /norestart
.\vcredist_2015_2022_x86.exe /q /norestart
.\vcredist_2015_2022_x64.exe /q /norestart

Write-Host "✅ All Visual C++ Redistributables installed!" -ForegroundColor Green
```

### 6.2. .NET Framework 4.5+ (có sẵn trong Windows Server 2012 R2)

Kiểm tra:
```powershell
Get-ItemProperty "HKLM:\SOFTWARE\Microsoft\NET Framework Setup\NDP\v4\Full" | Select-Object Version, Release
```

---

## 7. DEPLOY GAME SERVER

### 7.1. Tạo thư mục cài đặt

```powershell
# Tạo thư mục chính
New-Item -Path "C:\GameServer" -ItemType Directory -Force
New-Item -Path "C:\GameServer\Logs" -ItemType Directory -Force
New-Item -Path "C:\GameServer\Backups" -ItemType Directory -Force

# Set permissions
icacls "C:\GameServer" /grant "Everyone:(OI)(CI)F" /T
```

### 7.2. Upload game server files

**Sử dụng WinSCP hoặc RDP để copy files:**

```
C:\GameServer\
├── Bishop.exe
├── Bishop.cfg
├── Goddess.exe
├── Goddess.cfg
├── Sword3PaySys.exe
├── GameServer.exe (hoặc GS1.exe, GS2.exe, GS3.exe, GS4.exe)
├── CoreServer.dll
├── Engine.dll
├── Rainbow.dll
├── Heaven.dll
├── FilterText.dll
└── Settings\
    └── (các file .ini, .txt, scripts)
```

### 7.3. Config Bishop.cfg

Sửa file `C:\GameServer\Bishop.cfg`:

```ini
[Network]
AccSvrIP=127.0.0.1
AccSvrPort=5002
RoleSvrIP=127.0.0.1
RoleSvrPort=5001
ClientOpenPort=5622
GameSvrIP=127.0.0.1
GameSvrOpenPort=5632
```

### 7.4. Config Goddess.cfg

Sửa file `C:\GameServer\Goddess.cfg`:

```ini
[Setting]
Port=5001
MaxRoleCount=3
BackupSleepTime=3
AutoBackupEnabled=1
AutoBackupMinutes=10
```

### 7.5. Config Database Connection

**Nếu dùng MySQL:**

Sửa file database config (kiểm tra trong Settings folder hoặc GameServer config):

```ini
[Database]
Type=MySQL
Host=127.0.0.1
Port=3306
Database=jxonline
User=gameserver
Password=GamePass@2024!
```

**Nếu dùng MSSQL:**

```ini
[Database]
Type=MSSQL
Host=127.0.0.1
Port=1433
Database=jxonline
User=gameserver
Password=GamePass@2024!
```

### 7.6. Tạo Windows Services (Tùy chọn)

Tạo file `Install-Services.ps1`:

```powershell
# Install NSSM (Non-Sucking Service Manager)
Invoke-WebRequest -Uri "https://nssm.cc/release/nssm-2.24.zip" -OutFile "C:\Temp\nssm.zip"
Expand-Archive "C:\Temp\nssm.zip" -DestinationPath "C:\Temp\"
Copy-Item "C:\Temp\nssm-2.24\win64\nssm.exe" -Destination "C:\Windows\System32\"

# Create services
nssm install GameServer-Bishop "C:\GameServer\Bishop.exe"
nssm set GameServer-Bishop AppDirectory "C:\GameServer"
nssm set GameServer-Bishop Start SERVICE_AUTO_START

nssm install GameServer-Goddess "C:\GameServer\Goddess.exe"
nssm set GameServer-Goddess AppDirectory "C:\GameServer"
nssm set GameServer-Goddess Start SERVICE_AUTO_START

nssm install GameServer-Account "C:\GameServer\Sword3PaySys.exe"
nssm set GameServer-Account AppDirectory "C:\GameServer"
nssm set GameServer-Account Start SERVICE_AUTO_START

# Start services
Start-Service GameServer-Goddess
Start-Sleep -Seconds 5
Start-Service GameServer-Account
Start-Sleep -Seconds 5
Start-Service GameServer-Bishop

Write-Host "✅ All game server services installed and started!" -ForegroundColor Green
```

### 7.7. Khởi động server thủ công (Test)

```powershell
# Khởi động theo thứ tự:
# 1. Database (đã chạy)
# 2. Goddess (Role Server)
cd C:\GameServer
Start-Process ".\Goddess.exe"

# Đợi 5 giây
Start-Sleep -Seconds 5

# 3. Account Server
Start-Process ".\Sword3PaySys.exe"

# Đợi 5 giây
Start-Sleep -Seconds 5

# 4. Bishop (Login Server)
Start-Process ".\Bishop.exe"

# Đợi 5 giây
Start-Sleep -Seconds 5

# 5. Game Servers (nếu có)
Start-Process ".\GS1.exe"
Start-Process ".\GS2.exe"
```

---

## 8. TROUBLESHOOTING

### 8.1. Kiểm tra Ports đã mở chưa

```powershell
# Kiểm tra port 5622
netstat -an | findstr :5622

# Kiểm tra tất cả listening ports
netstat -an | findstr LISTENING

# Test từ máy khác (thay VPS_IP)
Test-NetConnection -ComputerName VPS_IP -Port 5622
```

### 8.2. Kiểm tra Process đang chạy

```powershell
# Xem Bishop.exe có chạy không
Get-Process | Where-Object {$_.Name -like "*Bishop*"}

# Xem tất cả game server processes
Get-Process | Where-Object {$_.Name -like "*GameServer*" -or $_.Name -like "*Goddess*" -or $_.Name -like "*Bishop*"}
```

### 8.3. Kiểm tra Logs

```powershell
# Xem Windows Event Logs
Get-EventLog -LogName Application -Newest 50 | Where-Object {$_.Source -like "*Game*"}

# Xem game server logs (nếu có)
Get-Content "C:\GameServer\Logs\*.log" -Tail 50
```

### 8.4. Firewall Blocking Issue

**Triệu chứng:** Client không connect được vào port 5622

**Giải pháp:**

```powershell
# Tắt firewall tạm thời để test (KHÔNG KHUYẾN NGHỊ cho production)
Set-NetFirewallProfile -Profile Domain,Public,Private -Enabled False

# Test client connection
# Nếu connect được → vấn đề là firewall rules
# Nếu vẫn không connect được → vấn đề khác (server không chạy, port binding failed)

# Bật lại firewall
Set-NetFirewallProfile -Profile Domain,Public,Private -Enabled True

# Kiểm tra rule 5622 có enabled không
Get-NetFirewallRule -DisplayName "GameServer - Client Connection (5622)" | Select-Object DisplayName, Enabled, Action

# Enable rule nếu bị disable
Enable-NetFirewallRule -DisplayName "GameServer - Client Connection (5622)"
```

### 8.5. Database Connection Failed

**Triệu chứng:** Server không kết nối được database

**Giải pháp:**

```powershell
# Test MySQL connection
mysql -h 127.0.0.1 -P 3306 -u gameserver -p

# Test MSSQL connection
sqlcmd -S localhost,1433 -U gameserver -P GamePass@2024!

# Kiểm tra service đang chạy
Get-Service | Where-Object {$_.Name -like "*MySQL*" -or $_.Name -like "*SQL*"}

# Start service nếu bị tắt
Start-Service MySQL57  # hoặc MSSQLSERVER
```

### 8.6. Port Already in Use

**Triệu chứng:** Bishop.exe báo lỗi "Port 5622 already in use"

**Giải pháp:**

```powershell
# Tìm process đang sử dụng port 5622
netstat -ano | findstr :5622

# Lấy PID (cột cuối cùng), ví dụ PID = 1234
# Kill process
Stop-Process -Id 1234 -Force

# Hoặc xem process name
Get-Process -Id 1234

# Kill by name
Stop-Process -Name "Bishop" -Force
```

---

## 9. CHECKLIST CUỐI CÙNG

Trước khi cho server vào production, kiểm tra:

- [ ] Windows Server 2012 R2 đã update đầy đủ
- [ ] Firewall đã config đúng (chỉ mở port 5622 ra Internet)
- [ ] Port 3389 (RDP) chỉ cho phép IP admin
- [ ] Database đã cài đặt và test kết nối thành công
- [ ] Visual C++ Redistributables đã cài đầy đủ
- [ ] Game server files đã upload đầy đủ
- [ ] Config files (Bishop.cfg, Goddess.cfg) đã chỉnh đúng IP/Port
- [ ] Database connection config đã đúng
- [ ] Services/Processes đã chạy theo đúng thứ tự
- [ ] Test client connection từ máy ngoài thành công
- [ ] Logs không có error nghiêm trọng
- [ ] Backup database và config files định kỳ

---

## 10. BẢO MẬT BỔ SUNG (KHUYẾN NGHỊ)

### 10.1. Thay đổi RDP Port (Tránh bot scan)

```powershell
# Đổi RDP port từ 3389 sang 33890
Set-ItemProperty -Path 'HKLM:\System\CurrentControlSet\Control\Terminal Server\WinStations\RDP-Tcp' -Name PortNumber -Value 33890

# Restart service
Restart-Service TermService -Force

# Update firewall rule
Set-NetFirewallRule -DisplayName "GameServer - RDP Access" -LocalPort 33890
```

### 10.2. Cài đặt Fail2Ban for Windows (CSF/LFD alternative)

Download **EvlWatcher** hoặc **IPBan**:
- https://evlwatcher.com/
- https://github.com/DigitalRuby/IPBan

Config để tự động block IP sau N lần login RDP thất bại.

### 10.3. Disable Unnecessary Services

```powershell
# Tắt các services không cần thiết
Stop-Service -Name "Print Spooler" -Force
Set-Service -Name "Print Spooler" -StartupType Disabled

# List tất cả running services
Get-Service | Where-Object {$_.Status -eq "Running"} | Select-Object Name, DisplayName
```

---

## 📞 HỖ TRỢ

Nếu gặp vấn đề, kiểm tra:

1. **Logs:** `C:\GameServer\Logs\`
2. **Windows Event Viewer:** Application Logs
3. **Firewall Logs:** `C:\Windows\System32\LogFiles\Firewall\`
4. **Database Logs:** MySQL/MSSQL error logs

---

**✅ HOÀN TẤT!** VPS Windows Server 2012 R2 đã sẵn sàng cho game server.
