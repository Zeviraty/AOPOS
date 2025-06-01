#include "terminal.h"
#include "drivers/vga.h"
#include "system/x86/gdt.h"
#include "system/x86/idt.h"
#include "system/x86/io.h"
#include "system/x86/pic.h"
#include "shell.h"
#include "drivers/ata.h"
#include "lib/string.h"
#include "fs/fs.h"

void kernel_main(void) {
    asm volatile("cli");
    
    gdt_install();
    pic_remap();
    idt_init();

    terminal_initialize();

    asm volatile("sti");

    fs_init();

    terminal_writestring("===================================\n");
    terminal_writestring("===            ");
    terminal_setcolor(VGA_COLOR_MAGENTA);
    terminal_writestring("AOPOS");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("            ===\n");
    terminal_writestring("===  Another OSdev Project OS   ===\n");
    terminal_writestring("===================================\n");

    shell_init();
}
