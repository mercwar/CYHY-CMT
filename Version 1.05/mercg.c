#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "avis-log.h"

extern char gAvisBuffer[4096];
extern BOOL gAvisLoaded;

static const char MERCG_CLASS[] = "MERCG_Popup";

LRESULT CALLBACK MercgWndProc(HWND, UINT, WPARAM, LPARAM);

void MERCG_ShowPopup(HWND parent) {
    AVIS_Log("Opening MERC-G popup window.");

    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE);

    WNDCLASSA wc = {0};
    wc.lpfnWndProc = MercgWndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = MERCG_CLASS;
    RegisterClassA(&wc);

    CreateWindowExA(
        0, MERCG_CLASS, "MERC-G AVIS Config",
        WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        400, 200,
        parent, NULL, hInst, NULL
    );
}

LRESULT CALLBACK MercgWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        const char *text = gAvisLoaded
            ? "AVIS loaded into buffer."
            : "No AVIS loaded.";

        TextOutA(hdc, 10, 10, text, lstrlenA(text));
        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_CLOSE:
        AVIS_Log("MERC-G popup closed.");
        DestroyWindow(hWnd);
        return 0;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}
