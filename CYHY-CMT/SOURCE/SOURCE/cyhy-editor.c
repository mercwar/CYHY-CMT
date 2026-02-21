/*
AVIS:
    artifact: cyhy-editor.c
    role: editor-engine
    family: cyhy-cmt
    version: 1.3 (Terminal-Logged)
*/

#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shlobj.h>
#include <stdio.h>

#include "cyhy-editor.h"  
#include "avis-log.h"
#include "resource.h"
#include "cyhy.h"
#include "common.h"

/* ---------------------------------------------------------
   External globals from cyhy.c
--------------------------------------------------------- */
extern HWND hTree;
extern HWND hEdit;
extern BOOL gAutoSave;
extern char gProjectDir[MAX_PATH];
extern char gCurrentFile[MAX_PATH];

extern int gFileCount;
extern struct { char path[MAX_PATH]; } gFileList[];

/* ---------------------------------------------------------
   Helpers
--------------------------------------------------------- */
void NormalizeDir(char *dir)
{
    size_t len = lstrlenA(dir);
    if (len == 0) return;
    if (dir[len - 1] == '\\' || dir[len - 1] == '/')
        dir[len - 1] = 0;
}

void ScanProjectDir(const char *dir)
{
    AVIS_INFO("SCAN", "Opening project directory: %s", dir);
    
    char pattern[MAX_PATH];
    WIN32_FIND_DATAA fd;
    HANDLE hFind;

    gFileCount = 0;
    TreeView_DeleteAllItems(hTree);

    wsprintfA(pattern, "%s\\*.*", dir);

    hFind = FindFirstFileA(pattern, &fd);
    if (hFind == INVALID_HANDLE_VALUE) {
        AVIS_ERR("SCAN", "Failed to access directory: %s", dir);
        return;
    }

    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            if (gFileCount >= MAX_CYHY_FILES) {
                AVIS_WARN("SCAN", "Maximum file limit reached (%d)", MAX_CYHY_FILES);
                break;
            }

            wsprintfA(gFileList[gFileCount].path, "%s\\%s", dir, fd.cFileName);

            TVINSERTSTRUCTA ins;
            ZeroMemory(&ins, sizeof(ins));
            ins.hParent = NULL;
            ins.hInsertAfter = TVI_LAST;
            ins.item.mask = TVIF_TEXT | TVIF_PARAM;
            ins.item.pszText = fd.cFileName;
            ins.item.lParam = (LPARAM)gFileCount;

            TreeView_InsertItem(hTree, &ins);
            gFileCount++;
        }
    } while (FindNextFileA(hFind, &fd));

    FindClose(hFind);
    AVIS_MSG("SCAN", "Crawl complete. Indexed %d workspace files.", gFileCount);
}

/* ---------------------------------------------------------
   File I/O
--------------------------------------------------------- */
void LoadFileToEdit(const char *path)
{
    AVIS_INFO("EDIT", "Loading artifact: %s", path);

    HANDLE hFile = CreateFileA(
        path, GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );
    
    if (hFile == INVALID_HANDLE_VALUE) {
        AVIS_ERR("EDIT", "Access Denied: Could not open %s", path);
        return;
    }

    DWORD bytesRead = 0;
    char buf[4096];

    if (ReadFile(hFile, buf, sizeof(buf) - 1, &bytesRead, NULL))
    {
        buf[bytesRead] = 0;
        SetWindowTextA(hEdit, buf);
        AVIS_MSG("EDIT", "Buffer updated. Bytes read: %lu", bytesRead);
    } else {
        AVIS_ERR("EDIT", "Read error on file: %s", path);
    }

    CloseHandle(hFile);
}

void SaveEditToFile(const char *path)
{
    AVIS_INFO("EDIT", "Attempting save to: %s", path);

    HANDLE hFile = CreateFileA(
        path, GENERIC_WRITE, 0,
        NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
    );
    
    if (hFile == INVALID_HANDLE_VALUE) {
        AVIS_ERR("EDIT", "Save FAILED: Check folder permissions for %s", path);
        return;
    }

    char buf[4096];
    GetWindowTextA(hEdit, buf, sizeof(buf));

    DWORD len = (DWORD)lstrlenA(buf);
    DWORD written = 0;
    
    if (WriteFile(hFile, buf, len, &written, NULL)) {
        AVIS_MSG("EDIT", "Successfully wrote %lu bytes to disk.", written);
    } else {
        AVIS_ERR("EDIT", "Write operation failed.");
    }
    
    CloseHandle(hFile);
}

void MaybeAutoSave(void)
{
    if (!gAutoSave || !gCurrentFile[0])
        return;

    AVIS_INFO("AUTO", "Triggering Auto-Save pulse.");
    SaveEditToFile(gCurrentFile);
}

