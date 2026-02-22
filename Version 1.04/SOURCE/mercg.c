
/*
AVIS:
    artifact: mercg.c
    role: mercg-window
    family: cyhy-cmt
    version: 1.6 (Stabilized Header)
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <commdlg.h>
#include <stdio.h>
#include <ctype.h>
#include <direct.h>
#include <io.h>

#include "resource.h"
#include "mercg.h"
#include "mercg-editor.h"
#include "mercg-wins.h" 
#include "avis_defs.h"
#include "avis_core.h"
#include "avis_file.h"
#include "avis-log.h"

/* ---------------------------------------------------------
   GLOBALS & EXTERNS
--------------------------------------------------------- */
extern HINSTANCE hInst;
HWND hMercgMainWnd      = NULL; 
HWND hMercgEdit         = NULL;
HWND hMercgTerminal     = NULL;

const char MERCG_CLASS[] = "MERCG_Window";

char gMercgCurrentFile[MAX_PATH] = "";
char gMercgRoot[MAX_PATH]        = "";
int FILE_SURF_MODE               = 0; // 0 = HWND Crawler, 1 = File System Mode

/* --- STATIC CONTROLS (Single Declarations Only) --- */
static HWND hMercgPropLabel = NULL; 
static HWND hMercgTree      = NULL;
static BOOL bTreeActive     = FALSE; 

/* --- PROTOTYPES --- */
extern void MERCG_ToggleTopMost(HWND hwnd); 
static int CALLBACK MercgBrowseCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

