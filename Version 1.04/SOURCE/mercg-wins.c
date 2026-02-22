#include "mercg-wins.h"
#include "avis-log.h"
#include <stdio.h>
#include "mercg-wins.h"
#include "avis-log.h"



// REMOVE: const char MERCG_CLASS[] from here to avoid "multiply defined" error

// ... after includes ...
extern void MERCG_ToggleTopMost(HWND hwnd); // Fixes warning #2018

HWND hMercgMainWnd = NULL;

HWND hMercgEdit = NULL;
HWND hMercgTerminal = NULL;
char gMercgCurrentFile[MAX_PATH] = "";
char gMercgRoot[MAX_PATH] = "";
int FILE_SURF_MODE = 0; // 0 = HWND, 1 = File

// ONLY ONE DECLARATION OF THESE:
static HWND hMercgPropLabel = NULL; 
static HWND hMercgTree = NULL;
static BOOL bTreeActive = FALSE;
// Struct to pass parent context through the recursive crawl
typedef struct {
    HWND hTree;
    HTREEITEM hParentNode;
} CRAWL_DATA;

// PROTOTYPE FOR CALLBACK
static int CALLBACK MercgBrowseCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

// --- TOPMOST CONTROLLER ---
BOOL MERCG_IsTopMost(HWND hwnd) {
    return (GetWindowLongPtr(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST) != 0;
}

// REMOVE: static HWND hMercgTree, etc. (These belong in mercg.c)

// CHANGE: Ensure this is NOT static so mercg.obj can see it
/* --- REMOVE THE OLD DEFINITION AT LINE 38 --- */

// ... other code ...

/* --- KEEP THIS ONE AT THE BOTTOM --- */
void MERCG_ToggleTopMost(HWND hWnd) { 
    if (!IsWindow(hWnd)) return;

    LONG_PTR dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE); 
    BOOL isTop = (dwExStyle & WS_EX_TOPMOST);

    if (isTop) {
        SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        AVIS_INFO("Z-ORDER", "Artifact 0x%p released.", (void*)hWnd);
    } else {
        SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        AVIS_INFO("Z-ORDER", "Artifact 0x%p pinned.", (void*)hWnd);
    }
}

// --- ROBOT ENUM ENGINE ---

// THE RECURSIVE ENGINE: This finds children of children
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
    CRAWL_DATA* parentData = (CRAWL_DATA*)lParam;
    char title[256], className[128], nodeText[512];

    GetWindowTextA(hwnd, title, sizeof(title));
    GetClassNameA(hwnd, className, sizeof(className));

    // Format: "Title [ClassName] (HWND)"
    sprintf(nodeText, "%s [%s] (0x%p)", 
            (strlen(title) > 0) ? title : "{No Title}", 
            className, (void*)hwnd);

    TVINSERTSTRUCTA tvi = {0};
    tvi.hParent = parentData->hParentNode;
    tvi.hInsertAfter = TVI_LAST;
    tvi.item.mask = TVIF_TEXT | TVIF_PARAM;
    tvi.item.pszText = nodeText;
    tvi.item.lParam = (LPARAM)hwnd; // Store HWND for Sync

    HTREEITEM hThisNode = TreeView_InsertItem(parentData->hTree, &tvi);

    // Deep Crawl: Now find children of THIS specific child
    CRAWL_DATA nextLevel = { parentData->hTree, hThisNode };
    EnumChildWindows(hwnd, EnumChildProc, (LPARAM)&nextLevel);

    return TRUE;
}

// TOP-LEVEL CRAWL: Starts the process for all Desktop windows
BOOL CALLBACK EnumWinProc(HWND hwnd, LPARAM lParam) {
    HWND hTree = (HWND)lParam;
    if (!IsWindowVisible(hwnd)) return TRUE; // Skip hidden processes for performance

    char title[256], className[128], nodeText[512];
    GetWindowTextA(hwnd, title, sizeof(title));
    GetClassNameA(hwnd, className, sizeof(className));

    if (strlen(title) > 0) {
        sprintf(nodeText, "%s [%s]", title, className);

        TVINSERTSTRUCTA tvi = {0};
        tvi.hParent = TVI_ROOT;
        tvi.item.mask = TVIF_TEXT | TVIF_PARAM;
        tvi.item.pszText = nodeText;
        tvi.item.lParam = (LPARAM)hwnd;
        
        HTREEITEM hRootNode = TreeView_InsertItem(hTree, &tvi);

        // START RECURSION: Find all children inside this Top-Level Window
        CRAWL_DATA data = { hTree, hRootNode };
        EnumChildWindows(hwnd, EnumChildProc, (LPARAM)&data);
    }
    return TRUE;
}

void RefreshWindowTree(HWND hTree) {
    if (!hTree || !IsWindow(hTree)) return;
    SendMessage(hTree, WM_SETREDRAW, FALSE, 0); // Prevent flicker
    TreeView_DeleteAllItems(hTree);
    EnumWindows(EnumWinProc, (LPARAM)hTree);
    SendMessage(hTree, WM_SETREDRAW, TRUE, 0);
}
/* --- REMOVE THE OLD DEFINITION AT LINE 38 --- */

// ... other code ...

/* --- KEEP THIS ONE AT THE BOTTOM --- */


