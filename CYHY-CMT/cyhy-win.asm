#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#define ID_MENU_LOAD_AVIS  1001
#define ID_MENU_SHOW_MERCG 1002

static const char CYHY_CLASS[]  = "CYHY_Main";
static const char MERCG_CLASS[] = "MERCG_Popup";

static char  gAvisBuffer[4096];
static BOOL  gAvisLoaded = FALSE;

LRESULT CALLBACK CyhyWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MercgWndProc(HWND, UINT, WPARAM, LPARAM);

BOOL LoadAvisFile(const char *path) {
    HANDLE hFile = CreateFileA(
        path,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE) {
        gAvisLoaded = FALSE;
        return FALSE;
    }

    DWORD bytesRead = 0;
    BOOL ok = ReadFile(hFile, gAvisBuffer, sizeof(gAvisBuffer), &bytesRead, NULL);
    CloseHandle(hFile);

    if (!ok || bytesRead == 0) {
        gAvisLoaded = FALSE;
        return FALSE;
    }

    gAvisLoaded = TRUE;
    return TRUE;
}

HWND CreateMercgPopup(HWND hParent, HINSTANCE hInst) {
    return CreateWindowExA(
        0,
        MERCG_CLASS,
        "Merc-G AVIS Config",
        WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        400, 200,
        hParent,
        NULL,
        hInst,
        NULL
    );
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASSA wc = {0};

    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = CyhyWndProc;
    wc.hInstance     = hInst;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CYHY_CLASS;
    RegisterClassA(&wc);

    wc.lpfnWndProc   = MercgWndProc;
    wc.lpszClassName = MERCG_CLASS;
    RegisterClassA(&wc);

    HWND hWnd = CreateWindowExA(
        0,
        CYHY_CLASS,
        "CYHY Control Window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        hInst,
        NULL
    );

    if (!hWnd) return 0;

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

HMENU CreateCyhyMenu(void) {
    HMENU hMenu = CreateMenu();
    HMENU hFile = CreatePopupMenu();

    AppendMenuA(hFile, MF_STRING, ID_MENU_LOAD_AVIS,  "Load AVIS...");
    AppendMenuA(hFile, MF_STRING, ID_MENU_SHOW_MERCG, "Show MERC-G");
    AppendMenuA(hFile, MF_SEPARATOR, 0, NULL);
    AppendMenuA(hFile, MF_STRING, IDOK, "Exit");

    AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hFile, "File");
    return hMenu;
}

LRESULT CALLBACK CyhyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HINSTANCE sInst = NULL;

    switch (msg) {
    case WM_CREATE:
        sInst = ((LPCREATESTRUCT)lParam)->hInstance;
        SetMenu(hWnd, CreateCyhyMenu());
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_MENU_LOAD_AVIS:
            if (LoadAvisFile("template.avis")) {
                MessageBoxA(hWnd, "AVIS loaded into buffer.", "MERC-G", MB_OK | MB_ICONINFORMATION);
            } else {
                MessageBoxA(hWnd, "Failed to load template.avis", "MERC-G", MB_OK | MB_ICONERROR);
            }
            return 0;

        case ID_MENU_SHOW_MERCG:
            CreateMercgPopup(hWnd, sInst);
            return 0;

        case IDOK:
            PostMessage(hWnd, WM_CLOSE, 0, 0);
            return 0;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK MercgWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        const char *text = gAvisLoaded
            ? "AVIS is loaded into buffer."
            : "No AVIS loaded.";
        TextOutA(hdc, 10, 10, text, (int)lstrlenA(text));
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN:
        DestroyWindow(hWnd);
        return 0;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}