/* ---------------------------------------------------------
   UI CONSTRUCTION
--------------------------------------------------------- */
static void CreateControls(HWND hWnd)
{
    HINSTANCE hInstLocal = (HINSTANCE)GetModuleHandle(NULL);

    CreateWindowA("BUTTON", "Dir", WS_CHILD | WS_VISIBLE, 10, 10, 60, 25, hWnd, (HMENU)ID_MERCG_CHOOSE_DIR, hInstLocal, NULL);
    CreateWindowA("BUTTON", "Open", WS_CHILD | WS_VISIBLE, 80, 10, 60, 25, hWnd, (HMENU)ID_MERCG_OPEN_AVIS, hInstLocal, NULL);
    CreateWindowA("BUTTON", "Save", WS_CHILD | WS_VISIBLE, 150, 10, 60, 25, hWnd, (HMENU)ID_MERCG_SAVE_AVIS, hInstLocal, NULL);
    CreateWindowA("BUTTON", "Scan", WS_CHILD | WS_VISIBLE, 220, 10, 60, 25, hWnd, (HMENU)ID_MERCG_SCAN_DIR, hInstLocal, NULL);

    hMercgPropLabel = CreateWindowA("STATIC", "Ready", WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP, 300, 15, 550, 20, hWnd, NULL, hInstLocal, NULL);

    hMercgTree = CreateWindowExA(WS_EX_CLIENTEDGE, WC_TREEVIEWA, "", 
        WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
        10, 45, 250, 400, hWnd, NULL, hInstLocal, NULL);

    hMercgEdit = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", 
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
        270, 45, 500, 400, hWnd, NULL, hInstLocal, NULL);

    hMercgTerminal = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", 
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
        10, 455, 760, 100, hWnd, (HMENU)IDC_TERMINAL, hInstLocal, NULL);

    // Initial State Log
    AVIS_INFO("MERCG", "MERC-G Inspector UI Components Virtualized.");
}
/* ---------------------------------------------------------
   MERC-G WINDOW PROCEDURE (mercg.c) - STABILIZED
--------------------------------------------------------- */
LRESULT CALLBACK MercgWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateControls(hWnd);
        SetTimer(hWnd, IDT_WIND_REFRESH, 5000, NULL);
        return 0;

    case WM_TIMER:
        if (wParam == IDT_WIND_REFRESH) {
            if (bTreeActive && FILE_SURF_MODE == 0) {
                AVIS_INFO("ROBOT", "Pulse Sync: [REFRESHING DESKTOP]");
                RefreshWindowTree(hMercgTree); 
            }
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {

case ID_OPTIONS_TOPMOST: {
    // 1. Get current state of the Inspector window
    LONG_PTR dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
    BOOL isTop = (dwExStyle & WS_EX_TOPMOST);
    
    // 2. Flip the Z-Order
    SetWindowPos(hWnd, isTop ? HWND_NOTOPMOST : HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    
    // 3. FIX THE CHECKBOX: Get the specific menu handle
    HMENU hMenu = GetMenu(hWnd);
    if (hMenu) {
        // We use MF_BYCOMMAND to ensure it finds the ID regardless of position
        CheckMenuItem(hMenu, ID_OPTIONS_TOPMOST, MF_BYCOMMAND | (isTop ? MF_UNCHECKED : MF_CHECKED));
    }

    // 4. UI Feedback on the label
    SetWindowTextA(hMercgPropLabel, isTop ? "Z-ORDER >> NORMAL" : "Z-ORDER >> TOPMOST");
    AVIS_INFO("MERCG", "Inspector Z-Order Shift: %s", isTop ? "NORMAL" : "TOPMOST");
    break;
}




case ID_TREE_CLIPBOARD_COPY: { // "Copy to Clipboard"
    HTREEITEM hSel = TreeView_GetSelection(hMercgTree);
    TVITEMA tvi = {0}; char buf[256];
    tvi.hItem = hSel; tvi.mask = TVIF_TEXT;
    tvi.pszText = buf; tvi.cchTextMax = sizeof(buf);
    
    if (TreeView_GetItem(hMercgTree, &tvi) && OpenClipboard(hWnd)) {
        EmptyClipboard();
        HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, strlen(buf) + 1);
        lstrcpyA((char*)GlobalLock(hGlob), buf);
        GlobalUnlock(hGlob);
        SetClipboardData(CF_TEXT, hGlob);
        CloseClipboard();
        AVIS_INFO("MERCG", "Clipboard Sync: %s", buf);
    }
    break;
}




case ID_MERCG_SAVE_AVIS: 
{
    // 1. Get the length of the text in the editor
    int len = GetWindowTextLengthA(hMercgEdit);
    if (len <= 0) {
        AVIS_ERR("FIO", "Save Aborted: Editor is empty.");
        break;
    }

    // 2. Decide WHERE to save
    char szSavePath[MAX_PATH];
    if (strlen(gMercgCurrentFile) > 0 && FILE_SURF_MODE == 1) {
        // QUICK SAVE: Overwrite the last scanned/opened file
        lstrcpyA(szSavePath, gMercgCurrentFile);
    } else {
        // SAVE AS: Trigger dialog if we are in HWND mode or first save
        OPENFILENAMEA ofn = { 0 };
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hWnd;
        ofn.lpstrFilter = "AVIS Files (*.avis)\0*.avis\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = szSavePath;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrInitialDir = gMercgRoot;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
        
        szSavePath[0] = '\0'; // Start empty
        if (!GetSaveFileNameA(&ofn)) {
            AVIS_INFO("FIO", "Save Operation Cancelled by User.");
            break;
        }
        // Update the current file pointer for next time
        lstrcpyA(gMercgCurrentFile, szSavePath);
        FILE_SURF_MODE = 1; // Shift to File Mode now that we have a disk artifact
    }

    // 3. Extract text and Commit to Disk
    char* szBuffer = (char*)malloc(len + 1);
    if (szBuffer) {
        GetWindowTextA(hMercgEdit, szBuffer, len + 1);
        
        FILE* f = fopen(szSavePath, "w"); // Open for writing
        if (f) {
            fputs(szBuffer, f);
            fclose(f);
            
            AVIS_MSG("FIO", "SAVE SUCCESS: %d bytes committed to %s", len, szSavePath);
            SetWindowTextA(hMercgPropLabel, "AVIS NAVIGATOR >> ARTIFACT COMMITTED");
        } else {
            AVIS_ERR("FIO", "WRITE FAILED: Access Denied to %s", szSavePath);
        }
        free(szBuffer);
    }
    break;
}

        case ID_MERCG_CHOOSE_DIR: {
            BROWSEINFOA bi = { 0 }; char path[MAX_PATH];
            bi.hwndOwner = hWnd; bi.lpszTitle = "Select Root";
            bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
            bi.lpfn = MercgBrowseCallback; 
            LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
            if (pidl && SHGetPathFromIDListA(pidl, path)) {
                lstrcpyA(gMercgRoot, path);
                AVIS_INFO("MERCG", "New Root: %s", gMercgRoot);
                CoTaskMemFree(pidl);
            }
            break;
        }

        case ID_MERCG_OPEN_AVIS: {
            OPENFILENAMEA ofn = { 0 }; char szFile[MAX_PATH] = "";
            ofn.lStructSize = sizeof(ofn); ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = "AVIS Files (*.avis)\0*.avis\0All Files (*.*)\0*.*\0";
            ofn.lpstrFile = szFile; ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrInitialDir = gMercgRoot;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
            if (GetOpenFileNameA(&ofn)) {
                if (MERCG_LoadFile(szFile)) {
                    SetWindowTextA(hMercgEdit, gAvisBuffer);
                    lstrcpyA(gMercgCurrentFile, szFile);
                }
            }
            break;
        }

        case ID_MERCG_SCAN_DIR: {
            AVIS_INFO("SCAN", "Initializing File System Crawl: %s", gMercgRoot);
            FILE_SURF_MODE = 1; 
            TreeView_DeleteAllItems(hMercgTree);
            WIN32_FIND_DATAA fd; char search[MAX_PATH];
            sprintf(search, "%s\\*.avis", gMercgRoot);
            HANDLE hFind = FindFirstFileA(search, &fd);
            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    TVINSERTSTRUCTA tvi = {0};
                    tvi.hParent = TVI_ROOT; tvi.item.mask = TVIF_TEXT | TVIF_PARAM;
                    tvi.item.pszText = fd.cFileName;
                    char* fullPath = (char*)malloc(MAX_PATH);
                    sprintf(fullPath, "%s\\%s", gMercgRoot, fd.cFileName);
                    tvi.item.lParam = (LPARAM)fullPath; 
                    TreeView_InsertItem(hMercgTree, &tvi);
                } while (FindNextFileA(hFind, &fd));
                FindClose(hFind);
            }
            break;
        }

        case ID_MENU_TOGGLE_WINTREE: {
            bTreeActive = !bTreeActive;
            CheckMenuItem(GetMenu(hWnd), ID_MENU_TOGGLE_WINTREE, bTreeActive ? MF_CHECKED : MF_UNCHECKED);
            if (bTreeActive) {
                FILE_SURF_MODE = 0; 
                SetWindowTextA(hMercgPropLabel, "AVIS NAVIGATOR >> WINDOW SYNC: [ACTIVE]");
                RefreshWindowTree(hMercgTree); 
            }
            break;
        }
