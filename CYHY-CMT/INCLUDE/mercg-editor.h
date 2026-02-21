/*
AVIS:
    artifact: merg-editor.h
    role: merg-editor-header
    family: cyhy-cmt
    version: 1.0
*/

#ifndef MERG_EDITOR_H
#define MERG_EDITOR_H

void MERCG_SetRoot(const char *root);
void MERCG_LoadDefaultTemplate(void);
// Change this line
BOOL MERCG_LoadFile(const char *path);

void MERCG_SaveFile(const char *path);
void MERCG_SaveCurrent(void);
void CleanAvisLabel(char *s);
#endif
