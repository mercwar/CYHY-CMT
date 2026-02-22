/*
AVIS:
    artifact: auto_timer.h
    role: autoscan-timer-api
    family: cyhy-cmt
    version: 1.1
*/

#ifndef AUTO_TIMER_H
#define AUTO_TIMER_H

#include <windows.h>

#define AUTO_TIMER_ID 9001

void AUTO_Start(HWND hWnd);
void AUTO_Stop(HWND hWnd);
void AUTO_OnTimer(HWND hWnd);

#endif
