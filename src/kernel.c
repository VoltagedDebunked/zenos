#include "kernel.h"
#include "gdt.h"
#include "idt.h"

void kernel_main() {
    gdt_init();
    idt_init();
    while(1) {
    }
}