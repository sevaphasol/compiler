section .text
global _start

_start:
        call main
        mov rax, 60
        syscall
main:
        push qword rbp
        mov rbp, rsp
        push qword 1
        call print 
        add rsp, 8
        mov rax, 0
        pop qword rbp
        ret
