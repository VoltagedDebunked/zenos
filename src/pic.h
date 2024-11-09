#ifndef PIC_H
#define PIC_H

#include "lib/types.h"

// PIC ports
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

// Initialization command words
#define ICW1 0x11  // ICW1: Edge trigger mode, cascade mode
#define ICW2 0x20  // ICW2: Offset for master PIC
#define ICW3 0x00  // ICW3: No slave PIC
#define ICW4 0x01  // ICW4: 8086/8088 mode

// Function prototypes
void pic_init();
void pic_send_eoi(uint8_t irq);

#endif // PIC_H