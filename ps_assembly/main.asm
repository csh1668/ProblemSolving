; bss section: static variables (filled 0 when excuted)
section .bss
    a: resd 1 ; res(reserve), d(dword 4bytes) -> pointer that points 4byte value
    b: resd 1

; data section: static variables that have its own initial value
section .data
    input: db "%d %d", 0 ; d(data) b(1byte)
    output: db "%d", 10, 0 ; 10 means \n

; codes
section .text
    global main
    extern scanf
    extern printf

main:
    push rbp
    
    ; parameter order
    ; rdi rsi rdx rcx r8 r9 stack
    mov rdi, input
    mov rsi, a
    mov rdx, b
    mov rax, 0
    call scanf

    mov rax, [a] ; [] is * in C
    add rax, [b] ; all variables in asm is an address

    mov rdi, output
    mov rsi, rax
    mov rax, 0
    call printf

    pop rbp
    mov rax, 0
    ret