#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "avis-log.h"

char gAvisBuffer[4096];
BOOL gAvisLoaded = FALSE;

BOOL MERCG_LoadAVIS(const char *path) {
    AVIS_LogFmt("Loading AVIS file: %s", path);

    HANDLE hFile = CreateFileA(
        path,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        AVIS_Log("ERROR: Failed to open AVIS file.");
        gAvisLoaded = FALSE;
        return FALSE;
    }

    DWORD bytesRead = 0;
    BOOL ok = ReadFile(hFile, gAvisBuffer, sizeof(gAvisBuffer), &bytesRead, NULL);
    CloseHandle(hFile);

    if (!ok || bytesRead == 0) {
        AVIS_Log("ERROR: Failed to read AVIS file.");
        gAvisLoaded = FALSE;
        return FALSE;
    }

    gAvisLoaded = TRUE;
    AVIS_LogFmt("AVIS loaded (%lu bytes).", bytesRead);
    return TRUE;
}
