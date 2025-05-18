section .text
global _start

_start:
        call main
        mov rax, 60
        syscall
main:
        push qword rbp
        mov rbp, rsp
        mov [rbp - 8], 1
        mov [rbp - 16], 1
        mov [rbp - 24], 0
        call scan 
        mov [rbp - 24], rax
        mov rax, [rbp - 24]
        test rax, rax
        je .L4
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
        je .L5
        push qword [rbp - 16]
        call print 
        add rsp, 8
        push qword [rbp - 24]
        mov r11, 1
        pop qword r10
        sub r10, r11
        mov [rbp - 24], r10
        jmp .L6
.L7:
        mov [rbp - 32], [rbp - 16]
        push qword [rbp - 8]
        mov r11, [rbp - 16]
        pop qword r10
        add r10, r11
        mov [rbp - 16], r10
        mov [rbp - 8], [rbp - 32]
        push qword [rbp - 16]
        call print 
        add rsp, 8
        push qword [rbp - 24]
        mov r11, 1
        pop qword r10
        sub r10, r11
        mov [rbp - 24], r10
.L6:
        mov rax, [rbp - 24]
        test rax, rax
        jne .L7
        mov rax, 0
        pop qword rbp
        ret
.L5:
.L4:
        mov rax, 0
        pop qword rbp
        ret
