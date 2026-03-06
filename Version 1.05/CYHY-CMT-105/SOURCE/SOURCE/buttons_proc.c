/*
AVIS:
    artifact: buttons_proc.c
    role: ui-button-proc-block
    family: cyhy-cmt
    version: 1.1 (Terminal-Logged)
*/

#include <windows.h>
#include "resource.h"
#include "cyhy.h"
#include "auto_timer.h"
#include "buttons_proc.h"
#include "avis-log.h" // Integrated for terminal display

extern BOOL gAutoSave;
extern HWND hAutoSaveChk;
HWND hBtnAuto = NULL;
BOOL autoRunning = FALSE;

void BTN_ProcAuto(HWND hWnd)
{
    autoRunning = !autoRunning;

    if (autoRunning)
    {
        // 500ms Scan Pulse
        SetTimer(hWnd, AUTO_TIMER_ID, 500, NULL);
        SetWindowTextA(hBtnAuto, "AUTO-OFF");
        
        AVIS_INFO("AUTO", "Background Scanner ENABLED. Pulse rate: 500ms.");
    }
    else
    {
        KillTimer(hWnd, AUTO_TIMER_ID);
        SetWindowTextA(hBtnAuto, "AUTO-ON");
        
        AVIS_INFO("AUTO", "Background Scanner DISABLED.");
    }
}
