section .text
global _start

_start:
        add rax, 7
        mov rax, 60
        syscall
        nop
        push qword [rbp + 123]
        ret
