#include "vmm.h"
#include "pmm.h"
#include "lib/string.h"
#include "lib/types.h"

/* Constants for virtual memory management */
#define PAGE_SIZE           4096
#define PAGE_DIR_ENTRIES   1024
#define PAGE_TABLE_ENTRIES 1024
#define KERNEL_BASE       0xC0000000
#define KERNEL_PDI       (KERNEL_BASE >> 22)  // Page Directory Index for kernel
#define PAGE_ALIGN_DOWN(x) ((x) & ~(PAGE_SIZE - 1))
#define PAGE_ALIGN_UP(x)   (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define GET_PD_INDEX(addr) ((uint32_t)(addr) >> 22)
#define GET_PT_INDEX(addr) (((uint32_t)(addr) >> 12) & 0x3FF)
#define PAGE_PRESENT    0x1
#define PAGE_WRITE     0x2
#define PAGE_USER      0x4
#define PAGES_PER_MB   256
#define EARLY_MAPPED_MB 16  // Map first 16MB during boot

/* Global page directory and tables */
static struct page_directory_entry page_directory[PAGE_DIR_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
static struct page_table_entry page_tables[PAGE_DIR_ENTRIES][PAGE_TABLE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));

/* Static helper function declarations */
static void flush_tlb_single(void* addr);
static void map_page(uint32_t physical, uint32_t virtual, uint32_t flags);
static void unmap_page(uint32_t virtual);
static bool is_page_aligned(uint32_t addr);

/* Helper function implementations */
static void flush_tlb_single(void* addr) {
    asm volatile("invlpg (%0)" : : "r" (addr) : "memory");
}

static void flush_tlb_full(void) {
    asm volatile(
        "mov %%cr3, %%eax\n"
        "mov %%eax, %%cr3"
        : : : "eax", "memory"
    );
}

static bool is_page_aligned(uint32_t addr) {
    return (addr & (PAGE_SIZE - 1)) == 0;
}

static void map_page(uint32_t physical, uint32_t virtual, uint32_t flags) {
    uint32_t pd_index = GET_PD_INDEX(virtual);
    uint32_t pt_index = GET_PT_INDEX(virtual);

    // Ensure page directory entry is present
    page_directory[pd_index].present = 1;
    page_directory[pd_index].rw = !!(flags & PAGE_WRITE);
    page_directory[pd_index].user = !!(flags & PAGE_USER);
    page_directory[pd_index].frame = (uint32_t)&page_tables[pd_index] >> 12;

    // Map the page
    page_tables[pd_index][pt_index].present = 1;
    page_tables[pd_index][pt_index].rw = !!(flags & PAGE_WRITE);
    page_tables[pd_index][pt_index].user = !!(flags & PAGE_USER);
    page_tables[pd_index][pt_index].frame = physical >> 12;

    flush_tlb_single((void*)virtual);
}

static void unmap_page(uint32_t virtual) {
    uint32_t pd_index = GET_PD_INDEX(virtual);
    uint32_t pt_index = GET_PT_INDEX(virtual);

    if (page_directory[pd_index].present) {
        page_tables[pd_index][pt_index].present = 0;
        page_tables[pd_index][pt_index].frame = 0;
        flush_tlb_single((void*)virtual);
    }
}

