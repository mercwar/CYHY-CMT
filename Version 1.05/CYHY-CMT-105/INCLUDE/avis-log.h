#ifndef AVIS_LOG_H
#define AVIS_LOG_H

#include <windows.h>

//typedef enum { LOG_INFO = 0, LOG_WARN, LOG_ERROR, LOG_AVIS } AVIS_LEVEL;
typedef enum {
    LOG_INFO,
    LOG_ERROR,
   LOG_WARN,
    LOG_AVIS
} AVIS_LEVEL;

// Use parentheses around function names to prevent macro expansion during declaration
void (AVIS_LogWrite)(AVIS_LEVEL level, const char* module, const char* fmt, ...);
void AVIS_SetTerminalWindow(HWND hWnd);
void AVIS_LogSave(const char *path);

// Modern Tagged Macros
#define AVIS_INFO(mod, fmt, ...)  AVIS_LogWrite(LOG_INFO,  mod, fmt, ##__VA_ARGS__)
#define AVIS_WARN(mod, fmt, ...)  AVIS_LogWrite(LOG_WARN,  mod, fmt, ##__VA_ARGS__)
#define AVIS_ERR(mod, fmt, ...)   AVIS_LogWrite(LOG_ERROR, mod, fmt, ##__VA_ARGS__)
#define AVIS_MSG(mod, fmt, ...)   AVIS_LogWrite(LOG_AVIS,  mod, fmt, ##__VA_ARGS__)

// Legacy Support for 1-argument calls (Fixes auto_timer.c errors 1053/2070)
#define AVIS_Log(fmt, ...) AVIS_LogWrite(LOG_INFO, "GEN", fmt, ##__VA_ARGS__)
#define AVIS_ERROR(tag, fmt, ...) AVIS_LogWrite(LOG_ERROR, tag, fmt, ##__VA_ARGS__)

#endif
