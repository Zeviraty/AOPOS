#include "idt.h"
#include "io.h"

static struct idt_entry idt[IDT_MAX_DESCRIPTORS];
static struct idtr idtr;

extern void* isr_stub_table[];
extern void keyboard_handler_stub(void);

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    uint32_t isr_addr = (uint32_t)isr;

    volatile struct idt_entry* desc = &idt[vector];
    desc->isr_low    = isr_addr & 0xFFFF;
    desc->kernel_cs  = 0x08;  // Your kernel code segment selector from GDT
    desc->reserved   = 0;
    desc->attributes = flags;
    desc->isr_high   = (isr_addr >> 16) & 0xFFFF;
}

void idt_init(void) {
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
    idtr.base = (uint32_t)&idt;
    idtr.limit = sizeof(struct idt_entry) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t i = 0; i < 32; i++) {
        idt_set_descriptor(i, isr_stub_table[i], 0x8E);
    }

    idt_set_descriptor(33, keyboard_handler_stub, 0x8E);

    __asm__ volatile ("lidt %0" : : "m"(idtr));
}
