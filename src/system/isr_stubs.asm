[bits 32]
global isr_stub_table
extern exception_handler
extern keyboard_handler

%macro isr_no_err_stub 1
isr_stub_%1:
    push dword 0            ; push fake error code
    push dword %1           ; push interrupt number
    call exception_handler
    add esp, 8              ; clean stack
    iret
%endmacro

%macro isr_err_stub 1
isr_stub_%1:
    push dword %1           ; push interrupt number (real error code already on stack)
    call exception_handler
    add esp, 4              ; clean stack
    iret
%endmacro

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

isr_stub_table:
%assign i 0
%rep 32
    dd isr_stub_%+i
%assign i i+1
%endrep

global keyboard_handler_stub

keyboard_handler_stub:
    cli
    pushad            ; save all general registers
    push ds
    push es
    push fs
    push gs

    ; set data segments here if needed

    call keyboard_handler

    pop gs
    pop fs
    pop es
    pop ds
    popad

    ; send EOI to PIC
    mov al, 0x20
    out 0x20, al

    sti
    iret
