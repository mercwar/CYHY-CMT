/*
AVIS:
    artifact: avis_reg.h
    role: registry-class-header
    family: cyhy-cmt
    version: 1.0
*/

#ifndef AVIS_REG_H
#define AVIS_REG_H

#include <windows.h>
#include <stdbool.h>
#include "avis_core.h"

bool AVIS_REG_Set(const char *subkey, const char *name, const char *value);
bool AVIS_REG_Get(const char *subkey, const char *name, char *out, DWORD outSize);

#endif
