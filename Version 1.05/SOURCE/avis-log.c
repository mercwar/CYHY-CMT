/*
AVIS:
    artifact: avis-log.c
    role: log-core-terminal-engine
    family: cyhy-cmt
    version: 2.1 (Headers Restored)
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>   // For HWND, SYSTEMTIME, SendMessage
#include <stdio.h>     // For sprintf, vsnprintf
#include <stdarg.h>    // For va_list, va_start
#include <string.h>    // For strlen, memcpy
#include <time.h>      // For time

#include "avis-log.h"  // For AVIS_LEVEL, LOG_ERROR, etc.

/* --- GLOBALS (Internal to Logger) --- */
static char gAvisLog[65536]; 
static size_t gAvisLogPos = 0;
static HWND gHwndTerminal = NULL;

/* ---------------------------------------------------------
   Set current active terminal window
--------------------------------------------------------- */
void AVIS_SetTerminalWindow(HWND hWnd) {
    gHwndTerminal = hWnd;
    if (IsWindow(gHwndTerminal)) {
        SendMessage(gHwndTerminal, EM_LIMITTEXT, 0, 0); // No 64KB limit
    }
}

/* ---------------------------------------------------------
   Core Leveled Logger: [TIME] [LVL] [MOD] Message
--------------------------------------------------------- */
void AVIS_LogWrite(AVIS_LEVEL level, const char* module, const char* fmt, ...) {
    char message[1024], entry[2048], timestamp[16];
    SYSTEMTIME st;
    GetLocalTime(&st);
    
    sprintf(timestamp, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(message, sizeof(message), fmt, ap);
    va_end(ap);

    const char* lvl = (level == LOG_ERROR) ? "FAIL" : (level == LOG_AVIS ? "AVIS" : "INFO");
    int len = sprintf(entry, "[%s] [%s] [%s] %s\r\n", timestamp, lvl, module, message);

    // 1. Sync to Internal Buffer
    if (gAvisLogPos + len < sizeof(gAvisLog) - 1) {
        memcpy(gAvisLog + gAvisLogPos, entry, len);
        gAvisLogPos += len;
        gAvisLog[gAvisLogPos] = '\0';
    }

    // 2. Robot UI Output
    if (gHwndTerminal && IsWindow(gHwndTerminal)) {
        int txtLen = GetWindowTextLengthA(gHwndTerminal);
        
        // Auto-Truncate if buffer gets too large for the Edit Control
        if (txtLen > 30000) {
            SendMessage(gHwndTerminal, EM_SETSEL, 0, 15000);
            SendMessage(gHwndTerminal, EM_REPLACESEL, 0, (LPARAM)"... [LOG TRUNCATED] ...\r\n");
            txtLen = GetWindowTextLengthA(gHwndTerminal);
        }

        SendMessage(gHwndTerminal, EM_SETSEL, (WPARAM)txtLen, (LPARAM)txtLen);
        SendMessage(gHwndTerminal, EM_REPLACESEL, 0, (LPARAM)entry);
        SendMessage(gHwndTerminal, WM_VSCROLL, SB_BOTTOM, 0);
    }
    
    OutputDebugStringA(entry);
}

/* ---------------------------------------------------------
   Commit terminal buffer to disk
--------------------------------------------------------- */
void AVIS_LogSave(const char *path) {
    FILE* f = fopen(path, "w");
    if (f) {
        fputs(gAvisLog, f);
        fclose(f);
    }
}
