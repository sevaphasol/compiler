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
        push qword 1
        pop qword [rbp - 8]
        push qword 1
        pop qword [rbp - 16]
        push qword 0
        pop qword [rbp - 24]
        call scan 
        mov [rbp - 24], rax
        push qword [rbp - 24]
        pop qword rax
        test rax, rax
        je .L0
        push qword [rbp - 8]
        call print 
        add rsp, 8
        push qword [rbp - 24]
        push qword 1
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        pop qword [rbp - 24]
        push qword [rbp - 24]
        pop qword rax
        test rax, rax
        je .L1
        push qword [rbp - 16]
        call print 
        add rsp, 8
        push qword [rbp - 24]
        push qword 1
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        pop qword [rbp - 24]
        jmp .L2
.L3:
        push qword [rbp - 16]
        pop qword [rbp - 32]
        push qword [rbp - 8]
        push qword [rbp - 16]
        pop qword r11
        pop qword r10
        add r10, r11
        push qword r10
        pop qword [rbp - 16]
        push qword [rbp - 32]
        pop qword [rbp - 8]
        push qword [rbp - 16]
        call print 
        add rsp, 8
        push qword [rbp - 24]
        push qword 1
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        pop qword [rbp - 24]
.L2:
        push qword [rbp - 24]
        pop qword rax
        test rax, rax
        jne .L3
        push qword 0
        pop qword rax
        add rsp, 32
        pop qword rbp
        ret
.L1:
.L0:
        push qword 0
        pop qword rax
        add rsp, 32
        pop qword rbp
        ret
