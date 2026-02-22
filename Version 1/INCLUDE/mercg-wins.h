#ifndef MERCG_WINS_H
#define MERCG_WINS_H

#include <windows.h>
#include <commctrl.h>

// Timer ID for the background window scan
#define IDT_WND_REFRESH_TIMER 5001


// Function declarations
void RefreshWindowTree(HWND hTree);
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
// Callback for the EnumWindows API
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
void AVIS_ToggleTopMost(HWND hWnd);
#endif
