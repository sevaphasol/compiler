push 0
pop [0]
push 1
pop BP
call main:
hlt
factorial:
    push [0] ;pushing global var counter
    push 1
    add
    pop [0]
    push [BP + 0] ;pushing local var nomer
    push 1
    sub
    pop [BP + 1] ;assignment of nomer_
    push [BP + 0] ;pushing local var nomer
    push 0
    je else_body_0:
        push BP
        push [BP + 1] ;pushing local var nomer_
        push BP ;save BP
        push 1
        add
        pop BP
        pop [BP + 0]
        call factorial:
        pop  BP ;new BP
        push AX ;return value (in AX)
        push [BP + 0] ;pushing local var nomer
        mul
        pop AX ;pop in AX return value
        ret
    else_body_0:
        push 1
        pop AX ;pop in AX return value
        ret
main:
    push 0
    pop [BP + 0] ;assignment of nomer
    in
    pop [BP + 0]
    push BP
    push [BP + 0] ;pushing local var nomer
    push BP ;save BP
    push 1
    add
    pop BP
    pop [BP + 0]
    call factorial:
    pop  BP ;new BP
    push AX ;return value (in AX)
    pop [BP + 1] ;assignment of fact
    push [BP + 1] ;pushing local var fact
    out
    push [0] ;pushing global var counter
    out
    push [BP + 1] ;pushing local var fact
    pop AX ;pop in AX return value
    ret
