@echo off
REM ============================================================
REM GAME SERVER STOP SCRIPT
REM Huyen Thiet Kiem - Windows Server
REM ============================================================

echo.
echo ============================================
echo   STOPPING GAME SERVER COMPONENTS
echo ============================================
echo.

REM Kiểm tra quyền Administrator
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [ERROR] This script requires Administrator privileges!
    echo Please right-click and select "Run as Administrator"
    pause
    exit /b 1
)

echo [1/5] Stopping Game Servers (GS1-4)...
taskkill /IM GS1.exe /F >nul 2>&1
if %errorLevel% equ 0 echo       [OK] GS1.exe stopped
taskkill /IM GS2.exe /F >nul 2>&1
if %errorLevel% equ 0 echo       [OK] GS2.exe stopped
taskkill /IM GS3.exe /F >nul 2>&1
if %errorLevel% equ 0 echo       [OK] GS3.exe stopped
taskkill /IM GS4.exe /F >nul 2>&1
if %errorLevel% equ 0 echo       [OK] GS4.exe stopped
timeout /t 2 /nobreak >nul

echo.
echo [2/5] Stopping Bishop.exe (Gateway)...
taskkill /IM Bishop.exe /F >nul 2>&1
if %errorLevel% equ 0 (
    echo       [OK] Bishop.exe stopped
) else (
    echo       [INFO] Bishop.exe was not running
)
timeout /t 2 /nobreak >nul

echo.
echo [3/5] Stopping Sword3PaySys.exe (Account)...
taskkill /IM Sword3PaySys.exe /F >nul 2>&1
if %errorLevel% equ 0 (
    echo       [OK] Sword3PaySys.exe stopped
) else (
    echo       [INFO] Sword3PaySys.exe was not running
)
timeout /t 2 /nobreak >nul

echo.
echo [4/5] Stopping Goddess.exe (Role Server)...
taskkill /IM Goddess.exe /F >nul 2>&1
if %errorLevel% equ 0 (
    echo       [OK] Goddess.exe stopped
) else (
    echo       [INFO] Goddess.exe was not running
)
timeout /t 2 /nobreak >nul

echo.
echo [5/5] Verifying all processes stopped...
tasklist | findstr /i "Bishop Goddess Sword3PaySys GS1 GS2 GS3 GS4" >nul
if %errorLevel% equ 0 (
    echo       [WARNING] Some processes are still running:
    tasklist | findstr /i "Bishop Goddess Sword3PaySys GS1 GS2 GS3 GS4"
) else (
    echo       [OK] All game server processes stopped
)

echo.
echo ============================================
echo   GAME SERVER STOPPED
echo ============================================
echo.
echo NOTE: Database service (MySQL/MSSQL) was NOT stopped
echo       Stop manually if needed: net stop MySQL57
echo.
echo Press any key to close this window...
pause >nul
