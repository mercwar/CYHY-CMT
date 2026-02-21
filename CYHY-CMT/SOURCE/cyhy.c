/*
AVIS:
    artifact: cyhy.c
    role: main-editor
    family: cyhy-cmt
    version: 6.3 (Terminal-Integrated)
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <shlobj.h>

#include "common.h"
#include "resource.h"
#include "cyhy.h"
#include "avis-log.h"
#include "cyhy-editor.h"
#include "buttons_create.h"
#include "buttons_proc.h"
#include "template_loader.h"
#include "auto_timer.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")

/* ---------------------------------------------------------
   GLOBALS
--------------------------------------------------------- */
const char CYHY_CLASS[]  = "CYHY_Main";
extern const char MERCG_CLASS[];

char gAppPath[MAX_PATH] = "";
HWND hTree        = NULL;
HWND hEdit        = NULL;
HWND hAutoSaveChk = NULL;
HWND hTerminal    = NULL; // LIVE TERMINAL DISPLAY

BOOL gAutoSave = TRUE;
char gProjectDir[MAX_PATH]  = "";
char gCurrentFile[MAX_PATH] = "";

CYHY_FILE gFileList[MAX_CYHY_FILES];
int gFileCount = 0;
char gMercgRoot[MAX_PATH] = "";

LRESULT CALLBACK CyhyWndProc(HWND, UINT, WPARAM, LPARAM);

/* ---------------------------------------------------------
   WinMain
--------------------------------------------------------- */
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd)
{
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC  = ICC_TREEVIEW_CLASSES;
    InitCommonControlsEx(&icc);

    WNDCLASSA wc = {0};
    wc.lpfnWndProc   = CyhyWndProc;
    wc.hInstance     = hInst;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = CYHY_CLASS;
    RegisterClassA(&wc);

    HWND hWnd = CreateWindowExA(0, CYHY_CLASS, "CYHY Project Editor [Logged]", 
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 750, 
        NULL, NULL, hInst, NULL);

    AVIS_INFO("CORE", "CYHY WinMain initialized.");

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

/* ---------------------------------------------------------
   Window Procedure
--------------------------------------------------------- */
LRESULT CALLBACK CyhyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_TIMER:
        if (wParam == AUTO_TIMER_ID)
        {
            AUTO_OnTimer(hWnd);
            if (!gAvisLoaded) return 0;

            gAvisLoaded = FALSE;
            SetWindowTextA(hEdit, gAvisBuffer);

            if (!AvisHasCoord(gAvisBuffer))
            {
                KillTimer(hWnd, AUTO_TIMER_ID);
                SetWindowTextA(hBtnAuto, "AUTO-ON");
                autoRunning = FALSE;

                AVIS_ERR("AUTO", "Validation Failed: No COORD in AVIS buffer. Stopping Timer.");
                MessageBoxA(hWnd, "No AVIS COORD found.\nAUTO disabled.", "AVIS ERROR", MB_ICONWARNING);
                return 0;
            }

            if (SendMessage(hAutoSaveChk, BM_GETCHECK, 0, 0) == BST_CHECKED)
            {
                AVIS_INFO("AUTO", "Triggering Background Sync...");
                FIO_SaveAVIS();
            }

            RefreshProjectTree(hWnd);
            return 0;
        }
        break;

    case WM_CREATE:
    {
        HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINMENU));
        SetMenu(hWnd, hMenu);

        // Standard Tree & Edit
        hTree = CreateWindowExA(WS_EX_CLIENTEDGE, WC_TREEVIEWA, "", 
            WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
            10, 10, 300, 500, hWnd, (HMENU)IDC_TREEVIEW, GetModuleHandle(NULL), NULL);

        hEdit = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", 
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
            320, 10, 650, 500, hWnd, (HMENU)IDC_EDIT, GetModuleHandle(NULL), NULL);

        // Terminal UI Control
        hTerminal = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", 
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
            10, 520, 960, 120, hWnd, (HMENU)IDC_TERMINAL, GetModuleHandle(NULL), NULL);

        // Hook the logging engine to this window
        AVIS_SetTerminalWindow(hTerminal);
        AVIS_INFO("CORE", "Terminal System Link: Established.");

        hAutoSaveChk = CreateWindowA("BUTTON", "Auto Save", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            320, 650, 100, 20, hWnd, (HMENU)IDC_AUTOSAVE, GetModuleHandle(NULL), NULL);

        BTN_CreateAuto(hWnd);

        // Path Discovery
        GetModuleFileNameA(NULL, gAppPath, MAX_PATH);
        char *pp = strrchr(gAppPath, '\\');
        if (pp) *pp = 0;

        AVIS_MSG("CORE", "Host Environment: %s", gAppPath);
        lstrcpyA(gProjectDir, gAppPath);
        
        ScanProjectDir(gProjectDir);
        MERCG_SetRoot(gAppPath);
        MERCG_LoadDefaultTemplate();

        AVIS_INFO("CORE", "Project Scanner: Active.");
        return 0;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_AUTO:
            AVIS_INFO("UI", "Toggle Auto-Scanner.");
            BTN_ProcAuto(hWnd);
            return 0;

        case ID_FILE_OPEN_DIR:
            AVIS_INFO("UI", "Requesting Project Directory change.");
            DoChooseProjectDir(hWnd);
            return 0;

        case ID_FILE_OPEN:
            DoOpenFileFromDialog(hWnd);
            return 0;

        case ID_FILE_SAVE:
            AVIS_INFO("UI", "Manual Save triggered.");
            DoSave(hWnd);
            return 0;

        case ID_FILE_EXIT:
            AVIS_INFO("CORE", "Application Exit requested.");
            PostMessage(hWnd, WM_CLOSE, 0, 0);
            return 0;

        case ID_OPTIONS_AUTOSAVE:
        case IDC_AUTOSAVE:
        {
            BOOL checked = (SendMessage(hAutoSaveChk, BM_GETCHECK, 0, 0) == BST_CHECKED);
            if (LOWORD(wParam) == ID_OPTIONS_AUTOSAVE) checked = !checked;

            SendMessage(hAutoSaveChk, BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED, 0);
            CheckMenuItem(GetMenu(hWnd), ID_OPTIONS_AUTOSAVE, MF_BYCOMMAND | (checked ? MF_CHECKED : MF_UNCHECKED));
            
            AVIS_INFO("OPTS", "AutoSave state changed to: %s", checked ? "ENABLED" : "DISABLED");
            return 0;
        }

        case ID_TOOLS_MERCG:
            AVIS_MSG("MERCG", "Opening MERC-G Inspector...");
            MERCG_ShowPopup(hWnd);
            return 0;
        }
        break;

    case WM_NOTIFY:
        if (((LPNMHDR)lParam)->hwndFrom == hTree && ((LPNMHDR)lParam)->code == NM_DBLCLK)
        {
            AVIS_INFO("UI", "Project Tree: Item Activated.");
            OnTreeDblClick(hWnd);
            return 0;
        }
        break;

    case WM_DESTROY:
        AVIS_INFO("CORE", "Shutting down AVIS Terminal.");
        AVIS_LogSave("cyhy-log.txt");
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}
