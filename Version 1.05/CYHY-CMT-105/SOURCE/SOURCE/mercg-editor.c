/*
AVIS:
    artifact: mercg-editor.c
    role: mercg-editor-engine
    family: cyhy-cmt
    version: 1.2 (Fixed & Logged)
*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <ctype.h>
#include "resource.h"
#include "avis_defs.h"
#include "avis_core.h"
#include "avis_file.h"
#include "avis-log.h"
#include "mercg-editor.h"

/* ---------------------------------------------------------
   External globals (from MERCG main module)
--------------------------------------------------------- */
extern HWND hMercgEdit;              
extern char gMercgCurrentFile[MAX_PATH];
extern char gMercgRoot[MAX_PATH];

/* ---------------------------------------------------------
   Internal helpers
--------------------------------------------------------- */
static void MERCG_EditSetText(const char *text)
{
    SetWindowTextA(hMercgEdit, text ? text : "");
}

static void MERCG_EditGetText(char *buf, int bufSize)
{
    if (!buf || bufSize <= 0) return;
    GetWindowTextA(hMercgEdit, buf, bufSize);
}

/* Helper to strip special characters from property sub-nodes */
void CleanAvisLabel(char *s) {
    if (!s) return;
    char *src = s, *dst = s;
    while (*src) {
        // Keep only alphanumeric, spaces, and the colon
        if (isalnum((unsigned char)*src) || *src == ' ' || *src == ':') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

/* ---------------------------------------------------------
   Load file into MERCG editor (AVIS_FILE + gAvisBuffer)
--------------------------------------------------------- */
BOOL MERCG_LoadFile(const char *path)
{
    if (!path || !path[0]) return FALSE;

    AVIS_FILE f;
    AVIS_FILE_Init(&f, path);

    if (!AVIS_FILE_Read(&f))
    {
        AVIS_ERR("MERCG", "Failed to load: %s", path);
        return FALSE;
    }

    // gAvisBuffer is filled by AVIS_FILE_Read
    // The multi-line fix is handled in the TreeView double-click handler in mercg.c
    lstrcpyA(gMercgCurrentFile, path);
    AVIS_MSG("MERCG", "Active Artifact: %s", path);
    return TRUE; 
}

/* ---------------------------------------------------------
   Save MERCG editor text to file (AVIS_FILE)
--------------------------------------------------------- */
void MERCG_SaveFile(const char *path)
{
    if (!path || !path[0])
    {
        AVIS_ERR("MERCG", "Save failed: No target path.");
        return;
    }

    char buf[4096];
    MERCG_EditGetText(buf, sizeof(buf));

    AVIS_FILE f;
    AVIS_FILE_Init(&f, path);

    if (AVIS_FILE_Write(&f, buf))
    {
        AVIS_MSG("MERCG", "Saved: %s (%lu bytes)", path, (unsigned long)strlen(buf));
    }
    else
    {
        AVIS_ERR("MERCG", "Critical Write Failure: %s", path);
    }
}

/* ---------------------------------------------------------
   Load default ACIT template from factory under MERCG root
--------------------------------------------------------- */
void MERCG_LoadDefaultTemplate(void)
{
    AVIS_INFO("MERCG", "Loading Factory Default ACIT Template...");

    if (!gMercgRoot[0])
    {
        AVIS_ERR("MERCG", "CRITICAL: Root not set, cannot locate factory.");
        return;
    }

    char path[MAX_PATH];
    wsprintfA(path, "%s\\factory\\acit.txt", gMercgRoot);

    AVIS_FILE f;
    AVIS_FILE_Init(&f, path);

    if (AVIS_FILE_Read(&f))
    {
        MERCG_EditSetText(gAvisBuffer);
        lstrcpyA(gMercgCurrentFile, path);
        AVIS_MSG("MERCG", "Factory ACIT loaded into editor: %s", path);
    }
    else
    {
        AVIS_ERR("MERCG", "CRITICAL: Factory ACIT template missing at %s", path);
    }
}

/* ---------------------------------------------------------
   Set MERCG root directory (called from CYHY startup)
--------------------------------------------------------- */
void MERCG_SetRoot(const char *root)
{
    if (!root || !root[0])
    {
        gMercgRoot[0] = 0;
        AVIS_WARN("MERCG", "Workspace root cleared.");
        return;
    }

    lstrcpyA(gMercgRoot, root);
    AVIS_INFO("MERCG", "Workspace root set to: %s", gMercgRoot);
}

/* ---------------------------------------------------------
   Save current file (if any)
--------------------------------------------------------- */
void MERCG_SaveCurrent(void)
{
    if (!gMercgCurrentFile[0])
    {
        AVIS_WARN("MERCG", "Save ignored: No active file focused.");
        return;
    }

    AVIS_INFO("MERCG", "Syncing changes to current file...");
    MERCG_SaveFile(gMercgCurrentFile);
}
