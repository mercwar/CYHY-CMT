/*
AVIS:
    artifact: avis_core.c
    role: avis-core-implementation
    family: cyhy-cmt
    version: 2.0
*/
#include "avis_core.h"
#include "avis_file.h"
#include "avis-log.h"
#include <ctype.h>
#include <string.h>

#include <string.h>
#include "avis_defs.h"

#define LOGF AVIS_LogFmt


// ------------------------------------------------------------
// SHARED BUFFER
// ------------------------------------------------------------
char gAvisBuffer[4096];
BOOL gAvisLoaded = FALSE;

// ------------------------------------------------------------
// FILE PRIMITIVES
// ------------------------------------------------------------
bool AVIS_FileExists(const char *path)
{
    DWORD attr = GetFileAttributesA(path);
    return (attr != INVALID_FILE_ATTRIBUTES);
}


char *strtrimA(char *s)
{
    char *end;

    if (!s || !*s)
        return s;

    // Trim leading whitespace
    while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n')
        s++;

    if (*s == 0)
        return s;

    // Trim trailing whitespace
    end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'))
        end--;

    *(end + 1) = '\0';
    return s;
}

DWORD AVIS_FileSize(const char *path)
{
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (GetFileAttributesExA(path, GetFileExInfoStandard, &data))
        return data.nFileSizeLow;
    return 0;
}

bool AVIS_FileRead(const char *path, char *outBuf, size_t outSize)
{
    AVIS_INFO("FIO", "Reading artifact: %s", path);

    HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ,
                           NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (h == INVALID_HANDLE_VALUE) {
        AVIS_ERR("FIO", "Access Denied/Not Found: %s (Win32 Err: %lu)", path, GetLastError());
        return false;
    }

    DWORD size = GetFileSize(h, NULL);
    AVIS_MSG("FIO", "Physical file size: %lu bytes", size);

    // Safety: Ensure we don't overflow the AVIS workspace buffer (4096)
    if (size >= outSize) {
        AVIS_WARN("FIO", "Truncating file! Size %lu exceeds buffer capacity %zu.", size, outSize);
        size = (DWORD)(outSize - 1);
    }

    DWORD read = 0;
    if (ReadFile(h, outBuf, size, &read, NULL)) {
        outBuf[read] = 0;
        AVIS_MSG("FIO", "Successfully read %lu bytes into workspace.", read);
    } else {
        AVIS_ERR("FIO", "Critical ReadFile failure on: %s", path);
        CloseHandle(h);
        return false;
    }

    CloseHandle(h);
    return true;
}

bool AVIS_FileWrite(const char *path, const char *text)
{
    HANDLE h = CreateFileA(path, GENERIC_WRITE, 0,
                           NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE)
        return false;

    DWORD written = 0;
    WriteFile(h, text, (DWORD)strlen(text), &written, NULL);
    CloseHandle(h);
    return true;
}

// ------------------------------------------------------------
// REGISTRY PRIMITIVES
// ------------------------------------------------------------
bool AVIS_RegWriteString(HKEY root, const char *subkey,
                         const char *name, const char *value)
{
    HKEY hKey;
    if (RegCreateKeyA(root, subkey, &hKey) != ERROR_SUCCESS)
        return false;

    LONG r = RegSetValueExA(hKey, name, 0, REG_SZ,
                            (BYTE*)value, (DWORD)strlen(value) + 1);

    RegCloseKey(hKey);
    return (r == ERROR_SUCCESS);
}

bool AVIS_RegReadString(HKEY root, const char *subkey,
                        const char *name, char *out, DWORD outSize)
{
    HKEY hKey;
    if (RegOpenKeyA(root, subkey, &hKey) != ERROR_SUCCESS)
        return false;

    DWORD type = REG_SZ;
    LONG r = RegQueryValueExA(hKey, name, NULL, &type,
                              (BYTE*)out, &outSize);

    RegCloseKey(hKey);
    return (r == ERROR_SUCCESS);
}
