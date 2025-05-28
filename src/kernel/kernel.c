#include "system/terminal.h"
#include "drivers/vga.h"
#include "system/gdt.h"
#include "system/idt.h"
#include "system/io.h"
#include "system/pic.h"

void kernel_main(void) {
    gdt_install();
    pic_remap();
    idt_init();
    asm volatile("sti");
    
    terminal_initialize();

    terminal_writestring("===================================\n");
    terminal_writestring("===            ");
    terminal_setcolor(VGA_COLOR_MAGENTA);
    terminal_writestring("AOPOS");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("            ===\n");
    terminal_writestring("===  Another OSdev Project OS   ===\n");
    terminal_writestring("===================================\n");

    while(1) { __asm__ volatile ("hlt"); }
}
