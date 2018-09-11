# Hodaya Koslowsky 313377673
.data
d:        .string "%d"
s:        .string "%s"

.text

.globl  main
    .type   main, @function

main:
    pushq   %rbp
    movq    %rsp,       %rbp

#well use r12 and r13 to save pointers of the strings on the stack

#get the first string
    leaq    -4(%rsp),   %rsp    # place on stack for the length
    movq    $d,         %rdi
    movq    %rsp,       %rsi
    xor     %rax,       %rax
    call    scanf
    
    movl    (%rsp),     %r12d   #save the length to r12
       
    subq    %r12,       %rsp    # place for on the stack for the first string    
    dec     %rsp

    movq    $s,         %rdi    # get the string
    movq    %rsp,       %rsi
    xor     %rax,       %rax
    call    scanf  
    
    dec     %rsp                # decrease rsp and save the char of length
    movb    %r12b,      (%rsp)
    movq    %rsp,       %r12    #save the adress of the first string at r12

#get the second string
    leaq    -4(%rsp),   %rsp    # place on stack for char
    movq    $d,         %rdi
    movq    %rsp,       %rsi
    xorq     %rax,       %rax
    call    scanf
 
    xorq     %r13,         %r13
    movl    (%rsp),     %r13d   #save the len to r13
       
    subq    %r13,       %rsp    # place for string    
    dec     %rsp

    movq    $s,         %rdi # get the string
    movq    %rsp,       %rsi
    xorq    %rax,         %rax
    call    scanf  
    
    dec     %rsp                # dec rsp and save the char of length
    movb    %r13b,      (%rsp)
    movq    %rsp,       %r13    #save the adress at r13
        
#get the selected option
    
    leaq    -4(%rsp),   %rsp    #save place for the option
    movq    $d,         %rdi
    movq    %rsp,       %rsi
    movq    $0,         %rax
    call    scanf               # scan the option
        
#move the parameters: rdi will save the first string, rsi the second and rdx the selected option

    movl    (%rsp),     %edx    #rdx saves the option
    leaq    4(%rsp),   %rsp     #free place on the stack
        
    movq    %r12,       %rdi    #first pstring
    movq    %r13,       %rsi    #second pstring
    
    call    run_func
    
    xorq    %rax,       %rax
    movq    %rbp,       %rsp
    popq    %rbp
    
    ret
