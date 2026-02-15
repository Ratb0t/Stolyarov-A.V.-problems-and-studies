%macro syscall_ 1-*
    %rep %0 -1
        %rotate -1
        push %1
    %endrep

    %if %0 > 1
        pop rdi
        %if %0 > 2
            pop rsi
            %if %0 > 3
                pop rdx
                %if %0 > 4
                    pop r10
                    %if %0 > 5
                        pop r8
                        %if %0 > 6
                            pop r9
                        %endif
                    %endif
                %endif
            %endif
        %endif
    %endif

    %rotate -1
    mov rax, %1
    syscall
%endmacro

%assign exit_       60
%assign read_       0
%assign write_      1
%assign open_       2
%assign close_      3


%assign STDIN       0
%assign STDOUT      1


extern main

global print
global open
global close
global write
global read
section .text
    global _start
_start:
    mov rdi, [rsp]
    lea rsi, qword [rsp + 8]
    call main
    syscall_ exit_, rax
    ret

print:
    call strlen
    syscall_ write_, STDOUT, rdi, rax
    ret
    
strlen:
    xor rax, rax
    mov rsi, rdi
    mov ecx, -1
    cld
    repne scasb
    sub rdi, rsi
    mov rax, rdi
    mov rdi, rsi
    ret

open:
    syscall_ open_, rdi, rsi, rdx
    ret

close:
    syscall_ close_, rdi
    ret

write:
    syscall_ write_, rdi, rsi, rdx
    ret

read:
    syscall_ read_, rdi, rsi, rdx
    ret