case ID_TREE_POPUP_LOADFILE: {
    HTREEITEM hSel = TreeView_GetSelection(hMercgTree);
    TVITEMA tvi = {0};
    tvi.hItem = hSel;
    tvi.mask = TVIF_PARAM | TVIF_TEXT;
    char szNodeText[256];
    tvi.pszText = szNodeText;
    tvi.cchTextMax = sizeof(szNodeText);

    if (TreeView_GetItem(hMercgTree, &tvi) && tvi.lParam != 0) {
        
        // --- HWND MODE (0): Scrape Window Data ---
        if (FILE_SURF_MODE == 0) {
            HWND hTarget = (HWND)tvi.lParam;
            if (IsWindow(hTarget)) {
                AVIS_INFO("ROBOT", "Initiating Deep-Capture on HWND: 0x%p", (void*)hTarget);
                
                // Allocate a large buffer for the window content
                char* szContent = (char*)malloc(65536); 
                if (szContent) {
                    ZeroMemory(szContent, 65536);
                    
                    // WM_GETTEXT is more reliable than GetWindowText for external processes
                    DWORD_PTR dwResult;
                    if (SendMessageTimeoutA(hTarget, WM_GETTEXT, 65535, (LPARAM)szContent, 
                                           SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG, 
                                           1000, &dwResult)) 
                    {
                        SetWindowTextA(hMercgEdit, szContent);
                        AVIS_MSG("NAV", "SYNC SUCCESS: Captured %llu bytes from %s", (unsigned long long)strlen(szContent), szNodeText);
                    } else {
                        AVIS_ERR("ROBOT", "SYNC FAILED: Target 0x%p timed out or Access Denied.", (void*)hTarget);
                    }
                    free(szContent);
                }
            } else {
                AVIS_ERR("NAV", "INVALID TARGET: Window handle no longer exists.");
            }
        } 
        // --- AVIS MODE (1): Load File Artifact ---
        else if (FILE_SURF_MODE == 1) {
            if (MERCG_LoadFile((const char*)tvi.lParam)) {
                SetWindowTextA(hMercgEdit, gAvisBuffer);
                AVIS_MSG("NAV", "ARTIFACT SYNC: %s loaded.", szNodeText);
            }
        }
    }
    break;
}


        } // <--- CRITICAL: END OF WM_COMMAND switch
        break;
    case WM_NOTIFY: {
    LPNMHDR hdr = (LPNMHDR)lParam;
    if (hdr->hwndFrom == hMercgTree) 
    {
        // --- 1. RIGHT CLICK: SELECTS & SHOWS MENU ---
        if (hdr->code == NM_RCLICK) {
            TVHITTESTINFO ht = {0}; GetCursorPos(&ht.pt); ScreenToClient(hMercgTree, &ht.pt);
            HTREEITEM hItem = TreeView_HitTest(hMercgTree, &ht);
            if (hItem) {
                TreeView_SelectItem(hMercgTree, hItem); 
                HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_TREE_POPUP));
                if (hMenu) {
                    POINT pt; GetCursorPos(&pt);
                    TrackPopupMenu(GetSubMenu(hMenu, 0), TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
                    DestroyMenu(hMenu);
                }
            }
            return 0; 
        }

        // --- 2. DOUBLE CLICK: EXPAND/COLLAPSE ONLY ---
        // --- 2. DOUBLE CLICK: LOAD AVIS PROPERTIES INTO NODES ---
      // --- 2. DOUBLE CLICK: SURGICAL PROPERTY INJECTION ---
if (hdr->code == NM_DBLCLK) 
{
    TVHITTESTINFO ht = {0}; 
    GetCursorPos(&ht.pt); 
    ScreenToClient(hMercgTree, &ht.pt);

    HTREEITEM hItem = TreeView_HitTest(hMercgTree, &ht);
    if (hItem && FILE_SURF_MODE == 1) 
    {
        TVITEMA tvi = {0};
        char szDisplayName[MAX_PATH];
        tvi.hItem = hItem;
        tvi.mask = TVIF_PARAM | TVIF_TEXT; 
        tvi.pszText = szDisplayName;
        tvi.cchTextMax = MAX_PATH;

        if (TreeView_GetItem(hMercgTree, &tvi) && tvi.lParam != 0) 
        {
            const char* fullPath = (const char*)tvi.lParam;

            // 1. Wipe stale nodes
            HTREEITEM hChild = TreeView_GetChild(hMercgTree, hItem);
            while (hChild) {
                HTREEITEM hNext = TreeView_GetNextSibling(hMercgTree, hChild);
                TreeView_DeleteItem(hMercgTree, hChild);
                hChild = hNext;
            }

            // 2. Open and Scan for AVIS metadata
            FILE* f = fopen(fullPath, "r");
            if (f) {
                char line[512];
                int metaCount = 0;
                AVIS_INFO("SCAN", "Surgical Scan: %s", szDisplayName);
                
                while (fgets(line, sizeof(line), f)) {
                    // Stop if we hit the data body (+) or end of file
                    if (line[0] == '+') break; 

                    // Robust keyword detection (Case Insensitive-ish)
                    if (strstr(line, "artifact:") || strstr(line, "role:") || 
                        strstr(line, "family:")   || strstr(line, "version:") ||
                        strstr(line, "AVIS:")) 
                    {
                        // Clean line endings for TreeView
                        char* nl = strpbrk(line, "\r\n"); if (nl) *nl = 0;

                        TVINSERTSTRUCTA tvc = {0};
                        tvc.hParent = hItem;
                        tvc.item.mask = TVIF_TEXT;
                        tvc.item.pszText = line;
                        if (TreeView_InsertItem(hMercgTree, &tvc)) metaCount++;
                    }
                }
                fclose(f);
                
                if (metaCount > 0) {
                    TreeView_Expand(hMercgTree, hItem, TVE_EXPAND);
                    AVIS_MSG("SCAN", "Success: %d Metadata points mapped.", metaCount);
                } else {
                    AVIS_ERR("SCAN", "Structure Error: No AVIS keys found in %s", szDisplayName);
                }
            } else {
                AVIS_ERR("FIO", "CRITICAL: Path unreadable: %s", fullPath);
            }
        }
    }
    return 1; 
}

        // --- 3. LEFT CLICK (Selection): ONLY UPDATES LABEL (NO LOAD) ---
        if (hdr->code == TVN_SELCHANGEDA) {
            LPNMTREEVIEWA pnmtv = (LPNMTREEVIEWA)lParam;
            char text[MAX_PATH];
            TVITEMA item = {0};
            item.hItem = pnmtv->itemNew.hItem;
            item.mask = TVIF_TEXT;
            item.pszText = text;
            item.cchTextMax = sizeof(text);

            if (TreeView_GetItem(hMercgTree, &item)) {
                char navLog[300];
                wsprintfA(navLog, "AVIS NAVIGATOR >> READY TO SYNC: %s", text);
                SetWindowTextA(hMercgPropLabel, navLog);
            }
        }
    }
    break;
}

