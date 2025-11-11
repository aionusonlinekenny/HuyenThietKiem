# ============================================================
# GAME SERVER FIREWALL CONFIGURATION SCRIPT
# Windows Server 2012 R2 / 2016 / 2019 / 2022
# Huyền Thiết Kiếm Game Server
# ============================================================

param(
    [string]$AdminIP = "0.0.0.0/0"  # Thay đổi thành IP admin thật, ví dụ: "203.162.4.191"
)

Write-Host "============================================" -ForegroundColor Cyan
Write-Host "  GAME SERVER FIREWALL SETUP" -ForegroundColor Cyan
Write-Host "============================================`n" -ForegroundColor Cyan

# BƯỚC 1: Enable Windows Firewall
Write-Host "[1/8] Enabling Windows Firewall..." -ForegroundColor Yellow
Set-NetFirewallProfile -Profile Domain,Public,Private -Enabled True
Write-Host "      ✓ Firewall enabled" -ForegroundColor Green

# BƯỚC 2: Thiết lập Default Policy (Block tất cả, chỉ Allow những gì cần)
Write-Host "`n[2/8] Setting default firewall policy..." -ForegroundColor Yellow
Set-NetFirewallProfile -DefaultInboundAction Block -DefaultOutboundAction Allow
Write-Host "      ✓ Default policy: Block inbound, Allow outbound" -ForegroundColor Green

# BƯỚC 3: XÓA CÁC RULES CŨ (nếu có)
Write-Host "`n[3/8] Removing old game server firewall rules..." -ForegroundColor Yellow
$rulesToRemove = @(
    "GameServer - Client Connection (5622)",
    "GameServer - RDP Access",
    "GameServer - Internal Bishop-GameServer",
    "GameServer - Internal Goddess",
    "GameServer - Internal Account",
    "GameServer - MySQL Database",
    "GameServer - MSSQL Database"
)

foreach ($rule in $rulesToRemove) {
    Remove-NetFirewallRule -DisplayName $rule -ErrorAction SilentlyContinue
}
Write-Host "      ✓ Old rules removed" -ForegroundColor Green

