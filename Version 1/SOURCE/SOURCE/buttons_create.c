/*
AVIS:
    artifact: buttons_create.c
    role: ui-button-create-block
    family: cyhy-cmt
    version: 1.0
*/

#include <windows.h>
#include "resource.h"
#include "buttons_create.h"


extern HWND hBtnAuto;

void BTN_CreateAuto(HWND hWndParent)
{
    hBtnAuto = CreateWindowA(
        "BUTTON",
        "AUTO-ON",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       450, 650, 80, 22,
        hWndParent,
        (HMENU)IDC_BTN_AUTO,
        (HINSTANCE)GetModuleHandle(NULL),
        NULL
    );
}
