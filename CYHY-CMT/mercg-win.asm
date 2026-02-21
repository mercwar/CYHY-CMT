.intel_syntax noprefix

.equ WS_POPUP,   0x80000000
.equ WS_VISIBLE, 0x10000000

.data
MERCG_ClassName: .asciz "MERCG_Popup"
MERCG_Title:     .asciz "Merc-G AVIS Config"

.bss
hMERCG: .quad 0

.text
.extern GetModuleHandleA
.extern RegisterClassExA
.extern CreateWindowExA
.extern DefWindowProcA
.extern ShowWindow
.extern UpdateWindow

.global MERCG_ShowPopup
MERCG_ShowPopup:
    sub rsp,32
    xor rcx,rcx
    call GetModuleHandleA
    mov rbx,rax

    sub rsp,80
    mov qword ptr [rsp+0],80
    mov qword ptr [rsp+8],0

    lea rax,[rip+MERCG_WndProc]
    mov qword ptr [rsp+16],rax

    mov qword ptr [rsp+24],0
    mov qword ptr [rsp+32],0
    mov qword ptr [rsp+40],rbx
    mov qword ptr [rsp+48],0
    mov qword ptr [rsp+56],0
    mov qword ptr [rsp+64],0

    lea rax,[rip+MERCG_ClassName]
    mov qword ptr [rsp+72],rax

    lea rcx,[rsp]
    call RegisterClassExA
    add rsp,80

    mov rcx,0
    lea rdx,[rip+MERCG_ClassName]
    lea r8,[rip+MERCG_Title]

    mov r9, WS_POPUP + WS_VISIBLE

    sub rsp,32
    mov qword ptr [rsp+0],200
    mov qword ptr [rsp+8],200
    mov qword ptr [rsp+16],400
    mov qword ptr [rsp+24],200

    push 0
    push 0
    push rbx
    push 0

    call CreateWindowExA
    add rsp,32+32

    mov [hMERCG],rax

    mov rcx,rax
    mov rdx,1
    call ShowWindow

    mov rcx,rax
    call UpdateWindow

    add rsp,32
    ret

.global MERCG_WndProc
MERCG_WndProc:
    jmp DefWindowProcA
