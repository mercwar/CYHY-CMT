/*
AVIS:
    artifact: mercg.c
    role: mercg-window
    family: cyhy-cmt
    version: 1.5 (Delimiter Filtered)
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <commdlg.h>
#include <stdio.h>
#include <ctype.h>

#include "resource.h"
#include "mercg-editor.h"
#include "avis_defs.h"
#include "avis_core.h"
#include "avis_file.h"
#include "avis-log.h"

extern HINSTANCE hInst;

const char MERCG_CLASS[] = "MERCG_Window";
HWND hMercgEdit = NULL;
HWND hMercgTerminal = NULL;
char gMercgCurrentFile[MAX_PATH] = "";
char gMercgRoot[MAX_PATH] = "";
HWND hMercgPropLabel;
static HWND hMercgTree = NULL;

/* ---------------------------------------------------------
   BROWSE CALLBACK
--------------------------------------------------------- */
static int CALLBACK MercgBrowseCallback(HWND hwnd, UINT msg, LPARAM lParam, LPARAM lpData)
{
    if (msg == BFFM_INITIALIZED && lpData)
        SendMessageA(hwnd, BFFM_SETSELECTIONA, TRUE, (LPARAM)lpData);
    return 0;
}

/* ---------------------------------------------------------
   CONTROL CREATION
--------------------------------------------------------- */
static void CreateControls(HWND hWnd)
{
    HINSTANCE hInstLocal = (HINSTANCE)GetModuleHandle(NULL);

    CreateWindowA("BUTTON", "Dir", WS_CHILD | WS_VISIBLE, 10, 10, 60, 25, hWnd, (HMENU)ID_MERCG_CHOOSE_DIR, hInstLocal, NULL);
    CreateWindowA("BUTTON", "Open", WS_CHILD | WS_VISIBLE, 80, 10, 60, 25, hWnd, (HMENU)ID_MERCG_OPEN_AVIS, hInstLocal, NULL);
    CreateWindowA("BUTTON", "Save", WS_CHILD | WS_VISIBLE, 150, 10, 60, 25, hWnd, (HMENU)ID_MERCG_SAVE_AVIS, hInstLocal, NULL);
    CreateWindowA("BUTTON", "Scan", WS_CHILD | WS_VISIBLE, 220, 10, 60, 25, hWnd, (HMENU)ID_MERCG_SCAN_DIR, hInstLocal, NULL);

    hMercgPropLabel = CreateWindowA("STATIC", "Ready", WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP, 300, 15, 450, 20, hWnd, NULL, hInstLocal, NULL);

    hMercgTree = CreateWindowExA(WS_EX_CLIENTEDGE, WC_TREEVIEWA, "", 
        WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
        10, 45, 250, 400, hWnd, NULL, hInstLocal, NULL);

    hMercgEdit = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", 
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
        270, 45, 500, 400, hWnd, NULL, hInstLocal, NULL);

    hMercgTerminal = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", 
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
        10, 455, 760, 100, hWnd, (HMENU)IDC_TERMINAL, hInstLocal, NULL);

    AVIS_SetTerminalWindow(hMercgTerminal);
    AVIS_INFO("MERCG", "MERC-G Inspector Terminal Initialized.");
}

