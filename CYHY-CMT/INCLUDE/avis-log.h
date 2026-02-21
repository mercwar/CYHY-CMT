#ifndef AVIS_LOG_H
#define AVIS_LOG_H

#include <windows.h>

// 1. Define the type FIRST so the functions can see it
typedef enum {
    LOG_INFO = 0,
    LOG_WARN,
    LOG_ERROR,
    LOG_AVIS
} AVIS_LEVEL;

// 2. Function prototypes
void AVIS_LogWrite(AVIS_LEVEL level, const char* module, const char* fmt, ...);
void AVIS_LogSave(const char *path);
void AVIS_SetTerminalWindow(HWND hWnd);

// 3. Macros for the C files to use
#define AVIS_INFO(mod, fmt, ...)  AVIS_LogWrite(LOG_INFO,  mod, fmt, ##__VA_ARGS__)
#define AVIS_WARN(mod, fmt, ...)  AVIS_LogWrite(LOG_WARN,  mod, fmt, ##__VA_ARGS__)
#define AVIS_ERR(mod, fmt, ...)   AVIS_LogWrite(LOG_ERROR, mod, fmt, ##__VA_ARGS__)
#define AVIS_MSG(mod, fmt, ...)   AVIS_LogWrite(LOG_AVIS,  mod, fmt, ##__VA_ARGS__)

// Legacy support
void AVIS_Log(const char *msg);
void AVIS_LogFmt(const char *fmt, ...);

#endif
