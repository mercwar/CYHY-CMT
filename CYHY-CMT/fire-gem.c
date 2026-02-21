/* 
AVIS:
    artifact: fire-gem.c
    role: kernel-seed
    family: fire-gem
    version: 1.0
    description: Fire‑Gem kernel entry for cyhy-cmt
*/

#include "fire-gem.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <windows.h>

// --------------------------------------
// AVIS LOG BUFFER
// --------------------------------------
static char gAvisLog[16384];
static size_t gAvisLogPos = 0;

// --------------------------------------
// Logging Implementation
// --------------------------------------
void AVIS_Log(const char *msg) {
    size_t len = strlen(msg);
    if (gAvisLogPos + len + 2 >= sizeof(gAvisLog))
        return;

    memcpy(&gAvisLog[gAvisLogPos], msg, len);
    gAvisLogPos += len;
    gAvisLog[gAvisLogPos++] = '\n';
    gAvisLog[gAvisLogPos] = 0;
}

void AVIS_LogFmt(const char *fmt, ...) {
    char buf[512];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    AVIS_Log(buf);
}

void AVIS_LogSave(const char *path) {
    HANDLE hFile = CreateFileA(
        path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE)
        return;

    DWORD written = 0;
    WriteFile(hFile, gAvisLog, (DWORD)strlen(gAvisLog), &written, NULL);
    CloseHandle(hFile);
}

// --------------------------------------
// Fire‑Gem Kernel
// --------------------------------------
static FG_ModuleEntry gModule = 0;

void FireGem_Register(FG_ModuleEntry entry) {
    gModule = entry;
}

void FireGem_Start(void) {
    AVIS_Log("=== FIRE‑GEM KERNEL START ===");
    AVIS_Log("artifact: fire-gem.c");
    AVIS_Log("role: kernel-seed");
    AVIS_Log("family: fire-gem");
    AVIS_Log("version: 1.0");

    if (gModule) {
        AVIS_Log("Running drop‑in module...");
        gModule();
    } else {
        AVIS_Log("No drop‑in module registered.");
    }

    AVIS_Log("=== FIRE‑GEM KERNEL SHUTDOWN ===");
    AVIS_LogSave("fire-gem.log");
}
