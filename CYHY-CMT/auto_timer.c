/*
AVIS:
    artifact: auto_timer.c
    role: autoscan-timer-engine
    family: cyhy-cmt
    version: 1.0
*/

#include <windows.h>
#include "auto_timer.h"
#include "cyhy.h"
#include "resource.h"
#include "template_loader.h"
#include "auto_timer.h"
#include "template_loader.h"
#include "avis-log.h"

extern HWND hEdit;


#define AUTO_TIMER_ID  9001
#define AUTO_INTERVAL  800   // ms


static BOOL IsAvis(const char *t)
{
    if (!t) return FALSE;
    for (int i = 0; t[i] && i < 300; i++)
        if (t[i]=='A' && t[i+1]=='V' && t[i+2]=='I' && t[i+3]=='S' && t[i+4]==':')
            return TRUE;
    return FALSE;
}

void AUTO_Start(HWND hWnd)
{
    SetTimer(hWnd, AUTO_TIMER_ID, AUTO_INTERVAL, NULL);
    AVIS_Log("AUTO timer started");
}

void AUTO_Stop(HWND hWnd)
{
    KillTimer(hWnd, AUTO_TIMER_ID);
    AVIS_Log("AUTO timer stopped");
}

void AUTO_OnTimer(HWND hWnd)
{
    // 1. Attempt to load the terminal artifact
    if (!MERCG_LoadAVIS("terminal.avs"))
    {
        // Silent return if no file to avoid flooding terminal
        return; 
    }

    // 2. Validate the buffer status
    if (gAvisLoaded)
    {
        AVIS_INFO("AUTO", "AVIS detected in terminal.avs - Heartbeat active.");
        
        // Note: Logic in cyhy.c handles the buffer consumption
        // We just log the successful pulse here.
        AVIS_MSG("AUTO", "Pulse complete: Workspace buffer synchronized.");
    }
}
