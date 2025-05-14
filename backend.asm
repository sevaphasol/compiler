section .text
global _start

_start:
        call main
        mov rax, 60
        syscall
sum:
        push qword rbp
        mov rbp, rsp
        sub rsp, 0
        push qword [rbp + 16]
        push qword 5
        pop qword r10
        pop qword rax
        idiv r10
        push qword rax
        pop qword rax
        add rsp, 0
        pop qword rbp
        ret
main:
        push qword rbp
        mov rbp, rsp
        sub rsp, 8
        push qword 5
        call sum
        add rsp, 8
        push qword rax
        pop qword [rbp - 8]
        push qword 0
        pop qword rax
        add rsp, 8
        pop qword rbp
        ret
