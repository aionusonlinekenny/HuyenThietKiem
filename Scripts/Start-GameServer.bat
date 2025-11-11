@echo off
REM ============================================================
REM GAME SERVER START SCRIPT
REM Huyen Thiet Kiem - Windows Server
REM ============================================================

echo.
echo ============================================
echo   STARTING GAME SERVER COMPONENTS
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

REM Set thư mục game server (thay đổi nếu cần)
set GAME_DIR=C:\GameServer
cd /d %GAME_DIR%

if not exist "%GAME_DIR%" (
    echo [ERROR] Game server directory not found: %GAME_DIR%
    echo Please edit this script and set correct GAME_DIR path
    pause
    exit /b 1
)

echo [1/5] Checking database service...
sc query MySQL57 | find "RUNNING" >nul
if %errorLevel% equ 0 (
    echo       [OK] MySQL is running
) else (
    sc query MSSQLSERVER | find "RUNNING" >nul
    if %errorLevel% equ 0 (
        echo       [OK] MSSQL Server is running
    ) else (
        echo       [WARNING] No database service detected
        echo       Please start MySQL or MSSQL Server manually
    )
)

echo.
echo [2/5] Starting Goddess.exe (Role Server)...
if exist "%GAME_DIR%\Goddess.exe" (
    start "Goddess Server" /D "%GAME_DIR%" Goddess.exe
    echo       [OK] Goddess.exe started
    timeout /t 5 /nobreak >nul
) else (
    echo       [ERROR] Goddess.exe not found
    pause
    exit /b 1
)

echo.
echo [3/5] Starting Sword3PaySys.exe (Account Server)...
if exist "%GAME_DIR%\Sword3PaySys.exe" (
    start "Account Server" /D "%GAME_DIR%" Sword3PaySys.exe
    echo       [OK] Sword3PaySys.exe started
    timeout /t 5 /nobreak >nul
) else (
    echo       [ERROR] Sword3PaySys.exe not found
    pause
    exit /b 1
)

echo.
echo [4/5] Starting Bishop.exe (Gateway/Login Server)...
if exist "%GAME_DIR%\Bishop.exe" (
    start "Bishop Gateway" /D "%GAME_DIR%" Bishop.exe
    echo       [OK] Bishop.exe started
    timeout /t 5 /nobreak >nul
) else (
    echo       [ERROR] Bishop.exe not found
    pause
    exit /b 1
)

echo.
echo [5/5] Starting Game Servers...
if exist "%GAME_DIR%\GS1.exe" (
    start "Game Server 1" /D "%GAME_DIR%" GS1.exe
    echo       [OK] GS1.exe started
    timeout /t 2 /nobreak >nul
)
if exist "%GAME_DIR%\GS2.exe" (
    start "Game Server 2" /D "%GAME_DIR%" GS2.exe
    echo       [OK] GS2.exe started
    timeout /t 2 /nobreak >nul
)
if exist "%GAME_DIR%\GS3.exe" (
    start "Game Server 3" /D "%GAME_DIR%" GS3.exe
    echo       [OK] GS3.exe started
    timeout /t 2 /nobreak >nul
)
if exist "%GAME_DIR%\GS4.exe" (
    start "Game Server 4" /D "%GAME_DIR%" GS4.exe
    echo       [OK] GS4.exe started
)

echo.
echo ============================================
echo   ALL GAME SERVER COMPONENTS STARTED
echo ============================================
echo.
echo Running processes:
tasklist | findstr /i "Bishop Goddess Sword3PaySys GS1 GS2 GS3 GS4"
echo.
echo Press any key to close this window...
pause >nul