void LoadFileListIntoTree(HWND hTree)
{
    TVINSERTSTRUCTA tvi;
    HTREEITEM hRoot;

    TreeView_DeleteAllItems(hTree);

    ZeroMemory(&tvi, sizeof(tvi));
    tvi.hParent = TVI_ROOT;
    tvi.hInsertAfter = TVI_LAST;
    tvi.item.mask = TVIF_TEXT;
    tvi.item.pszText = "Project Files";

    hRoot = TreeView_InsertItem(hTree, &tvi);

    for (int i = 0; i < gFileCount; i++)
    {
        ZeroMemory(&tvi, sizeof(tvi));
        tvi.hParent = hRoot;
        tvi.hInsertAfter = TVI_LAST;
        tvi.item.mask = TVIF_TEXT | TVIF_PARAM;
        tvi.item.pszText = gFileList[i].path;
        tvi.item.lParam = (LPARAM)i;

        TreeView_InsertItem(hTree, &tvi);
    }

    TreeView_Expand(hTree, hRoot, TVE_EXPAND);
}

/* ---------------------------------------------------------
   UI Actions
--------------------------------------------------------- */
void RefreshProjectTree(HWND hWnd)
{
    AVIS_INFO("UI", "Refreshing workspace tree view.");
    LoadFileListIntoTree(hTree);
}

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if (uMsg == BFFM_INITIALIZED && lpData)
    {
        SendMessage(hwnd, BFFM_SETSELECTIONA, TRUE, lpData);
    }
    return 0;
}

void DoChooseProjectDir(HWND hWnd)
{
    BROWSEINFOA bi;
    ZeroMemory(&bi, sizeof(bi));

    char displayName[MAX_PATH] = "";
    bi.hwndOwner = hWnd;
    bi.pszDisplayName = displayName;
    bi.lpszTitle = "Select Project Directory";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.lpfn = BrowseCallbackProc;
    bi.lParam = (LPARAM)gProjectDir;

    LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
    if (pidl)
    {
        char path[MAX_PATH];
        if (SHGetPathFromIDListA(pidl, path))
        {
            lstrcpyA(gProjectDir, path);
            NormalizeDir(gProjectDir);
            AVIS_INFO("CORE", "Project Root re-targeted: %s", gProjectDir);
            ScanProjectDir(gProjectDir);
            SetWindowTextA(hEdit, "");
            gCurrentFile[0] = 0;
        }

        LPMALLOC pMalloc;
        if (SUCCEEDED(SHGetMalloc(&pMalloc)))
        {
            pMalloc->lpVtbl->Free(pMalloc, pidl);
            pMalloc->lpVtbl->Release(pMalloc);
        }
    }
}

void DoOpenFileFromDialog(HWND hWnd)
{
    char file[MAX_PATH] = "";
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = "All Files\0*.*\0";
    ofn.lpstrFile = file;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = "Open File";
    ofn.lpstrInitialDir = gProjectDir[0] ? gProjectDir : NULL;

    if (GetOpenFileNameA(&ofn))
    {
        lstrcpyA(gCurrentFile, file);

        lstrcpyA(gProjectDir, file);
        char *p = strrchr(gProjectDir, '\\');
        if (p) *p = 0;

        AVIS_MSG("UI", "Manual file selection: %s", gCurrentFile);
        ScanProjectDir(gProjectDir);
        LoadFileToEdit(gCurrentFile);
    }
}

void DoSave(HWND hWnd)
{
    if (!gCurrentFile[0])
    {
        char file[MAX_PATH] = "new.txt";
        OPENFILENAMEA ofn;
        ZeroMemory(&ofn, sizeof(ofn));

        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hWnd;
        ofn.lpstrFilter = "All Files\0*.*\0";
        ofn.lpstrFile = file;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_OVERWRITEPROMPT;
        ofn.lpstrTitle = "Save File As";
        ofn.lpstrInitialDir = gProjectDir[0] ? gProjectDir : NULL;

        if (!GetSaveFileNameA(&ofn))
            return;

        lstrcpyA(gCurrentFile, file);
    }

    SaveEditToFile(gCurrentFile);
}

void OnTreeDblClick(HWND hWnd)
{
    TVHITTESTINFO ht;
    POINT pt;

    GetCursorPos(&pt);
    ScreenToClient(hTree, &pt);
    ht.pt = pt;

    HTREEITEM hItem = TreeView_HitTest(hTree, &ht);
    if (!hItem)
        return;

    TVITEMA tvi;
    ZeroMemory(&tvi, sizeof(tvi));
    tvi.mask = TVIF_PARAM;
    tvi.hItem = hItem;

    if (!TreeView_GetItem(hTree, &tvi))
        return;

    int idx = (int)tvi.lParam;
    if (idx < 0 || idx >= gFileCount)
        return;

    lstrcpyA(gCurrentFile, gFileList[idx].path);
    AVIS_INFO("UI", "Workspace focus change via tree navigation.");
    LoadFileToEdit(gCurrentFile);
}
