/*
AVIS:
    artifact: mercg.h
    role: mercg-interface
    family: cyhy-cmt
    version: 2.0
*/

#ifndef MERCG_H
#define MERCG_H

#include <windows.h>
#include "resource.h"

void MERCG_ShowPopup(HWND parent);
void MERCG_SetRoot(const char *root);
void MERCG_LoadDefaultTemplate(void);
static int CALLBACK MercgBrowseCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) ;
void MERCG_SaveCurrent(void);
extern char gMercgCurrentFile[MAX_PATH];
extern HWND hMercgEdit;
void MERCG_Init(HINSTANCE hInst);

BOOL MERCG_LoadAVIS(const char* filename);

#endif
