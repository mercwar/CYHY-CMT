.intel_syntax noprefix

.data
MERC_AVIS_Buffer: .space 4096
MERC_AVIS_LoadedFlag: .byte 0

.text
.extern CreateFileA
.extern ReadFile
.extern CloseHandle

.global MERCG_LoadAVIS
MERCG_LoadAVIS:
    mov rdx, 0x80000000
    xor r8, r8
    xor r9, r9

    sub rsp, 32
    mov qword ptr [rsp+0], 3
    mov qword ptr [rsp+8], 0
    mov qword ptr [rsp+16], 0
    call CreateFileA
    add rsp, 32

    cmp rax, -1
    je load_fail

    mov rbx, rax

    sub rsp, 32
    mov rcx, rbx
    lea rdx, [rip+MERC_AVIS_Buffer]
    mov r8, 4096
    lea r9, [rsp+0]
    mov qword ptr [rsp+16], 0
    call ReadFile
    add rsp, 32

    mov rcx, rbx
    call CloseHandle

    mov byte ptr [MERC_AVIS_LoadedFlag], 1
    ret

load_fail:
    mov byte ptr [MERC_AVIS_LoadedFlag], 0
    ret
