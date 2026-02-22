/*
AVIS:
    artifact: cyhy-editor.h
    role: editor-engine-api
    family: cyhy-cmt
    version: 1.2
*/

#ifndef CYHY_EDITOR_H
#define CYHY_EDITOR_H

#include <windows.h>

// Project Management
void NormalizeDir(char *dir);
void ScanProjectDir(const char *dir);
void RefreshProjectTree(HWND hWnd);

// File Operations
// Change this line
BOOL MERCG_LoadFile(const char *path);

void SaveEditToFile(const char *path);
void MaybeAutoSave(void);

// UI Dialogs
void DoChooseProjectDir(HWND hWnd);
void DoOpenFileFromDialog(HWND hWnd);
void DoSave(HWND hWnd);
void OnTreeDblClick(HWND hWnd);

#endif
