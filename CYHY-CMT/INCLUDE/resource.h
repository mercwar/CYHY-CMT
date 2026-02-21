/*
AVIS:
    artifact: resource.h
    role: resource-identifiers
    family: cyhy-cmt
    version: 1.2
*/

#ifndef RESOURCE_H
#define RESOURCE_H

// Main menu
#define IDR_MAINMENU        101

// File menu commands
#define ID_FILE_OPEN_DIR    40001
#define ID_FILE_OPEN        40002
#define ID_FILE_SAVE        40003
#define ID_FILE_EXIT        40004
#define IDC_TERMINAL        50004 
// Options menu
#define ID_OPTIONS_AUTOSAVE 41001   // <-- KEEP THIS

// Controls
#define IDC_TREEVIEW        50001
#define IDC_EDIT            50002
#define IDC_AUTOSAVE        50003

// Tools menu
#define ID_TOOLS_MERCG      40050

// Buttons
#define IDC_BTN_AUTO        41002   // <-- FIXED (NEW UNIQUE ID)
#define AUTO_TIMER_ID 9001

#define ID_OPEN_AVIS   2001
#define ID_SAVE_AVIS   2002
#define ID_SCAN_DIR    2003
#define ID_CHOOSE_DIR  2004


// ---------------------------------------------------------
// MERCG BUTTON IDS
// ---------------------------------------------------------
#define ID_MERCG_CHOOSE_DIR   5000
#define ID_MERCG_OPEN_AVIS    5001
#define ID_MERCG_SAVE_AVIS    5002
#define ID_MERCG_SCAN_DIR     5003

#endif
