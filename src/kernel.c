#include "kernel.h"
#include "lib/vga.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "pmm.h"
#include "vmm.h"
#include "acpi.h"
#include "ioapic.h"
#include "lapic.h"
#include "vfs.h"

void kernel_main() {
    vga_init();
    gdt_init();
    idt_init();
    pic_init();
    pmm_init(1024 * 1024 * 10);
    acpi_init();
    ioapic_init();
    lapic_init();
    vfs_init();
    vmm_init();
    print("Kernel loaded.");
    while(1) {
    }
}