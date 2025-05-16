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
        push qword 25
        pop qword [rbp - 8]
        call scan 
        mov [rbp - 8], rax
        fildl [rbp - 8]
        fsqrt
        fistpl [rbp - 8]
        push qword [rbp - 8]
        pop qword [rbp - 8]
        push qword [rbp - 8]
        call print 
        add rsp, 8
        push qword 0
        pop qword rax
        add rsp, 8
        pop qword rbp
        ret
