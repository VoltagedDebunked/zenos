#ifndef IOAPIC_H
#define IOAPIC_H

#include "lib/types.h"

// IOAPIC registers
#define IOAPIC_REG_VERSION      0x01
#define IOAPIC_REG_ID           0x00
#define IOAPIC_REG_REDIRECTION   0x10

// IOAPIC base address (to be set during initialization)
#define IOAPIC_BASE_ADDRESS     0xFEC00000

// IOAPIC structure
struct ioapic {
    uint32_t reg;   // Register address
    uint32_t data;  // Data register
};

// Function prototypes
void ioapic_init();
void ioapic_set_irq(uint8_t irq, uint8_t vector);
void ioapic_send_eoi(uint8_t irq);

#endif // IOAPIC_H