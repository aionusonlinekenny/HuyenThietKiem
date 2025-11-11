# ============================================================
# GAME SERVER STATUS CHECK SCRIPT
# Windows Server 2012 R2+
# ============================================================

Write-Host "`n============================================" -ForegroundColor Cyan
Write-Host "  GAME SERVER STATUS CHECK" -ForegroundColor Cyan
Write-Host "============================================`n" -ForegroundColor Cyan

# 1. CHECK PROCESSES
Write-Host "[1/5] Checking game server processes..." -ForegroundColor Yellow
$processes = @("Bishop", "Goddess", "Sword3PaySys", "GS1", "GS2", "GS3", "GS4")
$runningCount = 0

foreach ($proc in $processes) {
    $running = Get-Process -Name $proc -ErrorAction SilentlyContinue
    if ($running) {
        Write-Host "      ✓ $proc.exe is running (PID: $($running.Id))" -ForegroundColor Green
        $runningCount++
    } else {
        Write-Host "      ✗ $proc.exe is NOT running" -ForegroundColor Red
    }
}

Write-Host "`n      Total: $runningCount / $($processes.Count) processes running`n" -ForegroundColor White

# 2. CHECK LISTENING PORTS
Write-Host "[2/5] Checking listening ports..." -ForegroundColor Yellow
$ports = @(
    @{Port=5622; Name="Client Connection (Bishop)"; Critical=$true},
    @{Port=5632; Name="Bishop-GameServer"},
    @{Port=5001; Name="Goddess Role Server"},
    @{Port=5002; Name="Account Server"},
    @{Port=3306; Name="MySQL Database"},
    @{Port=1433; Name="MSSQL Database"}
)

foreach ($portInfo in $ports) {
    $listening = Get-NetTCPConnection -LocalPort $portInfo.Port -State Listen -ErrorAction SilentlyContinue
    if ($listening) {
        $process = Get-Process -Id $listening.OwningProcess -ErrorAction SilentlyContinue
        $processName = if ($process) { $process.Name } else { "Unknown" }
        Write-Host "      ✓ Port $($portInfo.Port) - $($portInfo.Name) - Process: $processName" -ForegroundColor Green
    } else {
        if ($portInfo.Critical) {
            Write-Host "      ✗ Port $($portInfo.Port) - $($portInfo.Name) - NOT LISTENING [CRITICAL]" -ForegroundColor Red
        } else {
            Write-Host "      ⚠ Port $($portInfo.Port) - $($portInfo.Name) - NOT LISTENING" -ForegroundColor DarkYellow
        }
    }
}

# 3. CHECK FIREWALL RULES
Write-Host "`n[3/5] Checking firewall rules..." -ForegroundColor Yellow
$fwRules = Get-NetFirewallRule | Where-Object {$_.DisplayName -like "GameServer*"}
if ($fwRules.Count -gt 0) {
    Write-Host "      ✓ Found $($fwRules.Count) firewall rules" -ForegroundColor Green
    foreach ($rule in $fwRules) {
        $status = if ($rule.Enabled -eq "True") { "✓" } else { "✗" }
        $color = if ($rule.Enabled -eq "True") { "Green" } else { "Red" }
        Write-Host "      $status $($rule.DisplayName) - $($rule.Action)" -ForegroundColor $color
    }
} else {
    Write-Host "      ✗ No GameServer firewall rules found!" -ForegroundColor Red
    Write-Host "        Run Setup-Firewall.ps1 to create rules" -ForegroundColor Yellow
}

# 4. CHECK DATABASE SERVICE
Write-Host "`n[4/5] Checking database services..." -ForegroundColor Yellow
$mysqlService = Get-Service -Name "MySQL*" -ErrorAction SilentlyContinue | Where-Object {$_.Status -eq "Running"}
$mssqlService = Get-Service -Name "MSSQL*" -ErrorAction SilentlyContinue | Where-Object {$_.Status -eq "Running"}

if ($mysqlService) {
    Write-Host "      ✓ MySQL is running ($($mysqlService.Name))" -ForegroundColor Green
} elseif ($mssqlService) {
    Write-Host "      ✓ MSSQL is running ($($mssqlService.Name))" -ForegroundColor Green
} else {
    Write-Host "      ✗ No database service detected" -ForegroundColor Red
    Write-Host "        Please start MySQL or MSSQL Server" -ForegroundColor Yellow
}

# 5. TEST PORT 5622 FROM LOCALHOST
Write-Host "`n[5/5] Testing client connection port (5622)..." -ForegroundColor Yellow
$testResult = Test-NetConnection -ComputerName localhost -Port 5622 -WarningAction SilentlyContinue
if ($testResult.TcpTestSucceeded) {
    Write-Host "      ✓ Port 5622 is accessible from localhost" -ForegroundColor Green
} else {
    Write-Host "      ✗ Port 5622 is NOT accessible!" -ForegroundColor Red
    Write-Host "        Bishop server may not be running" -ForegroundColor Yellow
}

# SUMMARY
Write-Host "`n============================================" -ForegroundColor Cyan
Write-Host "  SUMMARY" -ForegroundColor Cyan
Write-Host "============================================`n" -ForegroundColor Cyan

if ($runningCount -eq $processes.Count -and $testResult.TcpTestSucceeded) {
    Write-Host "✅ ALL SYSTEMS OPERATIONAL" -ForegroundColor Green
    Write-Host "   Game server is ready for client connections`n" -ForegroundColor Green
} elseif ($runningCount -gt 0) {
    Write-Host "⚠ PARTIAL OPERATION" -ForegroundColor Yellow
    Write-Host "   Some components are not running`n" -ForegroundColor Yellow
} else {
    Write-Host "✗ GAME SERVER NOT RUNNING" -ForegroundColor Red
    Write-Host "   Use Start-GameServer.bat to start all components`n" -ForegroundColor Yellow
}

# NETWORK INFO
$publicIP = (Invoke-WebRequest -Uri "http://ifconfig.me/ip" -UseBasicParsing -TimeoutSec 5 -ErrorAction SilentlyContinue).Content.Trim()
if ($publicIP) {
    Write-Host "📡 Public IP: $publicIP" -ForegroundColor White
    Write-Host "   Clients should connect to: $publicIP`:5622`n" -ForegroundColor White
}
