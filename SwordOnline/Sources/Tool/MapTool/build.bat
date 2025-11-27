@echo off
echo ========================================
echo Sword Online Map Tool - Build Script
echo ========================================
echo.

REM Check if dotnet is available
where dotnet >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: .NET SDK not found!
    echo Please install .NET SDK from https://dotnet.microsoft.com/download
    pause
    exit /b 1
)

echo Building MapTool...
echo.

dotnet build MapTool.csproj -c Release

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Build succeeded!
    echo ========================================
    echo.
    echo Executable location:
    echo bin\Release\net48\MapTool.exe
    echo.
    echo To run the tool:
    echo   cd bin\Release\net48
    echo   MapTool.exe
    echo.
) else (
    echo.
    echo ========================================
    echo Build FAILED!
    echo ========================================
    echo.
    echo Please check the errors above.
    echo.
)

pause
