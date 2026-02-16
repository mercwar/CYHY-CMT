@echo off
setlocal enabledelayedexpansion

echo ============================================
echo   SCANNING REMOVABLE DRIVE: G:\PROGRAMS
echo ============================================

if not exist "G:\PROGRAMS" (
    echo ERROR: G:\PROGRAMS not found.
    echo Insert the removable drive and try again.
    pause
    exit /b
)

echo Creating file list...
dir "G:\PROGRAMS" /s /b /a:-d > filelist_G_PROGRAMS.txt

echo Done.
echo Output saved to: filelist_G_PROGRAMS.txt
echo ============================================
pause
