section .text
global _start

_start:
        call main
        mov rax, 60
        syscall
main:
        push qword rbp
        mov rbp, rsp
        sub rsp, 0
        push qword 0
        pop qword rax
        test rax, rax
        je .L0
        push qword 0
        call print 
        add rsp, 8
        push qword 0
        pop qword rax
        add rsp, 0
        pop qword rbp
        ret
.L0:
        push qword 1
        call print 
        add rsp, 8
        push qword 1
        pop qword rax
        add rsp, 0
        pop qword rbp
        ret
