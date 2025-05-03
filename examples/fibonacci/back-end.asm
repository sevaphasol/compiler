push 5
pop [0]
push 1
pop BP
call main:
hlt
main:
    push 1
    pop [BP + 0] ;assignment of a
    push 1
    pop [BP + 1] ;assignment of b
    push [BP + 0] ;pushing local var a
    out
    push [BP + 1] ;pushing local var b
    out
    start_of_while_0:
    push [0] ;pushing global var counter
    push 0
    je end_of_while_0:
        push [BP + 1] ;pushing local var b
        pop [BP + 2] ;assignment of tmp
        push [BP + 0] ;pushing local var a
        push [BP + 1] ;pushing local var b
        add
        pop [BP + 1] ;assignment of b
        push [BP + 2] ;pushing local var tmp
        pop [BP + 0] ;assignment of a
        push [BP + 1] ;pushing local var b
        out
        push [0] ;pushing global var counter
        push 1
        sub
        pop [0]
    jmp start_of_while_0:
    end_of_while_0:
    push 0
    pop AX ;pop in AX return value
    ret
