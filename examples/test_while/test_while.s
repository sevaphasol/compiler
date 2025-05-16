section .text
global _start

_start:
        call main
        mov rax, 60
        syscall
main:
        push qword rbp
        mov rbp, rsp
        sub rsp, 8
        push qword 5
        pop qword [rbp - 8]
        jmp .L0
.L1:
        push qword [rbp - 8]
        call print 
        add rsp, 8
        push qword [rbp - 8]
        push qword 1
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        pop qword [rbp - 8]
.L0:
        push qword [rbp - 8]
        pop qword rax
        test rax, rax
        jne .L1
        push qword 0
        pop qword rax
        add rsp, 8
        pop qword rbp
        ret
