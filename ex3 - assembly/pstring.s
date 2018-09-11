# Hodaya Koslowsky 313377673

.section .rodata
err:    .string "invalid input!\n"

.text

########################
.globl  pstrlen
    .type pstrlen, @function

pstrlen:
# rdi-points to the pstring
    pushq   %rbp
    movq    %rsp,       %rbp

    movzbq  (%rdi),     %rax    #copy the first byte of rdi with len
    
    movq    %rbp,       %rsp
    popq    %rbp
    ret

########################
.globl  replaceChar
    .type replaceChar, @function

replaceChar:
#pstring in rdi, old char in rsi, new char in rdx, rcx will be a len counter

    pushq   %rbp
    movq    %rsp,       %rbp
    
    pushq   %rdi                #save the beginning of pstring on the function stack, for the return
    
    movzbq  (%rdi),     %rcx    #rcx counter - will start from the len and go down to 0

.loop1:
    cmp     $0,         %rcx    #continue while rcx is positive
    je      .end1

    dec     %rcx                #decrease counter
    inc     %rdi                #increase the pointer of pstring

    movzbq  (%rdi),     %rax    #copy the corrent byte of pstring
    
    cmp     %al,        %sil     #if the byte is not equal to old char, continue
    jne     .loop1
    movb    %dl,        (%rdi)   #if the byte is equal, put new char in the pstring

    jmp     .loop1
    
.end1:
    popq    %rdi                #pop the original pstring pointer
    movq    %rdi,       %rax    #move to rax for the return

    movq    %rbp,       %rsp
    popq    %rbp
    ret

########################
.globl  pstrijcpy
    .type pstrijcpy, @function

pstrijcpy:
#rdi-dst, rsi-src, rdx-i, rcx-j
    pushq   %rbp
    movq    %rsp,       %rbp
    pushq   %rdi                #save dest pointer
    
    #check input correctness. the following are wrong
    cmp     $0,         %rdx    #if i < 0
    jb      .error
    cmp     %rcx,       (%rdi)  #dst.len < j
    jb     .error
    cmp     %rcx,       (%rsi)  #src.len < j
    jb     .error
    cmp     %rdx,       %rcx    #j < i
    jb     .error
    
    #go to the first index to change in src and dst, by adding i to both pointers
    leaq    (%rdx, %rdi),   %rdi    #add i to dst pointer, by adding rdx
    leaq    (%rdx, %rsi),   %rsi    #add i to src pointer, by adding rdx

.loop2:

    cmp     %rdx,           %rcx    #finish when j < i
    jb     .end2

    inc     %rdi                    #move dst pointer
    inc     %rsi                    #move src pointer
    movb    (%rsi),         %bl     #copy the src byte to another register
    movb    %bl,            (%rdi)  #copy from the register to dst byte
    
    inc     %rdx                    #move the index i
    
    jmp     .loop2

.error:
    movq    $err,           %rdi    #error message
    xorq    %rax,           %rax
    call    printf 
     
.end2:
    popq    %rdi                    #pop original dst pointer
    movq    %rdi,           %rax
    movq    %rbp,           %rsp
    popq    %rbp
    ret

########################
.globl  swapCase
    .type swapCase, @function

swapCase:
#rdi will save pstring, rsi will be a counter
    pushq   %rbp
    movq    %rsp,           %rbp
    pushq   %rdi                    #save the beginning pointer on the stack
                
    movzbq  (%rdi),         %rsi    #copy the len to the counter rsi
                  
.loop3:
    cmp     $0,             %rsi    #finish when counter < 0
    jbe     .end3

    inc     %rdi                    #increase pointer
    dec     %rsi                    #increase counter
    
    movzbq  (%rdi),         %rdx    #copy the char to rdx
            
    cmp     $'A',           %rdx    # if char < 'A', continue
    jb      .loop3
    cmp     $'z',           %rdx    #if char > 'z', continue
    ja      .loop3
    
    cmp     $'Z',           %rdx    #if char < 'Z', jump to lower
    jbe     .tolow
    cmp     $'a',           %rdx    #if char > 'a', jump to upper
    jae     .toup
                            #if neither, continue the loop
    jmp     .loop3

.tolow:
    addb     $32,           (%rdi)  #add 32 and turn the char to lower case
    jmp     .loop3                  #continue the loop
.toup:
    subb     $32,           (%rdi)  #subtract 32 and turn the char to upper case
    jmp     .loop3                  #continue the loop
.end3:
    popq    %rdi                    #return the beginning of the pstring
    movq    %rdi,           %rax
    movq    %rbp,           %rsp
    popq    %rbp
    ret
