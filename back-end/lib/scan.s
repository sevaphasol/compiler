section .text

scan:
        sub rsp, 16
        mov rsi, rsp ; start of buffer
        mov rdx, 16  ; buffer size

.read_input:
        mov rax, 0 ; sys_read
        mov rdi, 0 ; stdin
        syscall

        test rax, rax ; check status
        jle .exit

.read_int:
        xor rax, rax ; result
        xor rbx, rbx ; sign of minus
        xor rcx, rcx ; counter

.check_sign:
        cmp byte [rsi], '-'
        jne .parse_loop

.negative:
        inc rcx
        inc rbx ; rbx = 1 — means negative

.parse_loop:
        movzx rdx, byte [rsi + rcx]
        test dl, dl
        jz .parse_done
        cmp dl, '0'
        jb .parse_done
        cmp dl, '9'
        ja .parse_done

        imul eax, eax, 10

        sub dl, '0'
        add rax, rdx

        inc rcx
        jmp .parse_loop

.parse_done:
        test bl, bl
        jz .done
        neg eax

.done:
        add rsp, 16
        ret

.exit:
        add rsp, 16
        mov rax, 60  ; sys_exit
        mov rdi, 0
        syscall

