#include "lapic.h"
#include "lib/io.h"

static struct lapic* lapic;

void lapic_init() {
    lapic = (struct lapic*)LAPIC_BASE_ADDRESS;

    // Disable LAPIC
    outl(lapic->reg + LAPIC_REG_SPURIOUS, 0x00000000);

    // Set the LAPIC ID and version
    uint32_t id = inl(lapic->reg + LAPIC_REG_ID);
    uint32_t version = inl(lapic->reg + LAPIC_REG_VERSION);

    // Configure the LAPIC (set spurious interrupt vector, enable LAPIC)
    outl(lapic->reg + LAPIC_REG_SPURIOUS, 0x000000FF | 0x00000001); // Enable LAPIC
}

void lapic_send_eoi() {
    // Send End Of Interrupt (EOI) signal to LAPIC
    outl(lapic->reg + LAPIC_REG_EOI, 0);
}

void lapic_set_timer(uint32_t timer_value) {
    // Set the timer value (example implementation)
    outl(lapic->reg + LAPIC_REG_LVT_TIMER, timer_value);
}