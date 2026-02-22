/*
AVIS:
    artifact: avis_file.h
    role: file-class-header
    family: cyhy-cmt
    version: 2.0
*/

#ifndef AVIS_FILE_H
#define AVIS_FILE_H

#include <windows.h>
#include <stdbool.h>


typedef struct _AVIS_FILE
{
    char fullpath[AVIS_MAX_PATH];   // Full absolute path
    char filename[AVIS_MAX_PATH];   // File name only
    char directory[AVIS_MAX_PATH];  // Directory only
    char extension[32];             // .txt, .md, etc.
    char version[64];               // Optional AVIS version tag

    bool exists;                    // File exists?
    DWORD sizeBytes;                // Size on disk
AVIS_HEADER_ AVIS_HEADER;
} AVIS_FILE;

/* API */
void AVIS_FILE_Init(AVIS_FILE *f, const char *path);
void AVIS_FILE_Refresh(AVIS_FILE *f);
bool AVIS_FILE_Read(AVIS_FILE *f);
bool AVIS_FILE_Write(AVIS_FILE *f, const char *text);
void AVIS_ParseLine(AVIS_HEADER_ *h, const char *line);

void AVIS_ParseHeader(AVIS_HEADER_ *h, const char *text);
static void AVIS_ParseHeaderLine(AVIS_HEADER_ *h, const char *line);
static void ExtractValue(char *dest, const char *line, const char *label, size_t destSize);

#endif
