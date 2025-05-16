section .text
global _start

_start:
        call main
        mov rax, 60
        syscall
solve_linear:
        push qword rbp
        mov rbp, rsp
        sub rsp, 0
        push qword [rbp + 16]
        pop qword rax
        test rax, rax
        je .L0
        push qword 0
        push qword [rbp + 24]
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        push qword [rbp + 16]
        pop qword r10
        pop qword rax
        cqo
        idiv r10
        push qword rax
        call print 
        add rsp, 8
.L0:
        push qword 0
        pop qword rax
        add rsp, 0
        pop qword rbp
        ret
solve_square_eq:
        push qword rbp
        mov rbp, rsp
        sub rsp, 24
        push qword [rbp + 16]
        pop qword rax
        test rax, rax
        je .L1
        push qword [rbp + 24]
        push qword [rbp + 24]
        pop qword r11
        pop qword r10
        imul r10, r11
        push qword r10
        push qword 4
        push qword [rbp + 16]
        pop qword r11
        pop qword r10
        imul r10, r11
        push qword r10
        push qword [rbp + 32]
        pop qword r11
        pop qword r10
        imul r10, r11
        push qword r10
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        pop qword [rbp - 8]
        push qword 0
        push qword [rbp + 24]
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        fildl [rbp - 8]
        fsqrt
        fistpl [rbp - 8]
        push qword [rbp - 8]
        pop qword r11
        pop qword r10
        add r10, r11
        push qword r10
        push qword 2
        push qword [rbp + 16]
        pop qword r11
        pop qword r10
        imul r10, r11
        push qword r10
        pop qword r10
        pop qword rax
        cqo
        idiv r10
        push qword rax
        pop qword [rbp - 16]
        push qword 0
        push qword [rbp + 24]
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        fildl [rbp - 8]
        fsqrt
        fistpl [rbp - 8]
        push qword [rbp - 8]
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        push qword 2
        push qword [rbp + 16]
        pop qword r11
        pop qword r10
        imul r10, r11
        push qword r10
        pop qword r10
        pop qword rax
        cqo
        idiv r10
        push qword rax
        pop qword [rbp - 24]
        push qword [rbp - 16]
        call print 
        add rsp, 8
        push qword [rbp - 24]
        call print 
        add rsp, 8
        push qword 0
        pop qword rax
        add rsp, 24
        pop qword rbp
        ret
.L1:
        push qword [rbp + 32]
        push qword [rbp + 24]
        call solve_linear
        add rsp, 16
        push qword rax
        pop qword rax
        add rsp, 24
        pop qword rbp
        ret
main:
        push qword rbp
        mov rbp, rsp
        sub rsp, 24
        push qword 0
        pop qword [rbp - 8]
        push qword 0
        pop qword [rbp - 16]
        push qword 0
        pop qword [rbp - 24]
        call scan 
        mov [rbp - 8], rax
        call scan 
        mov [rbp - 16], rax
        call scan 
        mov [rbp - 24], rax
        push qword [rbp - 24]
        push qword [rbp - 16]
        push qword [rbp - 8]
        call solve_square_eq
        add rsp, 24
        push qword rax
        pop qword rax
        add rsp, 24
        pop qword rbp
        ret
