#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 256

// 32-bit IDT entry structure
struct idt_entry {
    uint16_t isr_low;      // Lower 16 bits of ISR address
    uint16_t kernel_cs;    // Kernel code segment selector
    uint8_t  reserved;     // Set to 0
    uint8_t  attributes;   // Type and attributes
    uint16_t isr_high;     // Higher 16 bits of ISR address
} __attribute__((packed));

struct idtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Function declarations
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_init(void);

#endif

