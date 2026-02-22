/*
AVIS:
    artifact: cyhy.h
    role: main-editor
    family: cyhy-cmt
    version: 6.2
*/

#ifndef CYHY_MAIN_HEADER
#define CYHY_MAIN_HEADER

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <shlobj.h>

#include "common.h"
#include "resource.h"

#include "avis-log.h"
#include "cyhy-editor.h"
#include "buttons_create.h"
#include "buttons_proc.h"
#include "auto_timer.h"
#include "template_loader.h"
#include "mercg.h"

/* class names */
extern const char CYHY_CLASS[];
extern const char MERCG_CLASS[];

/* main controls */
extern HWND hTree;
extern HWND hEdit;
extern HWND hAutoSaveChk;

/* project paths */
extern char gProjectDir[MAX_PATH];
extern char gCurrentFile[MAX_PATH];

/* CYHY file list type */
//typedef struct {
    //char path[MAX_PATH];
//} CYHY_FILE;

#define MAX_CYHY_FILES 1024
extern CYHY_FILE gFileList[MAX_CYHY_FILES];
extern int gFileCount;

/* MERCG root */
extern char gMercgRoot[MAX_PATH];
void EnsureFactoryExists(const char* root);
/* window proc */
LRESULT CALLBACK CyhyWndProc(HWND, UINT, WPARAM, LPARAM);
/* Global Prototype */
extern void MERCG_Init(HINSTANCE hInst);
extern HWND hMercgMainWnd; // Allow cyhy.c to check if it's null

#endif /* CYHY_MAIN_HEADER */
