#include "kernel.h"
#include "lib/vga.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "pmm.h"
#include "vmm.h"
#include "acpi.h"

void kernel_main() {
    gdt_init();
    idt_init();
    pic_init();
    pmm_init(1024 * 1024 * 10);
    vmm_init();
    acpi_init();
    print("Kernel loaded.");
    while(1) {
    }
}