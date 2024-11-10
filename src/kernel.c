#include "kernel.h"
#include "lib/vga.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"

void kernel_main() {
    gdt_init();
    idt_init();
    pic_init();
    print("Kernel loaded.");
    while(1) {
    }
}