void vmm_init(void) {
    // Zero out all page directory entries first
    for (uint32_t i = 0; i < PAGE_DIR_ENTRIES; i++) {
        page_directory[i].present = 0;
        page_directory[i].rw = 0;
        page_directory[i].user = 0;
        page_directory[i].frame = 0;
    }

    // Identity map the first 16MB for boot essentials 
    // This covers kernel, GDT, IDT, and early boot structures
    for (uint32_t mb = 0; mb < EARLY_MAPPED_MB; mb++) {
        uint32_t startAddr = mb * 1024 * 1024;
        for (uint32_t page = 0; page < PAGES_PER_MB; page++) {
            uint32_t physAddr = startAddr + (page * PAGE_SIZE);
            map_page(physAddr, physAddr, PAGE_PRESENT | PAGE_WRITE); // Kernel level, read-write
        }
    }

    // Map the kernel to higher half
    // Use the same first 16MB but map it to KERNEL_BASE
    for (uint32_t mb = 0; mb < EARLY_MAPPED_MB; mb++) {
        uint32_t startAddr = mb * 1024 * 1024;
        for (uint32_t page = 0; page < PAGES_PER_MB; page++) {
            uint32_t physAddr = startAddr + (page * PAGE_SIZE);
            uint32_t virtAddr = KERNEL_BASE + startAddr + (page * PAGE_SIZE);
            map_page(physAddr, virtAddr, PAGE_PRESENT | PAGE_WRITE); // Kernel level, read-write
        }
    }

    // Load the page directory
    asm volatile("mov %0, %%cr3" : : "r"(&page_directory) : "memory");

    // Enable paging and write protect
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging (PG bit)
    asm volatile("mov %0, %%cr0" : : "r"(cr0) : "memory");

    // Ensure TLB is fully up to date
    flush_tlb_full();
}

void* vmm_alloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    // Calculate number of pages needed
    size_t num_pages = (PAGE_ALIGN_UP(size)) / PAGE_SIZE;
    
    // Search for consecutive free pages
    // Start after the early mapped area
    uint32_t base_page = EARLY_MAPPED_MB * PAGES_PER_MB;
    
    for (uint32_t pd_idx = base_page / PAGE_TABLE_ENTRIES; pd_idx < KERNEL_PDI; pd_idx++) {
        for (uint32_t pt_idx = 0; pt_idx < PAGE_TABLE_ENTRIES; pt_idx++) {
            bool found = true;
            
            // Check if we have enough consecutive free pages
            for (size_t i = 0; i < num_pages; i++) {
                uint32_t curr_pt_idx = pt_idx + i;
                if (curr_pt_idx >= PAGE_TABLE_ENTRIES) {
                    found = false;
                    break;
                }
                
                if (page_tables[pd_idx][curr_pt_idx].present) {
                    found = false;
                    break;
                }
            }
            
            if (found) {
                // Allocate the pages
                for (size_t i = 0; i < num_pages; i++) {
                    void* phys_page = pmm_alloc(PAGE_SIZE);
                    if (!phys_page) {
                        // Cleanup on failure
                        for (size_t j = 0; j < i; j++) {
                            unmap_page((pd_idx * PAGE_SIZE * 1024) + ((pt_idx + j) * PAGE_SIZE));
                            pmm_free((void*)(page_tables[pd_idx][pt_idx + j].frame << 12));
                        }
                        return NULL;
                    }
                    
                    uint32_t virt_addr = (pd_idx * PAGE_SIZE * 1024) + ((pt_idx + i) * PAGE_SIZE);
                    map_page((uint32_t)phys_page, virt_addr, PAGE_PRESENT | PAGE_WRITE | PAGE_USER);
                }
                
                return (void*)(pd_idx * PAGE_SIZE * 1024 + pt_idx * PAGE_SIZE);
            }
            
            // Skip to next potential starting point
            pt_idx += num_pages - 1;
        }
    }
    
    return NULL;
}

void vmm_free(void* ptr) {
    if (!ptr || (uint32_t)ptr >= KERNEL_BASE || !is_page_aligned((uint32_t)ptr)) {
        return;
    }

    uint32_t pd_idx = GET_PD_INDEX((uint32_t)ptr);
    uint32_t pt_idx = GET_PT_INDEX((uint32_t)ptr);

    // Don't free early mapped memory
    if (pd_idx < (EARLY_MAPPED_MB * PAGES_PER_MB / PAGE_TABLE_ENTRIES)) {
        return;
    }

    if (page_directory[pd_idx].present) {
        if (page_tables[pd_idx][pt_idx].present) {
            void* phys_addr = (void*)(page_tables[pd_idx][pt_idx].frame << 12);
            pmm_free(phys_addr);
            unmap_page((uint32_t)ptr);
        }
    }
}