#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "avis-log.h"

#define ID_LOAD_AVIS   1001
#define ID_SHOW_MERCG  1002
#define ID_SAVE_LOG    1003

extern BOOL MERCG_LoadAVIS(const char *path);
extern void MERCG_ShowPopup(HWND parent);

LRESULT CALLBACK CyhyWndProc(HWND, UINT, WPARAM, LPARAM);

static const char CYHY_CLASS[] = "CYHY_Main";

HMENU CreateCyhyMenu(void) {
    HMENU hMenu = CreateMenu();
    HMENU hFile = CreatePopupMenu();

    AppendMenuA(hFile, MF_STRING, ID_LOAD_AVIS,  "Load AVIS...");
    AppendMenuA(hFile, MF_STRING, ID_SHOW_MERCG, "Show MERC-G");
    AppendMenuA(hFile, MF_STRING, ID_SAVE_LOG,   "Save AVIS Log");
    AppendMenuA(hFile, MF_SEPARATOR, 0, NULL);
    AppendMenuA(hFile, MF_STRING, IDOK, "Exit");

    AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hFile, "File");
    return hMenu;
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE prev, LPSTR cmd, int show) {
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = CyhyWndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = CYHY_CLASS;
    RegisterClassA(&wc);

    HWND hWnd = CreateWindowExA(
        0, CYHY_CLASS, "CYHY Control Window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInst, NULL
    );

    if (!hWnd) return 0;

    ShowWindow(hWnd, show);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK CyhyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        SetMenu(hWnd, CreateCyhyMenu());
        AVIS_Log("CYHY window created.");
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_LOAD_AVIS:
            if (MERCG_LoadAVIS("template.avis"))
                MessageBoxA(hWnd, "AVIS loaded.", "MERC-G", MB_OK);
            else
                MessageBoxA(hWnd, "Failed to load template.avis", "MERC-G", MB_OK | MB_ICONERROR);
            return 0;

        case ID_SHOW_MERCG:
            MERCG_ShowPopup(hWnd);
            return 0;

        case ID_SAVE_LOG:
            AVIS_LogSave("avis.log");
            MessageBoxA(hWnd, "Saved to avis.log", "AVIS Log", MB_OK);
            return 0;

        case IDOK:
            PostMessage(hWnd, WM_CLOSE, 0, 0);
            return 0;
        }
        break;

    case WM_DESTROY:
        AVIS_Log("CYHY shutting down.");
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}
