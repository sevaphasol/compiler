push 0
pop BP
call main:
hlt
factorial:
    push [BP + 0] ;pushing local var n
    push 0
    je else_body_0:
        push [BP + 0] ;pushing local var n
        push BP
        push [BP + 0] ;pushing local var n
        push 1
        sub
        push BP ;save BP
        push 1
        add
        pop BP
        pop [BP + 0]
        call factorial:
        pop  BP ;new BP
        push AX ;return value (in AX)
        mul
        pop AX ;pop in AX return value
        ret
    else_body_0:
        push 1
        pop AX ;pop in AX return value
        ret
main:
    push 5
    pop [BP + 0] ;assignment of n
    in
    pop [BP + 0]
    push BP
    push [BP + 0] ;pushing local var n
    push BP ;save BP
    push 1
    add
    pop BP
    pop [BP + 0]
    call factorial:
    pop  BP ;new BP
    push AX ;return value (in AX)
    out
    push 0
    pop AX ;pop in AX return value
    ret
