/*
AVIS:
    artifact: avis-log.c
    role: log-core-terminal-engine
    family: cyhy-cmt
    version: 2.0 (Terminal-Integrated)
*/

#include "avis-log.h"
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "common.h"

static char gAvisLog[65536]; // Expanded for terminal history
static size_t gAvisLogPos = 0;
static HWND gHwndTerminal = NULL;

/* ---------------------------------------------------------
   Set current active terminal window (Main or MERC-G)
--------------------------------------------------------- */
void AVIS_SetTerminalWindow(HWND hWnd) {
    gHwndTerminal = hWnd;
}

/* ---------------------------------------------------------
   Core Leveled Logger: [TIME] [LVL] [MOD] Message
--------------------------------------------------------- */
void AVIS_LogWrite(AVIS_LEVEL level, const char* module, const char* fmt, ...) {
    char message[1024];
    char entry[2048];
    char timestamp[16];
    
    // 1. Generate Timestamp
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf(timestamp, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);

    // 2. Format Body
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(message, sizeof(message), fmt, ap);
    va_end(ap);

    // 3. Construct Terminal Entry
    const char* lvlStr;
    switch(level) {
        case LOG_ERROR: lvlStr = "FAIL"; break;
        case LOG_WARN:  lvlStr = "WARN"; break;
        case LOG_AVIS:  lvlStr = "AVIS"; break;
        default:        lvlStr = "INFO"; break;
    }

    int len = snprintf(entry, sizeof(entry), "[%s] [%s] [%s] %s\r\n", 
                       timestamp, lvlStr, module, message);

    // 4. Thread-Safe Buffer Write (Circular Logic)
    if (gAvisLogPos + len >= sizeof(gAvisLog)) {
        gAvisLogPos = 0; 
        memset(gAvisLog, 0, sizeof(gAvisLog));
    }
    memcpy(&gAvisLog[gAvisLogPos], entry, len);
    gAvisLogPos += len;

    // 5. LIVE TERMINAL PUSH
    if (gHwndTerminal && IsWindow(gHwndTerminal)) {
        // Append text to the end of the Edit control
        int txtLen = GetWindowTextLength(gHwndTerminal);
        SendMessage(gHwndTerminal, EM_SETSEL, (WPARAM)txtLen, (LPARAM)txtLen);
        SendMessage(gHwndTerminal, EM_REPLACESEL, 0, (LPARAM)entry);
        // Auto-scroll to bottom
        SendMessage(gHwndTerminal, WM_VSCROLL, SB_BOTTOM, 0);
    }

    // Always mirror to Debugger for MSYS64 console monitoring
    OutputDebugStringA(entry);
}

/* ---------------------------------------------------------
   Legacy Wrappers
--------------------------------------------------------- */
void AVIS_Log(const char *msg) {
    AVIS_LogWrite(LOG_INFO, "GEN", "%s", msg);
}

void AVIS_LogFmt(const char *fmt, ...) {
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    AVIS_Log(buf);
}

/* ---------------------------------------------------------
   Commit terminal buffer to disk
--------------------------------------------------------- */
void AVIS_LogSave(const char *path) {
    HANDLE hFile = CreateFileA(path, GENERIC_WRITE, 0, NULL, 
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD written = 0;
        WriteFile(hFile, gAvisLog, (DWORD)strlen(gAvisLog), &written, NULL);
        CloseHandle(hFile);
    }
}
