# Hodaya Koslowsky 313377673
.section .rodata
    .align 8
.switch:
    .quad   .switch_A
    .quad   .switch_B
    .quad   .switch_C
    .quad   .switch_D
    .quad   .switch_Default
    
err:    .string "invalid option!\n"
print1: .string "first pstring length: %d, second pstring length: %d\n"
print2: .string "old char: %c, new char: %c, first string: %s, second string: %s\n"
print3: .string "length: %d, string: %s\n"

d:    .string "%d"
s:    .string "%s"
c:    .string " %c"
twoc: .string " %c %c"

.text

.globl  run_func
    .type   run_func, @function

run_func:

    pushq   %rbp
    movq    %rsp,       %rbp
    
#we will save the first string in r12 and the second in r13.
#they are transfered to run_func through rdi and rsi
    movq    %rdi,       %r12    #first
    movq    %rsi,       %r13    #second
    leaq    -50(%rdx),  %rdx    #subtract 50 from the option given in rdx

    cmp     $0,         %rdx
    jb      .switch_Default
    cmp     $3,         %rdx
    ja      .switch_Default
    jmp     *.switch(,%rdx,8)

.switch_A:
    movq    %r12,       %rdi
    movq    $0,         %rax
    call    pstrlen             #call length for first string
    movq    %rax,       %r14
    movq    %r13,       %rdi
    movq    $0,         %rax
    call    pstrlen             #call length for second string
    movq    %rax,       %r15
    
    movq    %r14,       %rsi
    movq    %r15,       %rdx
    movq    $print1,    %rdi
    movq    $0,         %rax
    call    printf              #print according to the requested format
    jmp     .end_switch

.switch_B:

    #save place for two characters
    dec     %rsp
    movq    %rsp,       %rsi    #rsi will get the first char from scanf
    dec     %rsp
    movq    %rsp,       %rdx    #rdi will get the second char from scanf
        
    movq    $twoc,      %rdi    #scanf format for two chars  
    movq    $0,         %rax
    call    scanf
    
    movzbq  (%rsp),     %r15    #r15 will hold the new char
    inc     %rsp
    movzbq  (%rsp),     %r14    #r14 will hold the old char

    #replace char will get pstring pointer in rdi, old char in rsi, new char in rdx            
    
    #replace for first
    movq    %r12,       %rdi    #first pstring
    movq    %r14,       %rsi    #old char
    movq    %r15,       %rdx    #new char
    movq    $0,         %rax
    call    replaceChar
    movq    %rax,       %r12    #new first pstring

    #replace for second
    movq    %r13,       %rdi    #second pstring
    movq    %r14,       %rsi    #old char
    movq    %r15,       %rdx    #new char
    movq    $0,         %rax
    call    replaceChar
    movq    %rax,       %r13    #new second pstring
        
    inc     %r12                #increment the two pstring to print without len
    inc     %r13
    
    movq    $print2,    %rdi    #print format
    movq    %r14,       %rsi    #old char
    movq    %r15,       %rdx    #new char
    movq    %r12,       %rcx    #first string
    movq    %r13,       %r8     #second string
    movq    $0,         %rax
    call    printf
    jmp     .end_switch
    
.switch_C:
    
    #scan int i to r14 and int j to r15
    leaq    -4(%rsp),   %rsp    #save place for int
    movq    $d,         %rdi    #scan format
    movq    %rsp,       %rsi    #save to the stack
    xorq    %rax,         %rax
    call    scanf
    movl    (%rsp),     %r14d #index i
    
    leaq    -4(%rsp),   %rsp    #save place for int
    movq    $d,         %rdi
    movq    %rsp,       %rsi
    xorq    %rax,       %rax
    call    scanf
    movl    (%rsp),     %r15d #index j
    
    #move parameters for the function: rdi-dst, rsi-src, rdx-i, rcx-j
    pushq   %r13                #save source on the stack to print afterwards
    movq    %r12,       %rdi    #dst
    movq    %r13,       %rsi    #src
    movq    %r14,       %rdx    #i
    movq    %r15,       %rcx    #j
    call    pstrijcpy
    popq    %r13                #return the source pstring from the stack
    
    movq    $print3,    %rdi    #print format for first pstring
    movzbq  (%r12),     %rsi    #len
    inc     %r12                #increase pointer
    movq    %r12,       %rdx    #string itself
    xorq    %rax,       %rax
    call    printf
    
    movq    $print3,    %rdi    #print format for second pstring
    movzbq  (%r13),     %rsi    #len
    inc     %r13                #increase pointer
    movq    %r13,       %rdx    #string itself
    xorq    %rax,       %rax
    call    printf
    jmp     .end_switch
    

.switch_D:

    #swapcase will get the pstring in rdi
    movq    %r12,       %rdi    #give first pstring
    xorq    %rax,       %rax
    call    swapCase
    movq    %rax,       %r12    #update the first pstring from the function

    movq    %r13,       %rdi    #give second pstring
    xorq    %rax,       %rax
    call    swapCase
    movq    %rax,      %r13    #update the second pstring from the function
    
    movq    $print3,    %rdi    #print format for the first
    movzbq  (%r12),     %rsi
    inc     %r12                #increase the pointer to print without len
    movq    %r12,       %rdx
    xorq    %rax,       %rax
    call    printf
    
    movq    $print3,    %rdi    #print format for the second
    movzbq  (%r13),     %rsi
    inc     %r13                #increase the pointer to print without len
    movq    %r13,       %rdx
    xorq    %rax,       %rax
    call    printf
    jmp     .end_switch
    
    
.switch_Default:
    movq    $err,      %rdi     #print error message
    xorq    %rax,      %rax
    call    printf
    
.end_switch:
    xorq    %rax,      %rax         
    movq    %rbp,      %rsp
    popq    %rbp
    ret 
       
