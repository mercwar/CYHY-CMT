#ifndef AVIS_CORE_H
#define AVIS_CORE_H

#include "avis_defs.h"
#include "avis-log.h"
#include <stdint.h>
#include <stdbool.h>

// 1. SHARED BUFFER (External Linkage)
extern char gAvisBuffer[4096];
extern BOOL gAvisLoaded;
bool AVIS_RegWriteString(HKEY root, const char *subkey, const char *name, const char *value);
bool AVIS_RegReadString(HKEY root, const char *subkey, const char *name, char *out, DWORD outSize);

// 2. FILE PRIMITIVE PROTOTYPES
bool AVIS_FileRead(const char *path, char *outBuf, size_t outSize);
bool AVIS_FileWrite(const char *path, const char *text);
bool AVIS_FileExists(const char *path);
DWORD AVIS_FileSize(const char *path);

// 3. UTILITY PROTOTYPES
char *strtrimA(char *s);

// 4. HEADER STRUCTURE
/* 
   Updated Structure in avis_core.h 
*/
typedef struct _AVIS_HEADER_ {
    char avis_id[256];
    char coords[256];
    char file[AVIS_MAX_PATH];
    char path[AVIS_MAX_PATH];
    char role[128];
    char platform[64];
    char family[128];     // <--- ADDED: Fixes the 4 unknown member errors
    char version[64];
    char raw[4096];
    bool validated;
} AVIS_HEADER_;


#endif
