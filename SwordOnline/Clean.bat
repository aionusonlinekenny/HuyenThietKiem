@echo off
echo ----------------------------------------------------
echo Press any key to delete all files with extensions:
echo  *.obj, *.pdb
echo Visual C++/.Net junk files in C:\ThienDieuOnline
echo ----------------------------------------------------
pause

cd /d "D:\HuyenThietKiem\SwordOnline"
del /F /Q /S *.obj
echo Cleanup complete.
pause
