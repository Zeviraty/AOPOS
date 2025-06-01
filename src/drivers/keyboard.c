#include "system/x86/io.h"
#include <stdint.h>
#include <stdbool.h>

#define INPUT_BUFFER_SIZE 128

static char input_buffer[INPUT_BUFFER_SIZE];
static int input_head = 0;  // Next free position
static int input_tail = 0;  // Next character to read

bool input_buffer_full() {
    return ((input_head + 1) % INPUT_BUFFER_SIZE) == input_tail;
}

bool input_buffer_empty() {
    return input_head == input_tail;
}

void input_buffer_put(char c) {
    if (!input_buffer_full()) {
        input_buffer[input_head] = c;
        input_head = (input_head + 1) % INPUT_BUFFER_SIZE;
    }
}

int input_buffer_get() {
    if (input_buffer_empty()) {
        return -1; // No data
    }
    char c = input_buffer[input_tail];
    input_tail = (input_tail + 1) % INPUT_BUFFER_SIZE;
    return c;
}


unsigned char scancode_to_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', // 0x00 - 0x07
   '7', '8', '9', '0', '-', '=', '\b',   // Backspace
   '\t',                                 // Tab
   'q', 'w', 'e', 'r',                   // 0x10 - 0x13
   't', 'y', 'u', 'i',
   'o', 'p', '[', ']', '\n',             // Enter key
   0,                                    // Control
   'a', 's', 'd', 'f',
   'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
   0,                                    // Left shift
   '\\', 'z', 'x', 'c', 'v', 'b', 'n',   // 0x2C - 0x33
   'm', ',', '.', '/', 0,               // Right shift
   '*',
   0,                                    // Alt
   ' ',                                  // Space bar
   0,                                    // Caps lock
};

unsigned char scancode_to_shift_ascii[] = {
    ')',  27, '!', '@', '#', '$', '%', '^', // 0x00 - 0x07
   '&', '*', '(', ')', '_', '+', '\b',   // Backspace
   '\t',                                 // Tab
   'Q', 'W', 'E', 'R',                   // 0x10 - 0x13
   'T', 'Y', 'U', 'I',
   'O', 'P', '{', '}', '\n',             // Enter key
   0,                                    // Control
   'A', 'S', 'D', 'F',
   'G', 'H', 'J', 'K', 'L', ':', '|', '~',
   0,                                    // Left shift
   '\\', 'Z', 'X', 'C', 'V', 'B', 'N',   // 0x2C - 0x33
   'M', '<', '>', '?', 0,               // Right shift
   '*',
   0,                                    // Alt
   ' ',                                  // Space bar
   0,                                    // Caps lock
};

bool shift_pressed = false;

void keyboard_handler() {
    uint8_t scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = true;
        outb(0x20, 0x20);
	return;
    } else if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = false;
        outb(0x20, 0x20);
	return;
    }

    if (scancode & 0x80) {
        outb(0x20, 0x20);
	return;
    }

    char c = scancode_to_ascii[scancode];

    if (shift_pressed) {
        c = scancode_to_shift_ascii[scancode]; 
	if (!c) {
	    c = scancode_to_ascii[scancode];
	}
    }

    if (c) {
        input_buffer_put(c);
    }
    outb(0x20, 0x20);
}


__attribute__((used))
void keyboard_handler();
