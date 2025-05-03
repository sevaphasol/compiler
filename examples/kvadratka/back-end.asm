push 0
pop BP
call main:
hlt
solve_linear:
    push [BP + 0] ;pushing local var b
    push 0
    je else_body_0:
        push 0
        push [BP + 1] ;pushing local var c
        sub
        push [BP + 0] ;pushing local var b
        div
        pop [BP + 2] ;assignment of x
        push [BP + 2] ;pushing local var x
        pop AX ;pop in AX return value
        ret
    else_body_0:
        hlt

discriminant:
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
solve_quadratic_equation:
    push [BP + 0] ;pushing local var a
    push 0
    je else_body_1:
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
        call discriminant:
        pop  BP ;new BP
        push AX ;return value (in AX)
        pop [BP + 3] ;assignment of D
        push 0
        push [BP + 1] ;pushing local var b
        sub
        push [BP + 3] ;pushing local var D
        sqrt
        add
        push 2
        push [BP + 0] ;pushing local var a
        mul
        div
        pop [BP + 4] ;assignment of x_one
        push 0
        push [BP + 1] ;pushing local var b
        sub
        push [BP + 3] ;pushing local var D
        sqrt
        sub
        push 2
        push [BP + 0] ;pushing local var a
        mul
        div
        pop [BP + 5] ;assignment of x_two
        push [BP + 4] ;pushing local var x_one
        out
        push [BP + 5] ;pushing local var x_two
        out
        push 0
        pop AX ;pop in AX return value
        ret
    else_body_1:
        push BP
        push [BP + 1] ;pushing local var b
        push [BP + 2] ;pushing local var c
        push BP ;save BP
        push 2
        add
        pop BP
        pop [BP + 1]
        pop [BP + 0]
        call solve_linear:
        pop  BP ;new BP
        push AX ;return value (in AX)
        pop [BP + 6] ;assignment of x
        push [BP + 6] ;pushing local var x
        out
        push 0
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
    call solve_quadratic_equation:
    pop  BP ;new BP
    push AX ;return value (in AX)
    pop [BP + 3] ;assignment of status
    push 0
    pop AX ;pop in AX return value
    ret
