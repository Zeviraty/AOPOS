#ifndef PIC_H
#define PIC_H

#include <stdint.h>

// I/O port addresses for the PICs
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

// Function declarations
void pic_remap(void);
void pic_unmask_irq1(void);
void pic_send_eoi(void);

#endif // PIC_H

