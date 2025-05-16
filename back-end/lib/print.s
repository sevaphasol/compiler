section .text

print:
        mov eax, [rsp + 8] ; number is in the stack

        sub rsp, 16
        mov rsi, rsp ; start of buffer
        add rsi, 15  ; end of buffer

        pxor xmm0, xmm0 ; zeroing buffer
        movdqu [rsp], xmm0

        xor ebx, ebx
        test eax, eax
        jns .positive

.negative:
        neg eax
        inc ebx ; ebx = 1 — means negative

.positive:
        mov ecx, 10 ; ecx = divider

.convert_loop:
        xor edx, edx
        div ecx ; eax = eax / 10, edx = eax % 10
        dec rsi
        add dl, '0'
        mov [rsi], dl
        test eax, eax
        jnz .convert_loop

        test ebx, ebx
        jz .print_buffer

.add_minus:
        dec rsi
        mov byte [rsi], '-'

.print_buffer:
        mov rdi, rsi ; rdi = start of buffer

.find_end:
        cmp byte [rdi], 0
        je .found_end
        inc rdi
        jmp .find_end

.found_end:
        mov byte [rdi], 0x0a ; '\n'

        lea rdx, [rdi + 1]
        sub rdx, rsi ; rdx = buffer size

        mov rax, 1 ; sys_write
        mov rdi, 1 ; stdout
        syscall

        add rsp, 16
        ret
