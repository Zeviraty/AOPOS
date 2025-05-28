#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt_entry {
    uint16_t limit_low;     // Lower 16 bits of limit
    uint16_t base_low;      // Lower 16 bits of base
    uint8_t  base_middle;   // Next 8 bits of base
    uint8_t  access;        // Access flags
    uint8_t  granularity;   // Granularity + high limit bits
    uint8_t  base_high;     // High 8 bits of base
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void gdt_install();

#endif

