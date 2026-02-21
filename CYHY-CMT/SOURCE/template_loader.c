/*
AVIS:
    artifact: template_loader.c
    role: avis-file-io-sync
    family: cyhy-cmt
    version: 1.1 (Full-Logged)
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "template_loader.h"
#include "common.h"
#include "avis-log.h"

char gAvisBuffer[4096];
BOOL gAvisLoaded = FALSE;

/* ---------------------------------------------------------
   Load AVIS into global buffer
--------------------------------------------------------- */
BOOL MERCG_LoadAVIS(const char *path)
{
    AVIS_INFO("FIO", "Loading AVIS: %s", path);

    HANDLE hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile == INVALID_HANDLE_VALUE) {
        AVIS_ERR("FIO", "Failed to open: %s (Err: %lu)", path, GetLastError());
        return FALSE;
    }

    DWORD bytesRead = 0;
    if (!ReadFile(hFile, gAvisBuffer, sizeof(gAvisBuffer) - 1, &bytesRead, NULL))
    {
        AVIS_ERR("FIO", "Read failure on: %s", path);
        CloseHandle(hFile);
        return FALSE;
    }

    gAvisBuffer[bytesRead] = 0;
    gAvisLoaded = TRUE;
    CloseHandle(hFile);

    AVIS_MSG("FIO", "Loaded %lu bytes into workspace.", bytesRead);
    return TRUE;
}

/* ---------------------------------------------------------
   Save provided text to specific AVIS path
--------------------------------------------------------- */
BOOL MERCG_SaveAVIS(const char *path, const char *text)
{
    AVIS_INFO("FIO", "Saving AVIS to: %s", path);

    HANDLE hFile = CreateFileA(path, GENERIC_WRITE, 0,
        NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile == INVALID_HANDLE_VALUE) {
        AVIS_ERR("FIO", "Save access denied: %s", path);
        return FALSE;
    }

    DWORD len = (DWORD)lstrlenA(text);
    DWORD written = 0;
    BOOL ok = WriteFile(hFile, text, len, &written, NULL);
    CloseHandle(hFile);

    if (ok && written == len) {
        AVIS_MSG("FIO", "Successfully wrote %lu bytes.", written);
        return TRUE;
    }

    AVIS_ERR("FIO", "Write incomplete: %lu/%lu bytes written.", written, len);
    return FALSE;
}

/* ---------------------------------------------------------
   Validate COORD tag existence
--------------------------------------------------------- */
BOOL AvisHasCoord(const char *avisText)
{
    if (!avisText) return FALSE;

    const char *p = strstr(avisText, "COORD");
    if (!p) {
        AVIS_WARN("VALID", "No COORD tag detected in buffer.");
        return FALSE;
    }

    if (strstr(p, ":\\")) {
        return TRUE;
    }

    AVIS_WARN("VALID", "COORD tag found but lacks rooted path (:\\).");
    return FALSE;
}

/* ---------------------------------------------------------
   COORD-Sync: Extracts path from buffer and saves file
--------------------------------------------------------- */
BOOL FIO_SaveAVIS(void) 
{
    AVIS_INFO("SYNC", "Initiating COORD-based Save...");

    // 1. Find COORD tag
    const char *p = strstr(gAvisBuffer, "COORD");
    if (!p) {
        AVIS_ERR("SYNC", "Sync aborted: No 'COORD' tag found in workspace.");
        return FALSE;
    }

    // 2. Locate the delimiter
    const char *pathStart = strpbrk(p, ":=");
    if (!pathStart) {
        AVIS_ERR("SYNC", "Sync aborted: Malformed COORD tag (missing : or =).");
        return FALSE;
    }
    pathStart++; // skip the delimiter

    // 3. Trim whitespace
    while (*pathStart == ' ' || *pathStart == '\t') pathStart++;

    // 4. Extract Path
    char path[MAX_PATH];
    lstrcpynA(path, pathStart, MAX_PATH);

    // 5. Clean trailing newlines
    char *nl = strpbrk(path, "\r\n");
    if (nl) *nl = 0;

    if (path[0] == 0) {
        AVIS_ERR("SYNC", "Sync aborted: COORD tag is empty.");
        return FALSE;
    }

    AVIS_MSG("SYNC", "Writing buffer to target: %s", path);

    // 6. Physical Save
    HANDLE hFile = CreateFileA(path, GENERIC_WRITE, 0,
        NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        AVIS_ERR("SYNC", "Target access denied: %s", path);
        return FALSE;
    }

    DWORD written = 0;
    DWORD len = (DWORD)strlen(gAvisBuffer);
    BOOL ok = WriteFile(hFile, gAvisBuffer, len, &written, NULL);
    CloseHandle(hFile);

    if (ok) {
        AVIS_MSG("SYNC", "Sync complete: %lu bytes.", written);
    } else {
        AVIS_ERR("SYNC", "Critical write failure during file sync.");
    }

    return ok;
}
