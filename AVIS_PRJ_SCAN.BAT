@echo off
setlocal EnableExtensions EnableDelayedExpansion

:: ============================================================
::  FIXED + STABLE ESCAPE SEQUENCE INITIALIZATION
:: ============================================================
for /f %%A in ('echo prompt $E^| cmd') do set "e=%%A"

chcp 65001 >nul
rem ----------------------------------------------------------------------------
rem STEP 1: HARDWARE DETECTION
rem ----------------------------------------------------------------------------
echo [AVIS] INITIALIZING HARDWARE DETECTION...
call "%~dp0AVIS_PRJ_SCAN\AVIS_REM_DRV.BAT"

if errorlevel 1 (
    echo [AVIS] CRITICAL ERROR: Removable drive detection failed.
    PAUSE
    exit /b 1
)
cls
:: ============================================================
::  AVIS AUDIO ENGINE (Embedded)
::  Supports: MIDI (.mid/.midi) and WAV (.wav)
::  Silent playback using PowerShell COM objects
:: ============================================================

reg add "HKCU\Console" /v FaceName /t REG_SZ /d "" /f >nul 2>&1
reg add "HKCU\Console" /v FontFamily /t REG_DWORD /d 0x00000000 /f >nul 2>&1
reg add "HKCU\Console" /v FontSize /t REG_DWORD /d 0x00100000 /f >nul 2>&1


rem start /min "" "%~dp0AVIS_PRJ_SCAN\midi\JP-YGATC.mid" -a ds -m ds -i -s -g 1.0

