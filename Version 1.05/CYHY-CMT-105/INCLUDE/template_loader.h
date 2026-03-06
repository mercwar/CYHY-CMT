/*
AVIS:
    artifact: template_loader.h
    role: avis-file-io-header
    family: cyhy-cmt
    version: 1.2
*/

#ifndef TEMPLATE_LOADER_H
#define TEMPLATE_LOADER_H

#include <windows.h>

// Load AVIS file into global buffer
BOOL MERCG_LoadAVIS(const char *path);

// Save AVIS file from provided text
BOOL MERCG_SaveAVIS(const char *path, const char *text);

// Save AVIS using current buffer (used by AUTO SAVE)
BOOL FIO_SaveAVIS(void);

// Check if AVIS text contains a valid COORD / rooted file path
BOOL AvisHasCoord(const char *avisText);

// Shared AVIS buffer
extern char gAvisBuffer[4096];
extern BOOL gAvisLoaded;

#endif
