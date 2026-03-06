/*
AVIS:
    artifact: avis_file.c
    role: file-class-implementation
    family: cyhy-cmt
    version: 2.0
*/


#include <string.h>
#include "avis_defs.h"
#include "avis_core.h"
#include "avis_file.h"
#define LOGF AVIS_LogFmt
static void AVIS_FILE_Split(AVIS_FILE *f)
{
    // Extract filename
    const char *slash = strrchr(f->fullpath, '\\');
    if (slash)
        strncpy(f->filename, slash + 1, AVIS_MAX_PATH - 1);
    else
        strncpy(f->filename, f->fullpath, AVIS_MAX_PATH - 1);

    // Extract directory
    if (slash)
    {
        size_t len = slash - f->fullpath;
        strncpy(f->directory, f->fullpath, len);
        f->directory[len] = 0;
    }

    // Extract extension
    const char *dot = strrchr(f->filename, '.');
    if (dot)
        strncpy(f->extension, dot, sizeof(f->extension) - 1);
}

void AVIS_FILE_Init(AVIS_FILE *f, const char *path)
{
    ZeroMemory(f, sizeof(AVIS_FILE));
    strncpy(f->fullpath, path, AVIS_MAX_PATH - 1);

    AVIS_FILE_Split(f);
    AVIS_FILE_Refresh(f);
}

void AVIS_FILE_Refresh(AVIS_FILE *f)
{
    f->exists    = AVIS_FileExists(f->fullpath);
    f->sizeBytes = AVIS_FileSize(f->fullpath);
}
static void AVIS_ParseHeaderLine(AVIS_HEADER_ *h, const char *line)
{
    char key[64], value[256];
    
    // Find either '=' or ':'
    const char *delim = strpbrk(line, "=:");
    if (!delim) return;

    // Extract key (before delimiter)
    size_t klen = delim - line;
    if (klen >= sizeof(key)) klen = sizeof(key) - 1;
    memcpy(key, line, klen);
    key[klen] = '\0';

    // Extract value (after delimiter)
    const char *vStart = delim + 1;
    
    // Skip any leading whitespace or extra delimiters (like "role: = terminal")
    while (*vStart == ' ' || *vStart == '\t' || *vStart == '=' || *vStart == ':') 
        vStart++;

    lstrcpynA(value, vStart, sizeof(value));

    // Strip trailing CR/LF
    size_t len = strlen(value);
    while (len > 0 && (value[len - 1] == '\r' || value[len - 1] == '\n' || value[len - 1] == ' '))
    {
        value[len - 1] = '\0';
        len--;
    }

    strtrimA(key);
    strtrimA(value);

    // Map to struct
    if (_stricmp(key, "file") == 0)      lstrcpynA(h->file, value, sizeof(h->file));
    else if (_stricmp(key, "path") == 0) lstrcpynA(h->path, value, sizeof(h->path));
    else if (_stricmp(key, "role") == 0) lstrcpynA(h->role, value, sizeof(h->role));
    else if (_stricmp(key, "platform") == 0) lstrcpynA(h->platform, value, sizeof(h->platform));
    else if (_stricmp(key, "family") == 0)   lstrcpynA(h->family, value, sizeof(h->family));
    else if (_stricmp(key, "version") == 0)  lstrcpynA(h->version, value, sizeof(h->version));
}

void AVIS_ParseHeader(AVIS_HEADER_ *h, const char *text)
{
    char line[256];
    const char *p = text;

    while (*p)
    {
        // Read one line
        int i = 0;
        while (*p && *p != '\n' && i < sizeof(line)-1)
            line[i++] = *p++;
        line[i] = '\0';

        if (*p == '\n')
            p++;

        // Parse key=value
        AVIS_ParseHeaderLine(h, line);
    }
}



static void ExtractValue(char *dest, const char *line, const char *label, size_t destSize) {
    const char *p = strstr(line, label);
    if (p) {
        p += strlen(label);
        
        // Skip whitespace AND delimiters (= or :)
        while (*p == ' ' || *p == '\t' || *p == ':' || *p == '=') 
            p++;
            
        lstrcpynA(dest, p, (int)destSize);
        
        // Trim trailing newline/spaces
        char *end = dest + strlen(dest) - 1;
        while (end >= dest && (*end == '\r' || *end == '\n' || *end == ' ')) {
            *end = '\0';
            end--;
        }
    }
}

/*
AVIS:
    artifact: avis_file.c
    role: metadata-parser-fix
    version: 2.1 (Full-Buffer Fix)
*/

bool AVIS_FILE_Read(AVIS_FILE *f) {
    // 1. Load the raw file into the global workspace buffer
    if (!AVIS_FileRead(f->fullpath, gAvisBuffer, sizeof(gAvisBuffer))) {
        return false;
    }

    // 2. FIX: Create a temporary ARRAY (not a char) for parsing
    // This prevents strtok from destroying gAvisBuffer with null terminators
    static char tempBuf[8192]; 
    memcpy(tempBuf, gAvisBuffer, sizeof(tempBuf));

    // 3. Reset the header struct
    memset(&f->AVIS_HEADER, 0, sizeof(AVIS_HEADER_));

    // 4. Parse the COPY for metadata tags
    char *line = strtok(tempBuf, "\n");
    while (line != NULL) {
        if (strstr(line, "role"))     ExtractValue(f->AVIS_HEADER.role,     line, "role",     sizeof(f->AVIS_HEADER.role));
        if (strstr(line, "family"))   ExtractValue(f->AVIS_HEADER.family,   line, "family",   sizeof(f->AVIS_HEADER.family));
        if (strstr(line, "version"))  ExtractValue(f->AVIS_HEADER.version,  line, "version",  sizeof(f->AVIS_HEADER.version));
        if (strstr(line, "platform")) ExtractValue(f->AVIS_HEADER.platform, line, "platform", sizeof(f->AVIS_HEADER.platform));
        if (strstr(line, "COORD"))    ExtractValue(f->AVIS_HEADER.coords,   line, "COORD",    sizeof(f->AVIS_HEADER.coords));
        
        line = strtok(NULL, "\n");
    }

    // gAvisBuffer is now preserved for the MERC-G Edit window!
    return true;
}


bool AVIS_FILE_Write(AVIS_FILE *f, const char *text)
{
    bool ok = AVIS_FileWrite(f->fullpath, text);
    AVIS_FILE_Refresh(f);
    return ok;
}
