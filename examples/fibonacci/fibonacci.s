section .text
global _start

_start:
        call main
        mov rax, 60
        syscall
main:
        push qword rbp
        mov rbp, rsp
        sub rsp, 32
        mov [rbp - 8], 1
        mov [rbp - 16], 1
        mov [rbp - 24], 7
        mov rax, [rbp - 24]
        test rax, rax
        je .L0
        push qword [rbp - 8]
        call print 
        add rsp, 8
        push qword [rbp - 24]
        mov r11, 1
        pop qword r10
        sub r10, r11
        mov [rbp - 24], r10
        mov rax, [rbp - 24]
        test rax, rax
        je .L1
        push qword [rbp - 16]
        call print 
        add rsp, 8
        push qword [rbp - 24]
        mov r11, 1
        pop qword r10
        sub r10, r11
        mov [rbp - 24], r10
        jmp .L2
.L3:
        push qword [rbp - 16]
        pop qword [rbp - 32]
        push qword [rbp - 8]
        mov r11, [rbp - 16]
        pop qword r10
        add r10, r11
        mov [rbp - 16], r10
        push qword [rbp - 32]
        pop qword [rbp - 8]
        push qword [rbp - 16]
        call print 
        add rsp, 8
        push qword [rbp - 24]
        mov r11, 1
        pop qword r10
        sub r10, r11
        mov [rbp - 24], r10
.L2:
        mov rax, [rbp - 24]
        test rax, rax
        jne .L3
        mov rax, 0
        add rsp, 32
        pop qword rbp
        ret
.L1:
.L0:
        mov rax, 0
        add rsp, 32
        pop qword rbp
        ret
