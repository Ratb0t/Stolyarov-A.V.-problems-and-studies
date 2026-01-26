global check_overflow
section .note.GNU-stack noalloc noexec nowrite progbits
section .text


check_overflow:
    xor rax, rax
    add edi, esi
    seto al
    ret
    