# BƯỚC 4: MỞ PORT 5622 - CLIENT CONNECTION (PUBLIC)
Write-Host "`n[4/8] Opening port 5622 for client connections..." -ForegroundColor Yellow
New-NetFirewallRule -DisplayName "GameServer - Client Connection (5622)" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 5622 `
    -Action Allow `
    -Profile Any `
    -Description "Allow game clients to connect to Bishop login server" | Out-Null
Write-Host "      ✓ Port 5622 opened (PUBLIC - Client access)" -ForegroundColor Green

# BƯỚC 5: MỞ PORT 3389 - RDP (RESTRICTED TO ADMIN IP)
Write-Host "`n[5/8] Opening port 3389 for RDP..." -ForegroundColor Yellow
if ($AdminIP -eq "0.0.0.0/0") {
    Write-Host "      ⚠ WARNING: RDP is open to ALL IPs (0.0.0.0/0)" -ForegroundColor Red
    Write-Host "      ⚠ Please change -AdminIP parameter to your actual IP!" -ForegroundColor Red
    New-NetFirewallRule -DisplayName "GameServer - RDP Access" `
        -Direction Inbound `
        -Protocol TCP `
        -LocalPort 3389 `
        -Action Allow `
        -Profile Any `
        -Description "Allow RDP from any IP (INSECURE - CHANGE THIS!)" | Out-Null
} else {
    Write-Host "      ✓ RDP restricted to IP: $AdminIP" -ForegroundColor Green
    New-NetFirewallRule -DisplayName "GameServer - RDP Access" `
        -Direction Inbound `
        -Protocol TCP `
        -LocalPort 3389 `
        -RemoteAddress $AdminIP `
        -Action Allow `
        -Profile Any `
        -Description "Allow RDP from admin IP: $AdminIP" | Out-Null
}

# BƯỚC 6: MỞ PORTS INTERNAL (Chỉ localhost hoặc internal network)
Write-Host "`n[6/8] Opening internal communication ports..." -ForegroundColor Yellow

# Port 5632: Bishop → GameServer
New-NetFirewallRule -DisplayName "GameServer - Internal Bishop-GameServer" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 5632 `
    -RemoteAddress LocalSubnet `
    -Action Allow `
    -Profile Private `
    -Description "Internal communication between Bishop and GameServers" | Out-Null
Write-Host "      ✓ Port 5632 opened (Bishop-GameServer)" -ForegroundColor Green

# Port 5001: Goddess Role Server
New-NetFirewallRule -DisplayName "GameServer - Internal Goddess" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 5001 `
    -RemoteAddress LocalSubnet `
    -Action Allow `
    -Profile Private `
    -Description "Internal Goddess role server" | Out-Null
Write-Host "      ✓ Port 5001 opened (Goddess Role Server)" -ForegroundColor Green

# Port 5002: Account Server
New-NetFirewallRule -DisplayName "GameServer - Internal Account" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 5002 `
    -RemoteAddress LocalSubnet `
    -Action Allow `
    -Profile Private `
    -Description "Internal account authentication server" | Out-Null
Write-Host "      ✓ Port 5002 opened (Account Server)" -ForegroundColor Green

# Port 3306: MySQL Database (nếu dùng)
New-NetFirewallRule -DisplayName "GameServer - MySQL Database" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 3306 `
    -RemoteAddress LocalSubnet `
    -Action Allow `
    -Profile Private `
    -Description "MySQL database access (localhost only)" | Out-Null
Write-Host "      ✓ Port 3306 opened (MySQL)" -ForegroundColor Green

# Port 1433: MSSQL Database (nếu dùng)
New-NetFirewallRule -DisplayName "GameServer - MSSQL Database" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 1433 `
    -RemoteAddress LocalSubnet `
    -Action Allow `
    -Profile Private `
    -Description "Microsoft SQL Server database access (localhost only)" | Out-Null
Write-Host "      ✓ Port 1433 opened (MSSQL)" -ForegroundColor Green

# BƯỚC 7: TEST PORTS
Write-Host "`n[7/8] Testing firewall configuration..." -ForegroundColor Yellow
$portsToTest = @(5622, 3389, 5632, 5001, 5002, 3306, 1433)
foreach ($port in $portsToTest) {
    $listening = Get-NetTCPConnection -LocalPort $port -ErrorAction SilentlyContinue
    if ($listening) {
        Write-Host "      ✓ Port $port is listening" -ForegroundColor Green
    } else {
        Write-Host "      ⚠ Port $port not listening (service may not be started yet)" -ForegroundColor DarkYellow
    }
}

# BƯỚC 8: HIỂN THỊ CÁC RULES VỪA TẠO
Write-Host "`n[8/8] Firewall rules summary:" -ForegroundColor Yellow
Write-Host "============================================" -ForegroundColor Cyan
Get-NetFirewallRule | Where-Object {$_.DisplayName -like "GameServer*"} |
    Select-Object @{Name="Rule Name";Expression={$_.DisplayName}},
                  @{Name="Direction";Expression={$_.Direction}},
                  @{Name="Action";Expression={$_.Action}},
                  @{Name="Enabled";Expression={$_.Enabled}} |
    Format-Table -AutoSize

Write-Host "`n============================================" -ForegroundColor Cyan
Write-Host "✅ FIREWALL CONFIGURATION COMPLETED!" -ForegroundColor Green
Write-Host "============================================`n" -ForegroundColor Cyan

Write-Host "IMPORTANT NOTES:" -ForegroundColor Yellow
Write-Host "1. Port 5622 is OPEN to the Internet for client connections" -ForegroundColor White
if ($AdminIP -eq "0.0.0.0/0") {
    Write-Host "2. ⚠ RDP is OPEN to ALL IPs - SECURITY RISK!" -ForegroundColor Red
    Write-Host "   Run script again with -AdminIP parameter:" -ForegroundColor Red
    Write-Host "   .\Setup-Firewall.ps1 -AdminIP `"YOUR_IP_HERE`"" -ForegroundColor Cyan
} else {
    Write-Host "2. ✓ RDP is restricted to: $AdminIP" -ForegroundColor Green
}
Write-Host "3. All other ports are restricted to LocalSubnet only" -ForegroundColor White
Write-Host "4. Verify firewall rules with: Get-NetFirewallRule | Where DisplayName -like `"GameServer*`"" -ForegroundColor White
Write-Host "5. Test client connection: Test-NetConnection -ComputerName YOUR_VPS_IP -Port 5622`n" -ForegroundColor White
