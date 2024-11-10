#include "vmm.h"
#include "pmm.h"

#define PAGE_SIZE 4096
#define MAX_PAGES 1024
#define KERNEL_BASE 0xC0000000

static struct page_directory_entry page_directory[MAX_PAGES];
static struct page_table_entry page_tables[MAX_PAGES][1024]; // 1024 Entries per page table

void vmm_init() {
    // Initialize the page directory and tables
    for (int i = 0; i < MAX_PAGES; i++) {
        page_directory[i].present = 0;
        page_directory[i].rw = 1; // Allow read/write
        page_directory[i].user = 0; // Kernel mode
        page_directory[i].accessed = 0;
        page_directory[i].frame = 0;

        for (int j = 0; j < 1024; j++) {
            page_tables[i][j].present = 0;
            page_tables[i][j].rw = 1; // Allow read/write
            page_tables[i][j].user = 0; // Kernel mode
            page_tables[i][j].accessed = 0;
            page_tables[i][j].dirty = 0;
            page_tables[i][j].frame = 0;
        }
    }
}

void* vmm_alloc(size_t size) {
    // This function should allocate virtual memory
    size_t pages_needed = (size + PAGE_SIZE - 1) / PAGE_SIZE;

    for (size_t i = 0; i < MAX_PAGES; i++) {
        if (!page_directory[i].present) {
            // Allocate a new page table
            page_directory[i].present = 1;
            page_directory[i].frame = (uint32_t)&page_tables[i] >> 12; // Store frame number

            for (size_t j = 0; j < pages_needed; j++) {
                if (!page_tables[i][j].present) {
                    // Allocate a physical page using PMM
                    void* physical_page = pmm_alloc(PAGE_SIZE);
                    if (physical_page == NULL) {
                        return NULL; // No available physical memory
                    }

                    // Map the physical page to the virtual address
                    page_tables[i][j].present = 1;
                    page_tables[i][j].frame = (uint32_t)physical_page >> 12; // Store frame number
                }
            }
            return (void*)(i * PAGE_SIZE); // Return the virtual address
        }
    }
    return NULL; // No available virtual memory
}

void vmm_free(void* ptr) {
    // This function should free virtual memory
    uint32_t virtual_address = (uint32_t)ptr;
    size_t page_index = virtual_address / PAGE_SIZE;

    // Check if the page is within the kernel space
    if (virtual_address >= KERNEL_BASE) {
        return; // Do not free kernel pages
    }

    // Free the corresponding page table entry
    if (page_directory[page_index].present) {
        for (size_t j = 0; j < 1024; j ) {
            if (page_tables[page_index][j].present) {
                // Free the physical page using PMM
                pmm_free((void*)(page_tables[page_index][j].frame << 12));
                page_tables[page_index][j].present = 0; // Unmap the page
            }
        }
        page_directory[page_index].present = 0; // Unmap the page directory entry
    }
}