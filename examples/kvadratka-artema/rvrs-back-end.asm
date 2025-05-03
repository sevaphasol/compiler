push 0
pop BP
call main:
hlt
diskriminant:
    push [BP + 1] ;pushing local var b
    push [BP + 1] ;pushing local var b
    mul
    push 4
    push [BP + 0] ;pushing local var a
    mul
    push [BP + 2] ;pushing local var c
    mul
    sub
    pop AX ;pop in AX return value
    ret
reshi_pozhaluysta_lineinoe:
    push [BP + 0] ;pushing local var b
    push 0
    je else_body_0:
        push 0
        push [BP + 1] ;pushing local var c
        push [BP + 0] ;pushing local var b
        div
        sub
        out
        push 1
        pop AX ;pop in AX return value
        ret
    else_body_0:
        push [BP + 1] ;pushing local var c
        push 0
        je else_body_1:
            push 0
            pop AX ;pop in AX return value
            ret
        else_body_1:
            push 1488
            out
            push 211
            pop AX ;pop in AX return value
            ret
reshi_pozhaluysta_kvadratnoe:
    push BP
    push [BP + 0] ;pushing local var a
    push [BP + 1] ;pushing local var b
    push [BP + 2] ;pushing local var c
    push BP ;save BP
    push 3
    add
    pop BP
    pop [BP + 2]
    pop [BP + 1]
    pop [BP + 0]
    call diskriminant:
    pop  BP ;new BP
    push AX ;return value (in AX)
    pop [BP + 3] ;assignment of D
    push [BP + 3] ;pushing local var D
    sqrt
    pop [BP + 4] ;assignment of koren
    push 0
    push [BP + 1] ;pushing local var b
    sub
    push [BP + 4] ;pushing local var koren
    sub
    push 2
    push [BP + 0] ;pushing local var a
    mul
    div
    out
    push 0
    push [BP + 1] ;pushing local var b
    sub
    push [BP + 4] ;pushing local var koren
    add
    push 2
    push [BP + 0] ;pushing local var a
    mul
    div
    out
    push 0
    pop AX ;pop in AX return value
    ret
reshi_pozhaluysta_uravneniye:
    push [BP + 0] ;pushing local var a
    push 0
    je else_body_2:
        push BP
        push [BP + 0] ;pushing local var a
        push [BP + 1] ;pushing local var b
        push [BP + 2] ;pushing local var c
        push BP ;save BP
        push 3
        add
        pop BP
        pop [BP + 2]
        pop [BP + 1]
        pop [BP + 0]
        call reshi_pozhaluysta_kvadratnoe:
        pop  BP ;new BP
        push AX ;return value (in AX)
        pop AX ;pop in AX return value
        ret
    else_body_2:
        push BP
        push [BP + 1] ;pushing local var b
        push [BP + 2] ;pushing local var c
        push BP ;save BP
        push 2
        add
        pop BP
        pop [BP + 1]
        pop [BP + 0]
        call reshi_pozhaluysta_lineinoe:
        pop  BP ;new BP
        push AX ;return value (in AX)
        pop AX ;pop in AX return value
        ret
main:
    push 0
    pop [BP + 0] ;assignment of a
    push 0
    pop [BP + 1] ;assignment of b
    push 0
    pop [BP + 2] ;assignment of c
    in
    pop [BP + 0]
    in
    pop [BP + 1]
    in
    pop [BP + 2]
    push BP
    push [BP + 0] ;pushing local var a
    push [BP + 1] ;pushing local var b
    push [BP + 2] ;pushing local var c
    push BP ;save BP
    push 3
    add
    pop BP
    pop [BP + 2]
    pop [BP + 1]
    pop [BP + 0]
    call reshi_pozhaluysta_uravneniye:
    pop  BP ;new BP
    push AX ;return value (in AX)
    pop [BP + 3] ;assignment of num
    push [BP + 3] ;pushing local var num
    pop AX ;pop in AX return value
    ret
