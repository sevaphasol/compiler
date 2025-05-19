section .text
global _start

_start:
        call main
        mov rax, 60
        syscall
h:
        push qword rbp
        mov rbp, rsp
        mov rax, 0
        pop qword rbp
        ret
g:
        push qword rbp
        mov rbp, rsp
        push qword [rbp + 16]
        call h
        add rsp, 8
        mov rax, rax
        pop qword rbp
        ret
f:
        push qword rbp
        mov rbp, rsp
        push qword [rbp + 24]
        push qword [rbp + 16]
        call g
        add rsp, 16
        mov rax, rax
        pop qword rbp
        ret
main:
        push qword rbp
        mov rbp, rsp
        push qword 3
        push qword 2
        push qword 1
        call f
        add rsp, 24
        mov rax, rax
        pop qword rbp
        ret
