.intel_syntax noprefix

    .data
AVIS_Buffer: .space 2048
AVIS_LoadedFlag: .byte 0

    .text
    .extern CreateFileA
    .extern ReadFile
    .extern CloseHandle

.global MERCG_LoadAVIS
MERCG_LoadAVIS:
    ; rcx = filename
    ret
