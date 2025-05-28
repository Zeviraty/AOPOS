global gdt_flush

gdt_flush:
    mov eax, [esp+4]     ; Pointer to gdt_ptr
    lgdt [eax]           ; Load GDT

    mov ax, 0x10         ; Data segment selector (0x10)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush      ; Far jump to reload CS
.flush:
    ret

