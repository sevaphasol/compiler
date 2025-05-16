section .text
global _start

_start:
        call main
        mov rax, 60
        syscall
h:
        push qword rbp
        mov rbp, rsp
        sub rsp, 0
        push qword 0
        pop qword rax
        add rsp, 0
        pop qword rbp
        ret
g:
        push qword rbp
        mov rbp, rsp
        sub rsp, 0
        push qword [rbp + 16]
        call h
        add rsp, 8
        push qword rax
        pop qword rax
        add rsp, 0
        pop qword rbp
        ret
f:
        push qword rbp
        mov rbp, rsp
        sub rsp, 0
        push qword [rbp + 24]
        push qword [rbp + 16]
        call g
        add rsp, 16
        push qword rax
        pop qword rax
        add rsp, 0
        pop qword rbp
        ret
main:
        push qword rbp
        mov rbp, rsp
        sub rsp, 0
        push qword 3
        push qword 2
        push qword 1
        call f
        add rsp, 24
        push qword rax
        pop qword rax
        add rsp, 0
        pop qword rbp
        ret
