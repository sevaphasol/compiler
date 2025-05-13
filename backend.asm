section .text
global _start

_start:
        call main
        mov rax, 60
        syscall
add:
        push qword rbp
        mov rbp, rsp
        sub rsp, 0
        push qword [rbp - 16]
        pop qword r10
        push qword [rbp - 24]
        pop qword r11
        add r10, r11
        push qword r10
        pop qword rax
        add rsp, 0
        pop qword rbp
        ret
main:
        push qword rbp
        mov rbp, rsp
        sub rsp, 24
        push qword 1
        pop qword [rbp + 8]
        push qword 2
        pop qword [rbp + 16]
        push qword [rbp + 8]
        pop qword r10
        push qword [rbp + 16]
        pop qword r11
        sub r10, r11
        push qword r10
        pop qword rax
        test rax, rax
        je .L0
        push qword [rbp + 16]
        push qword [rbp + 8]
        call add
        add rsp, 16
        push qword rax
        pop qword [rbp + 24]
.L0:
        push qword 0
        pop qword rax
        add rsp, 24
        pop qword rbp
        ret
