/*
AVIS:
    artifact: avis_reg.c
    role: registry-class-implementation
    family: cyhy-cmt
    version: 1.1 (Terminal-Logged)
*/

#include "avis_reg.h"
#include "avis-log.h"  // Essential for AVIS_INFO/ERR macros

/**
 * Sets a string value in the Windows Registry under HKEY_CURRENT_USER.
 * Logs success/failure to the AVIS Terminal.
 */
bool AVIS_REG_Set(const char *subkey, const char *name, const char *value) {
    bool ok = AVIS_RegWriteString(HKEY_CURRENT_USER, subkey, name, value);
    
    if (ok) {
        AVIS_INFO("REG", "Registry Write Success: [%s] = %s", name, value);
    } else {
        AVIS_ERR("REG", "Registry Write FAILURE: [%s] (Check Permissions)", name);
    }
    
    return ok;
}

/**
 * Retrieves a string value from the Windows Registry.
 * Logs the operation to the AVIS Terminal for configuration tracking.
 */
bool AVIS_REG_Get(const char *subkey, const char *name, char *out, DWORD outSize) {
    bool ok = AVIS_RegReadString(HKEY_CURRENT_USER, subkey, name, out, outSize);
    
    if (ok) {
        AVIS_INFO("REG", "Registry Read: [%s] -> %s", name, out);
    } else {
        // Logged as WARN because missing keys are common during initial setup
        AVIS_WARN("REG", "Registry: Key [%s] not found in %s", name, subkey);
    }
    
    return ok;
}
