#include <stdint.h>
#include <stddef.h>
#include "system/terminal.h"

const char* exception_messages[] = {
    "Division By Zero", "Debug", "Non Maskable Interrupt", "Breakpoint", "Into Detected Overflow",
    "Out of Bounds", "Invalid Opcode", "No Coprocessor", "Double Fault", "Coprocessor Segment Overrun",
    "Bad TSS", "Segment Not Present", "Stack Fault", "General Protection Fault", "Page Fault",
    "Unknown Interrupt", "Coprocessor Fault", "Alignment Check", "Machine Check",
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved"
};

void itoa(int value, char* str) {
    char* ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;
    if (value < 0) {
        *ptr++ = '-';
        value = -value;
    }
    do {
        tmp_value = value;
        value /= 10;
        *ptr++ = "0123456789"[tmp_value - value * 10];
    } while (value);
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

void exception_handler(uint32_t int_no, uint32_t err_code) {
    char int_str[10], err_str[10];

    const char* message = (int_no < 32) ? exception_messages[int_no] : "Unknown Exception";

    itoa(int_no, int_str);
    itoa(err_code, err_str);

    terminal_writestring("EXCEPTION ");
    terminal_writestring(int_str);
    terminal_writestring(": ");
    terminal_writestring(message);
    terminal_writestring(" (err code: ");
    terminal_writestring(err_str);
    terminal_writestring(")\n");
}

