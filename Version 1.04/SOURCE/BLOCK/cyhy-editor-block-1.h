/*
AVIS:
    artifact: cyhy-editor-block-1.h
    role: editor-api-block
    family: cyhy-cmt
    version: 1.0
*/

#ifndef CYHY_EDITOR_BLOCK_1_H
#define CYHY_EDITOR_BLOCK_1_H

#include <windows.h>

/* ---------------------------------------------------------
   EDITOR ENGINE PUBLIC API
   (functions implemented inside cyhy-editor.c)
--------------------------------------------------------- */

void NormalizeDir(char *dir);
void ScanProjectDir(const char *dir);

void LoadFileToEdit(const char *path);
void SaveEditToFile(const char *path);
void MaybeAutoSave(void);

void DoChooseProjectDir(HWND hWnd);
void DoOpenFileFromDialog(HWND hWnd);
void DoSave(HWND hWnd);
void OnTreeDblClick(HWND hWnd);

#endif
