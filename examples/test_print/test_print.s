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
        push qword 1
        call print 
        add rsp, 8
        push qword 22
        call print 
        add rsp, 8
        push qword 333
        call print 
        add rsp, 8
        push qword 4444
        call print 
        add rsp, 8
        push qword 55555
        call print 
        add rsp, 8
        push qword 666666
        call print 
        add rsp, 8
        push qword 7777777
        call print 
        add rsp, 8
        push qword 88888888
        call print 
        add rsp, 8
        push qword 999999999
        call print 
        add rsp, 8
        push qword 88888888
        call print 
        add rsp, 8
        push qword 7777777
        call print 
        add rsp, 8
        push qword 666666
        call print 
        add rsp, 8
        push qword 55555
        call print 
        add rsp, 8
        push qword 4444
        call print 
        add rsp, 8
        push qword 333
        call print 
        add rsp, 8
        push qword 22
        call print 
        add rsp, 8
        push qword 1
        call print 
        add rsp, 8
        push qword 0
        push qword 1
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 22
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 333
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 4444
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 55555
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 666666
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 7777777
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 88888888
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 999999999
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 88888888
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 7777777
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 666666
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 55555
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 4444
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 333
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 22
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        push qword 1
        pop qword r11
        pop qword r10
        sub r10, r11
        push qword r10
        call print 
        add rsp, 8
        push qword 0
        pop qword rax
        add rsp, 0
        pop qword rbp
        ret