// Inside MercgWndProc in mercg.c
    case WM_CLOSE:
        ShowWindow(hWnd, SW_HIDE);
        AVIS_INFO("MERCG", "Inspector moved to BACKGROUND (Resident Mode).");
        return 0; // INTERCEPT: Do NOT call DestroyWindow

    case WM_DESTROY:
        // This only fires if CYHY explicitly kills it or the app exits
        AVIS_INFO("CORE", "MERC-G Inspector: Final Termination.");
        KillTimer(hWnd, IDT_WIND_REFRESH);
        hMercgMainWnd = NULL;
        return 0;

    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

/* ---------------------------------------------------------
   Isolated Callback Function (Fixes Error #2084)
--------------------------------------------------------- */
static int CALLBACK MercgBrowseCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) 
{
    if (uMsg == BFFM_INITIALIZED) 
    {
        SendMessageA(hwnd, BFFM_SETSELECTIONA, TRUE, (LPARAM)gMercgRoot);
    }
    return 0;
}


/* ---------------------------------------------------------
   MERCG INITIALIZATION - CALL THIS IN WinMain()
--------------------------------------------------------- */
void MERCG_Init(HINSTANCE hInst) {
    WNDCLASSEXA wc = {0};
    
    // 1. Ensure Class is clean
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.lpfnWndProc = MercgWndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = MERCG_CLASS;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Use standard color
    wc.lpszMenuName = NULL; // DO NOT load menu here (Prevents Err 5)

    if (!RegisterClassExA(&wc)) {
        AVIS_WARN("DIAG", "Class registration skipped (already exists).");
    }

    // 2. Create window with NO MENU first to bypass Access Denied (Err 5)
    hMercgMainWnd = CreateWindowExA(
        WS_EX_TOPMOST,            // Force Z-Order at creation
        MERCG_CLASS, 
        "MERC-G Robot Inspector", 
        WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT, 820, 620, 
        NULL, NULL, hInst, NULL   // Menu is NULL here
    );

    if (hMercgMainWnd) {
        // 3. LATE-LOAD MENU: If window exists, try to attach menu now
        HMENU hMenu = LoadMenuA(hInst, MAKEINTRESOURCEA(10001));
        if (hMenu) SetMenu(hMercgMainWnd, hMenu);
        
        AVIS_INFO("DIAG", "MERC-G Handle Synced: %p", hMercgMainWnd);
    } else {
        // If this STILL fails with Err 5, run MSYS64 as Administrator
        AVIS_ERR("DIAG", "FATAL: Window Creation Failed. Err: %lu", GetLastError());
    }
}

