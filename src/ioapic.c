#include "ioapic.h"
#include "lib/io.h"

static struct ioapic* ioapic;

void ioapic_init() {
    ioapic = (struct ioapic*)IOAPIC_BASE_ADDRESS;

    // Set the IOAPIC version and ID
    uint32_t version = inb(ioapic->reg + IOAPIC_REG_VERSION);
    uint32_t id = inb(ioapic->reg + IOAPIC_REG_ID);

    
}

void ioapic_set_irq(uint8_t irq, uint8_t vector) {
    // Set the redirection table entry for the specified IRQ
    outb(ioapic->reg + IOAPIC_REG_REDIRECTION + (irq * 2), vector);
}

void ioapic_send_eoi(uint8_t irq) {
    // Send EOI to the IOAPIC for the specified IRQ
    outb(ioapic->reg + IOAPIC_REG_REDIRECTION + (irq * 2), 0x20);
}