echo.
echo   %e%[38;5;244m╔══════════════════════════════════════════════════════════════╗
echo   %e%[38;5;244m║                                                              ║
echo   %e%[38;5;244m║    %e%[38;5;255mPROJ: AVIS PROJECT SCAN                                   %e%[38;5;244m║
echo   %e%[38;5;244m║    %e%[38;5;255mARCH: SHINE ARCHITECTURE                                  %e%[38;5;244m║
echo   %e%[38;5;244m║    %e%[38;5;255mAUTH: CVBGOD                                              %e%[38;5;244m║
echo   %e%[38;5;244m║    %e%[38;5;255mCORE: VER 1.07                                            %e%[38;5;244m║
echo   %e%[38;5;244m║    %e%[38;5;255mDATE: 01/27/2026                                          %e%[38;5;244m║
echo   %e%[38;5;244m║                                                              ║
echo   %e%[38;5;244m║    %e%[38;5;240mCOPY: © MERCWAR FFF * ALL RIGHTS RESERVED *               %e%[38;5;244m║
echo   %e%[38;5;244m║                                                              ║
echo   %e%[38;5;244m║    %e%[38;5;239m   [::::::::::::::::::::::::::::::::::::::]               %e%[38;5;244m║
echo   %e%[38;5;244m║                                                              ║
echo   %e%[38;5;244m║            %e%[38;5;255mINITIALIZING AI SCAN SEQUENCE...                  %e%[38;5;244m║
echo   %e%[38;5;244m╚══════════════════════════════════════════════════════════════╝%e%[0m
echo.
echo   [  Welcome AVIS Project Scan DEV, in CVBGOD's World  ]
echo.
echo     :: {CHECK YOUR SPELLING IN YOUR INI FOR THROW OR HIDE} ::
echo        :: {APPLY YOUR CHANGES TO 'USER MODE SELECTION KEYS'}::
echo.
echo                                         [ Revised by MS Copilot! ]
echo.
timeout /t 1 >nul
cls
echo.
echo   %e%[38;5;239m┌────────────────────────────────────────────────────────────┐
echo   │                                                            │
echo   │           %e%[38;5;255mS H I N E   A R C H I T E C T U R E%e%[38;5;239m              │
echo   │                                                            │
echo   └────────────────────────────────────────────────────────────┘%e%[0m
echo.
echo       %e%[38;5;244m ██████╗ ██╗   ██╗██████╗  ██████╗  ██████╗ ██████╗
echo        ██╔════╝ ██║   ██║██╔══██╗██╔════╝ ██╔═══██╗██╔══██╗
echo        ██║      ██║   ██║██████╔╝██║  ███╗██║   ██║██║  ██║
echo        ██║      ╚██╗ ██╔╝██╔══██╗██║   ██║██║   ██║██║  ██║
echo        ╚██████╗  ╚████╔╝ ██████╔╝╚██████╔╝╚██████╔╝██████╔╝
echo         ╚═════╝   ╚═══╝  ╚═════╝  ╚═════╝  ╚═════╝ ╚═════╝ %e%[0m
echo.
echo   %e%[38;5;239m┌────────────────────────────────────────────────────────────┐
echo   │                     %e%[38;5;255mDESIGNED BY CVBGOD%e%[38;5;239m                     │
echo   └────────────────────────────────────────────────────────────┘%e%[0m
echo  © 2026 CVBGOD
echo.
echo   [ THIS VERSION HAS NO INI SUPPORT FOR 'UMSK * INI' ]
echo.
echo    :: {APPLY YOUR CHANGES TO 'USER MODE SELECTION KEYS'} ::
echo          :: {OPEN AVIS_PRJ_SCAN.BAT SCROLL DOWN TO UMSK} ::
echo            :: {REMOVABLE DRIVES HANG AT TIMES, IT ISN'T DOS} ::
echo.          
echo.
echo                   [ WaReZ bY m3rCw4r FFF D3M0N1z3r TH3 CV8G0D ]
timeout /t 1 >nul
cls

rem ============================================================================
rem AVIS PROJECT SCAN - UMSK — USER MODE SELECTION KEYS
rem ============================================================================
REM SET "REMDRV=G:"
set "PRJ_DRV_KEY=Apache24"
set "PRJ_DIR_KEY=htdocs"
set "PRJ_ROOT_KEY=\PHP\COPILOT_CMS\CYHY"
set "PRJ_APP_KEY=AVIS_PRJ_SCAN"
rem ============================================================================
rem AVIS PROJECT SCAN - MASTER CONTROLLER (DEMO VERSION)
rem ============================================================================
rem ROLE: Main Entry Point. Detects hardware and anchors the AI project path.
rem FILE: AVIS_PRJ_SCAN.BAT
rem VERSION: 1.07 - SHINE ARCHITECTURE
rem ============================================================================


rem ----------------------------------------------------------------------------
rem STEP 2: PROJECT ANCHORING
rem ----------------------------------------------------------------------------
echo [AVIS] ANCHORING PROJECT TO DETECTED DRIVE...



rem Establishing the root directory on the detected drive.
set "PROJECT_ROOT=%REMDRV%\%PRJ_DRV_KEY%\%PRJ_DIR_KEY%%PRJ_ROOT_KEY%"
cd /d "%PROJECT_ROOT%"

set "PROJECT_APPROOT=%PRJ_APP_KEY%\"
set "PROJECT_PATH=%PROJECT_ROOT%\%PROJECT_APPROOT%"

echo [AVIS] PROJECT ROOT : %PROJECT_ROOT%
echo [AVIS] APP PATH     : %PROJECT_PATH%
echo.

rem ----------------------------------------------------------------------------
rem STEP 3: SYSTEM INI
rem ----------------------------------------------------------------------------

echo.
echo [AVIS] SELECT MODE:
echo        1. CONTINUE WITH SCAN
echo        2. OPEN CONFIGURATION FILE
echo.

set /p AVIS_MODE=[AVIS] ENTER CHOICE (1/2): 

if "%AVIS_MODE%"=="2" (
    echo [AVIS] OPENING CONFIGURATION IN NOTEPAD...
    start notepad "%PROJECT_PATH%AVIS_PRJ_SCAN.INI"
    echo [AVIS] CONFIG OPENED. CLOSE NOTEPAD AND RE-RUN THE SCAN.
    pause
    exit /b 0
)

if not "%AVIS_MODE%"=="1" (
    echo [AVIS] INVALID SELECTION.
    pause
    exit /b 1
)

echo [AVIS] CONTINUING WITH SCAN...
echo.

rem ----------------------------------------------------------------------------
rem STEP 4: SYSTEM EXECUTION
rem ----------------------------------------------------------------------------
echo [AVIS] VERIFYING EXECUTION CORE...

if exist "%PROJECT_PATH%AVIS_PRJ_SCAN_EXEC.BAT" (
    echo [AVIS] EXECUTING PROJECT SCAN SEQUENCE...
    call "%PROJECT_PATH%AVIS_PRJ_SCAN_EXEC.BAT"
) else (
    echo [AVIS] SHINE_ERROR: Executor core missing at %PROJECT_PATH%
    pause
)


rem ----------------------------------------------------------------------------
rem STEP 5: EXIT & CLEANUP
rem ----------------------------------------------------------------------------
echo.
echo [AVIS] DEV SCAN COMPLETE !
echo [AVIS] I AM CVBGOD, AND I HAVE GIVEN IT TO YOU HEHEMM, ... LITTLE BOTS :)
rem timeout /t 3 >nul
PAUSE
endlocal
exit /b 0
