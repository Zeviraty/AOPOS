#include "terminal.h"
#include "drivers/keyboard.h"

void shell_init() {
    
    terminal_writestring("\n~> ");

    char command_buffer[128];
    int command_pos = 0;

    while (1) {
        int c = input_buffer_get();
        if (c == -1) {
            continue;
        }

        if (c == '\n') {
            command_buffer[command_pos] = '\0';
            terminal_writestring("\n");
            terminal_writestring("You typed: ");
            terminal_writestring(command_buffer);
            terminal_writestring("\n~> ");
            command_pos = 0;
        } else if (c == '\b') {
            if (command_pos > 0) {
                command_pos--;
                terminal_putchar('\b');
                terminal_putchar(' ');
                terminal_putchar('\b');
            }
        } else {
            if (command_pos < sizeof(command_buffer) - 1) {
		if (c >= 32 && c <= 126) {
                    command_buffer[command_pos++] = (char)c;
		    terminal_putchar(c);
		}
            }
        }
    }
}

