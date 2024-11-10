#ifndef LAPIC_H
#define LAPIC_H

#include "lib/types.h"

// LAPIC registers
#define LAPIC_BASE_ADDRESS        0xFEE00000
#define LAPIC_REG_ID              0x20
#define LAPIC_REG_VERSION         0x30
#define LAPIC_REG_SPURIOUS        0x0F0
#define LAPIC_REG_EOI             0x0B0
#define LAPIC_REG_TASK_PRIORITY   0x080
#define LAPIC_REG_LVT_TIMER       0x320
#define LAPIC_REG_LVT_LINT0       0x350
#define LAPIC_REG_LVT_LINT1       0x360
#define LAPIC_REG_LVT_ERROR       0x370
#define LAPIC_REG_ICR             0x300

// LAPIC structure
struct lapic {
    uint32_t reg;   // Register address
    uint32_t data;  // Data register
};

// Function prototypes
void lapic_init();
void lapic_send_eoi();
void lapic_set_timer(uint32_t timer_value);

#endif // LAPIC_H