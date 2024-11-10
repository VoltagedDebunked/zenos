#include "pmm.h"
#include "lib/types.h"

#define PAGE_SIZE 4096 // Size of a page
#define MAX_PAGES 1024 // Maximum number of pages

static uint8_t memory_pool[MAX_PAGES * PAGE_SIZE]; // Memory pool
static bool page_allocated[MAX_PAGES]; // Page allocation table

void pmm_init(uint32_t total_memory) {
    // Initialize memory pool and allocation table
    uint32_t total_pages = total_memory / PAGE_SIZE;
    if (total_pages > MAX_PAGES) {
        total_pages = MAX_PAGES;
    }
    for (uint32_t i = 0; i < total_pages; i++) {
        page_allocated[i] = false; // Mark all pages as free
    }
}

void* pmm_alloc(size_t size) {
    // Calculate number of pages needed
    size_t pages_needed = (size + PAGE_SIZE - 1) / PAGE_SIZE;

    // Find free pages
    for (size_t i = 0; i < MAX_PAGES; i++) {
        if (!page_allocated[i]) {
            size_t j;
            for (j = 0; j < pages_needed; j++) {
                if (i + j >= MAX_PAGES || page_allocated[i + j]) {
                    break; // Not enough contiguous free pages
                }
            }
            if (j == pages_needed) {
                // Mark pages as allocated
                for (size_t k = 0; k < pages_needed; k++) {
                    page_allocated[i + k] = true;
                }
                return (void*)&memory_pool[i * PAGE_SIZE]; // Return pointer to allocated memory
            }
        }
    }
    return NULL; // No available memory
}

void pmm_free(void* ptr) {
    if (ptr < (void*)memory_pool || ptr >= (void*)&memory_pool[MAX_PAGES * PAGE_SIZE]) {
        return; // Pointer out of bounds
    }
    size_t index = ((uint8_t*)ptr - memory_pool) / PAGE_SIZE;
    page_allocated[index] = false; // Mark page as free
}