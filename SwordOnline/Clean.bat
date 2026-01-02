@echo off
echo ----------------------------------------------------
echo Press any key to delete all files with extensions:
echo   *.obj, *.pch, *.idb
echo Visual C++/.Net junk files in C:\ThienDieuOnline
echo ----------------------------------------------------
pause

cd /d "E:\HuyenThietKiem\SwordOnline"
del /F /Q /S  *.obj, *.pch, *.idb
echo Cleanup complete.
pause
