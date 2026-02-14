extern main
section .data
    MAP_FLAGS dq 0x20 | 0x2 ;MAP_ANONYMOUS | MAP_PRIVATE
    PROT_FLAGS dq 0x1 | 0x2 ;PROT_READ | PROT_WRITE

section .text
    global _start
    global print_int
    global print
    global getchar
    global mmap
    global munmap
_start:
    call main
    call _exit
    ret

print_int:
    xor rdx, rdx
    xor rcx, rcx

    mov rsi, 10
    mov rax, rdi

    push -1
    mov rdi, rsp
    sub rsp, 16
    sub rdi, 2
    mov byte[rdi + 1], 0
.lp1:
    xor rdx, rdx
    idiv rsi
    
    add rdx, '0'
    mov byte [rdi], dl
    inc rcx
    sub rdi, 1
    test rax, rax
    jnz .lp1
    add rdi, 1
.out:

    mov rsi, rcx
    call print
    add rsp, 16
    pop rax
    mov rax, rsi
    ret 


print:
    push rdi
    call strlen ; ret count in rdx
    pop rsi ; buf
    mov rax, 1 ; write
    mov rdi, 1 ;stdout
    syscall
    ret

strlen:
    xor rax, rax
    mov rdx, rdi
    cld
    repne scasb
    sub rdi, rdx
    mov rdx, rdi
    ret

getchar:
    push 0          ; buf for read
    xor rax, rax    ; read
    mov rdi, 0      ; stdin
    mov rsi, rsp    ; buf addr
    mov rdx, 1      ; read count

    syscall
    pop rcx

    cmp rax, 0
    jg .quit 
    mov rcx, -1     ; eof or err

.quit:
    mov rax, rcx
    ret

; int page count
mmap:
    ;void *mmap(void *start, size_t length, int prot , int flags, int fd, off_t offset);
    ;x86_64	ret rax	
    ;syscall    rax
    ;arg0       rdi       start
    ;arg1       rsi       length
    ;arg2       rdx       prot
    ;arg3       r10       flags
    ;arg4       r8        fd
    ;arg5       r9        offset
    mov rax, 4096
    mul edi ;page_size * page count
    
    xor rdi, rdi                ; Kernel choose addr
    mov rsi, rax                ; size of memory to map
    mov rdx, qword [PROT_FLAGS] ; MAP_ANONYMOUS | MAP_PRIVATE
    mov r10, qword [MAP_FLAGS]  ; PROT_READ | PROT_WRITE
    mov r8, -1                  ; Not maping file
    xor r9, r9                  ; offset 0
    mov rax, 9                  ; code mmap
    syscall
    ;if error in rax -1 else start map address
    xor rcx, rcx
    cmp rax, -1
    cmove rax, rcx
    ret

    ; void *start_addres
    ; int page count
munmap:
    mov rax, 4096   ; page size
    mul esi         ; page count
    mov rsi, rax    ; unmap size
    mov rax, 11     ; code munmap
    syscall         ; The address is already in rdi
    ;if error in rax -1 else 0
    ret
_exit:
    mov rdi, rax
    mov rax, 60
    syscall
    ret