/* ---------------------------------------------------------
   FIXED SHOWPOPUP
--------------------------------------------------------- *//* ---------------------------------------------------------
   MERC-G ROBOT: DEPLOYMENT & LOG SYNC
--------------------------------------------------------- */
void MERCG_ShowPopup(HWND hParent) {
    char logBuf[256];
    
    // 1. RESIDENT RECOVERY: Re-build if handle is dead
    if (!hMercgMainWnd || !IsWindow(hMercgMainWnd)) {
        AVIS_INFO("MERCG", "Resident Sync: Handle lost. Re-building Component...");
        
        // Attempt recovery from OS window list
        hMercgMainWnd = FindWindowA(MERCG_CLASS, NULL);
        
        if (!hMercgMainWnd) {
            AVIS_INFO("MERCG", "Instance missing. Re-initializing via MERCG_Init...");
            MERCG_Init(GetModuleHandle(NULL)); 
        }
    }

    if (hMercgMainWnd && IsWindow(hMercgMainWnd)) {
        sprintf(logBuf, "Target Synchronized: HWND 0x%p", (void*)hMercgMainWnd);
        AVIS_MSG("MERCG", logBuf);

        // --- THE LOG ROBOT: RELINK TERMINAL ---
        // Crucial: Fixes "Log is Down" by piping the engine to the current UI
        if (hMercgTerminal && IsWindow(hMercgTerminal)) {
            AVIS_SetTerminalWindow(hMercgTerminal);
            AVIS_INFO("ROBOT", "Log Bot: Pipe re-established to Inspector.");
        }

        // --- THE FOCUS VIRUS KILLER: THREAD SYNC ---
        DWORD dwCurrentThread = GetCurrentThreadId();
        HWND hFore = GetForegroundWindow();
        DWORD dwTargetThread = GetWindowThreadProcessId(hFore, NULL);
        
        if (hFore != hMercgMainWnd && dwCurrentThread != dwTargetThread) {
            if (AttachThreadInput(dwCurrentThread, dwTargetThread, TRUE)) {
                AVIS_INFO("ROBOT", "Input Sync: Attached to Thread 0x%X", dwTargetThread);
            }
        }

        // 2. Deployment Sequence
        ShowWindow(hMercgMainWnd, SW_RESTORE); 
        SetWindowPos(hMercgMainWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        
        if (SetForegroundWindow(hMercgMainWnd)) {
            SetFocus(hMercgMainWnd);
            AVIS_INFO("ROBOT", "Focus Capture: SUCCESS.");
        } else {
            AVIS_ERR("ROBOT", "Focus Capture: DENIED (Err: %lu)", GetLastError());
        }

        // 3. Thread Release
        if (dwCurrentThread != dwTargetThread) {
            AttachThreadInput(dwCurrentThread, dwTargetThread, FALSE);
            AVIS_INFO("ROBOT", "Input Sync: Released.");
        }

        // 4. Reset Z-Order
        SetWindowPos(hMercgMainWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        AVIS_INFO("ROBOT", "UI Deployment: COMPLETE.");

    } else {
        AVIS_ERR("MERCG", "CRITICAL: Recovery Failed. Logical instance is UNREACHABLE.");
    }
}

/* ---------------------------------------------------------
   ENVIRONMENT FORCING (Fixes cyhy.c Undeclared Warning)
--------------------------------------------------------- */
void MERCG_ForceEnvironment(const char* root) {
    char path[MAX_PATH];
    
    // Create \factory
    sprintf(path, "%s\\factory", root);
    if (_access(path, 0) != 0) {
        _mkdir(path);
        AVIS_INFO("FIO", "Force-Created: %s", path);
    }

    // Update Global Root for the Scanner
    lstrcpyA(gMercgRoot, root);
    AVIS_INFO("CORE", "Environment Locked: %s", root);
}

