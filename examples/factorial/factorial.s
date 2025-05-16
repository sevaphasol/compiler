section .text
global _start

_start:
        call main
        mov rax, 60
        syscall
factorial:
        push qword rbp
        mov rbp, rsp
        sub rsp, 0
        push qword [rbp + 16]
        pop qword rax
        test rax, rax
        je .L0
        push qword [rbp + 16]
        push qword [rbp + 16]
        push qword 1
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call factorial
        add rsp, 8
        push qword rax
        pop qword r11
        pop qword r10
        imul r10, r11
        push qword r10
        pop qword rax
        add rsp, 0
        pop qword rbp
        ret
.L0:
        push qword 1
        pop qword rax
        add rsp, 0
        pop qword rbp
        ret
main:
        push qword rbp
        mov rbp, rsp
        sub rsp, 8
        push qword 0
        pop qword [rbp - 8]
        call scan 
        mov [rbp - 8], rax
        push qword [rbp - 8]
        call factorial
        add rsp, 8
        push qword rax
        call print 
        add rsp, 8
        push qword 0
        pop qword rax
        add rsp, 8
        pop qword rbp
        ret