/* ---------------------------------------------------------
   WINDOW PROC
--------------------------------------------------------- */
static LRESULT CALLBACK MercgWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateControls(hWnd);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_MERCG_OPEN_AVIS:
{
    char path[MAX_PATH] = "";
    OPENFILENAMEA ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = "AVIS Files (*.avis)\0*.avis\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameA(&ofn)) 
    {
        AVIS_INFO("MERCG", "Manual load: %s", path);
        if (MERCG_LoadFile(path)) 
        {
            // FIX: Convert \n to \r\n so the multi-line EDIT control 
            // shows the WHOLE file, including properties starting with '+'
            static char fixBuf[16384]; 
            char *s = gAvisBuffer;
            char *d = fixBuf;
            
            while (*s && (d - fixBuf < sizeof(fixBuf) - 2)) 
            {
                if (*s == '\n' && (s == gAvisBuffer || *(s-1) != '\r')) {
                    *d++ = '\r'; // Inject Carriage Return for Windows
                }
                *d++ = *s++;
            }
            *d = '\0';
            SetWindowTextA(hMercgEdit, fixBuf);
            lstrcpyA(gMercgCurrentFile, path);
        }
    }
    return 0;
}


        case ID_MERCG_CHOOSE_DIR:
        {
            BROWSEINFOA bi = {0};
            char dir[MAX_PATH] = "";
            bi.hwndOwner = hWnd;
            bi.lpszTitle = "Select MERCG Source Root";
            bi.ulFlags   = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
            bi.lpfn      = MercgBrowseCallback;
            bi.lParam    = (LPARAM)gMercgRoot;

            LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
            if (pidl && SHGetPathFromIDListA(pidl, dir)) {
                lstrcpynA(gMercgRoot, dir, MAX_PATH);
                AVIS_INFO("MERCG", "Source Root: %s", gMercgRoot);
                MERCG_SetRoot(gMercgRoot);
                CoTaskMemFree(pidl);
            }
            return 0;
        }

        case ID_MERCG_SAVE_AVIS:
            MERCG_SaveCurrent();
            return 0;

        case ID_MERCG_SCAN_DIR:
        {
            AVIS_INFO("SCAN", "Rebuilding AVIS Tree...");
            TreeView_DeleteAllItems(hMercgTree);

            WIN32_FIND_DATAA fd;
            char search[MAX_PATH];
            wsprintfA(search, "%s\\*.avis", gMercgRoot);

            HANDLE hFind = FindFirstFileA(search, &fd);
            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    char full[MAX_PATH];
                    wsprintfA(full, "%s\\%s", gMercgRoot, fd.cFileName);

                    AVIS_FILE f;
                    AVIS_FILE_Init(&f, full);
                    if (!AVIS_FILE_Read(&f)) continue;

                    TVINSERTSTRUCTA tvi = {0};
                    tvi.hParent = TVI_ROOT;
                    tvi.item.mask = TVIF_TEXT | TVIF_PARAM;
                    tvi.item.pszText = fd.cFileName;
                    tvi.item.lParam = (LPARAM)strdup(full); 
                    HTREEITEM hFileNode = TreeView_InsertItem(hMercgTree, &tvi);

                    char buf[256];
                    #define ADD_PROP(lbl, val) \
                        wsprintfA(buf, "%s: %s", lbl, (val && val[0]) ? val : "(none)"); \
                        tvi.hParent = hFileNode; \
                        tvi.item.pszText = buf; \
                        tvi.item.lParam = 0; \
                        TreeView_InsertItem(hMercgTree, &tvi);

                    ADD_PROP("Role", f.AVIS_HEADER.role);
                    ADD_PROP("Family", f.AVIS_HEADER.family);
                    ADD_PROP("Version", f.AVIS_HEADER.version);
                    ADD_PROP("Platform", f.AVIS_HEADER.platform);
                    ADD_PROP("Coords", f.AVIS_HEADER.coords);

                } while (FindNextFileA(hFind, &fd));
                FindClose(hFind);
            }
            AVIS_MSG("SCAN", "Tree Population Complete.");
            return 0;
        }
        }
        break;
    case WM_NOTIFY:
    {
        LPNMHDR hdr = (LPNMHDR)lParam;
        if (hdr->hwndFrom == hMercgTree) 
        {
            /* 1. SELECTION CHANGE - Strip the "=" for a clean AI Search Result */
            if (hdr->code == TVN_SELCHANGEDA) 
            {
                LPNMTREEVIEWA pnmtv = (LPNMTREEVIEWA)lParam;
                char text[256];
                TVITEMA item = {0};
                item.hItem = pnmtv->itemNew.hItem;
                item.mask = TVIF_TEXT | TVIF_PARAM;
                item.pszText = text;
                item.cchTextMax = sizeof(text);

                if (TreeView_GetItem(hMercgTree, &item)) {
                    // Check for the "=" or ":" delimiter
                    char *delim = strpbrk(text, "=:");

                    if (item.lParam == 0 && delim) {
                        char propName[128] = {0};
                        char propVal[128] = {0};
                        char displayBuf[512];

                        // Extract Name (Everything before the "=")
                        int nameLen = (int)(delim - text);
                        lstrcpynA(propName, text, nameLen + 1);

                        // Extract Value (Everything after the "=")
                        char *valStart = delim + 1;
                        // Skip any leading whitespace (e.g., "family= cbord" becomes "cbord")
                        while (*valStart == ' ' || *valStart == '\t') valStart++;
                        lstrcpyA(propVal, valStart);

                        // Final Display: "LAST AVIS AI SEARCH RESULT [Name] [Value]"
                        wsprintfA(displayBuf, "LAST AVIS AI SEARCH RESULT [%s] [%s]", propName, propVal);
                        
                        SetWindowTextA(hMercgPropLabel, displayBuf);
                        AVIS_INFO("MERCG", "AI Search Result Updated: %s", propName);
                    }
                }
            }
            /* 2. DOUBLE CLICK - Existing logic for loading file to editor */
                 else if (hdr->code == NM_DBLCLK) 
            {
                HTREEITEM hSel = TreeView_GetSelection(hMercgTree);
                TVITEMA item; // Explicit declaration to fix #2048
                
                ZeroMemory(&item, sizeof(item));
                item.hItem = hSel;
                item.mask = TVIF_PARAM;

                if (TreeView_GetItem(hMercgTree, &item) && item.lParam != 0) 
                {
                    AVIS_INFO("MERCG", "Loading Artifact: %s", (const char*)item.lParam);
                    
                    // Reload file to ensure gAvisBuffer is fresh and un-truncated
                    if (MERCG_LoadFile((const char*)item.lParam)) 
                    {
                        // FIX: Win32 Edit Controls require \r\n to show multiple lines.
                        static char fixBuf[16384]; 
                        memset(fixBuf, 0, sizeof(fixBuf));
                        
                        char *s = gAvisBuffer;
                        char *d = fixBuf;
                        
                        // Loop through the entire buffer to fix Windows line endings
                        while (*s && (d - fixBuf < sizeof(fixBuf) - 2)) 
                        {
                            if (*s == '\n' && (s == gAvisBuffer || *(s-1) != '\r')) {
                                *d++ = '\r'; // Inject Carriage Return
                            }
                            *d++ = *s++;
                        }
                        *d = '\0';
                        
                        SetWindowTextA(hMercgEdit, fixBuf);
                        AVIS_MSG("MERCG", "Full multi-line buffer sync successful: %d bytes.", (int)strlen(fixBuf));
                    }
                }
            }

        }
        break;
    }


    case WM_CLOSE:
        AVIS_INFO("MERCG", "Closing Inspector.");
        DestroyWindow(hWnd);
        return 0;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

void MERCG_ShowPopup(HWND parent)
{
    static int registered = 0;
    if (!registered) {
        WNDCLASSA wc = {0};
        wc.lpfnWndProc = MercgWndProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        wc.lpszClassName = MERCG_CLASS;
        RegisterClassA(&wc);
        registered = 1;
    }
    CreateWindowExA(WS_EX_TOOLWINDOW, MERCG_CLASS, "MERC-G AVIS Inspector", 
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, parent, NULL, GetModuleHandle(NULL), NULL);
}
