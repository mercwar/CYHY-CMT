@echo off
setlocal enabledelayedexpansion

:: FORCE ANCHOR APP ROOT (WORKS EVEN ON REMOVABLE DRIVES)
for %%A in ("%~f0") do set "APP_ROOT=%%~dpA"
set "APP_ROOT=%APP_ROOT:~0,-1%"

echo App Root: %APP_ROOT%
echo.

:: CREATE OUTPUT FOLDER
set "OUT=%APP_ROOT%\TEXT"
if exist "%OUT%" rd /s /q "%OUT%"
mkdir "%OUT%" >nul 2>&1

echo Copying and converting extensions to .text...
echo.

:: COPY AND RENAME — NO LOOPS, NO RECURSION, NO CRASHES
for %%X in ("*.vbp" "*.frm" "*.bas" "*.cls" "*.ctl" "*.pag" "*.dsr" "*.mak" "*.txt" "*.ini" "*.rc" "*.c" "*.h" "*.ppj") do (
    for %%F in (%APP_ROOT%\%%~X) do (
        if exist "%%~F" (
            copy /Y "%%~F" "%OUT%\%%~nF.text" >nul
            echo COPIED: %%~nxF  ^>  %%~nF.text
        )
    )
)

echo.
echo Done.
echo Files copied and converted to: %OUT%
echo.

pause
endlocal
exit /b 0
