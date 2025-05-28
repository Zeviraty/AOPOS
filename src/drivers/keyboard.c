#include "system/io.h"
#include "system/terminal.h"

void keyboard_handler() {
    uint8_t scancode = inb(0x60);
    terminal_writestring("Scancode: ");
    terminal_writehex(scancode);
    terminal_writestring("\n");
}

__attribute__((used))
void keyboard_handler();
