section .text
global _start

_start:
        call _main
        mov rax, 60
        syscall
_add:
        push rbp
        mov rbp, rsp
        sub rsp, 0 
        push qword [rbp + 16] ; func param 'minion'
        pop r10
        push qword [rbp + 24] ; func param 'papoi'
        pop r11
        add r10, r11
        push r10
        pop rax
        add rsp, 0
        pop rbp
        ret
_main:
        push rbp
        mov rbp, rsp
        sub rsp, 24
        push 1
        pop qword [rbp - 8] ; assign local var 'minion'
        push 2
        pop qword [rbp - 16] ; assign local var 'papoi'
        push qword [rbp - 16] ; local var 'papoi'
        push qword [rbp - 8] ; local var 'minion'
        call _add
        add rsp, 16
        push rax
        pop qword [rbp - 24] ; assign local var 'sum'
        push 0
        pop rax
        add rsp, 24
        pop rbp
        ret
