#include "pic.h"
#include "lib/io.h"  // You may need to create this for I/O operations

void pic_init() {
    // Start initialization sequence
    outb(PIC1_COMMAND, ICW1);
    outb(PIC2_COMMAND, ICW1);

    // Set offset for master and slave PIC
    outb(PIC1_DATA, ICW2);
    outb(PIC2_DATA, ICW2 + 8);  // Slave PIC offset is 8 higher

    // Tell master PIC that there is a slave PIC at IRQ2
    outb(PIC1_DATA, ICW3);
    outb(PIC2_DATA, ICW3);

    // Set 8086 mode
    outb(PIC1_DATA, ICW4);
    outb(PIC2_DATA, ICW4);

    // Mask all interrupts
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, 0x20);  // Send EOI to slave PIC
    }
    outb(PIC1_COMMAND, 0x20);      // Send EOI to master PIC
}