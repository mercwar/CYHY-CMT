/* 
AVIS:
    artifact: fire-gem.h
    role: kernel-include
    family: fire-gem
    version: 1.0
    description: Fire‑Gem include-mode kernel header for cyhy-cmt
*/

#ifndef FIRE_GEM_H
#define FIRE_GEM_H

// Drop‑in module type
typedef void (*FG_ModuleEntry)(void);

// Register a drop‑in module
void FireGem_Register(FG_ModuleEntry entry);

// Start the Fire‑Gem kernel
void FireGem_Start(void);

// AVIS Logging API
void AVIS_Log(const char *msg);
void AVIS_LogFmt(const char *fmt, ...);
void AVIS_LogSave(const char *path);

#endif
