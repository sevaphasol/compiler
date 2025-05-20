section .text
global _start

_start:
        call main
        mov rax, 60
        syscall
main:
        push qword rbp
        mov rbp, rsp
        sub rsp, 40
        mov [rbp - 8], 1000
        mov [rbp - 16], 5
        mov [rbp - 24], 1
        mov [rbp - 32], 1
        jmp .L0
.L1:
        jmp .L2
.L3:
        push qword [rbp - 32]
        pop qword [rbp - 40]
        push qword [rbp - 24]
        mov r11, [rbp - 32]
        pop qword r10
        add r10, r11
        mov [rbp - 32], r10
        push qword [rbp - 40]
        pop qword [rbp - 24]
        push qword [rbp - 16]
        mov r11, 1
        pop qword r10
        sub r10, r11
        mov [rbp - 16], r10
.L2:
        mov rax, [rbp - 16]
        test rax, rax
        jne .L3
        push qword [rbp - 8]
        mov r11, 1
        pop qword r10
        sub r10, r11
        mov [rbp - 8], r10
.L0:
        mov rax, [rbp - 8]
        test rax, rax
        jne .L1
        mov rax, 0
        add rsp, 40
        pop qword rbp
        ret
