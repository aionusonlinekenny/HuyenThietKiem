@echo off
REM Build script for MapCoord.exe
REM Requires Visual C++ 6.0 or later

echo ========================================
echo Building MapCoord.exe...
echo ========================================
echo.

REM Compile the program
cl.exe /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" MapCoord.cpp /link kernel32.lib user32.lib /out:MapCoord.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Build successful!
    echo ========================================
    echo MapCoord.exe created
    echo.
    echo Testing the tool:
    echo.
    MapCoord.exe
) else (
    echo.
    echo ========================================
    echo Build failed!
    echo ========================================
    echo Please check that cl.exe is in your PATH
    echo You may need to run vcvars32.bat first
)

echo.
pause
