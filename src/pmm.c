#include "pmm.h"
#include "lib/types.h"
#include "lib/string.h"
#include "lib/memory.h"

#define PAGE_SIZE 4096
#define MAX_PAGES 1024
#define MAX_CACHES 16

// Slab structure
typedef struct slab {
    void* start;           // Start of memory for this slab
    uint16_t free_objects; // Number of free objects
    uint16_t total_objects;// Total objects in this slab
    bool* bitmap;          // Allocation bitmap
    struct slab* next;     // Next slab in the cache
} slab_t;

// Cache structure
typedef struct {
    size_t object_size;    // Size of objects in this cache
    size_t objects_per_slab; // Number of objects per slab
    slab_t* slab_list;     // List of slabs
} cache_t;

static uint8_t memory_pool[MAX_PAGES * PAGE_SIZE];
static bool page_allocated[MAX_PAGES];
static cache_t caches[MAX_CACHES];
static size_t num_caches = 0;

// Create a new cache for a specific object size
cache_t* create_cache(size_t size) {
    if (num_caches >= MAX_CACHES) {
        return NULL; // No more cache slots
    }

    cache_t* cache = &caches[num_caches++];
    cache->object_size = size;
    cache->objects_per_slab = PAGE_SIZE / size;
    cache->slab_list = NULL;

    return cache;
}

// Allocate a new slab for a cache
slab_t* allocate_slab(cache_t* cache) {
    // Find a free page range
    for (size_t i = 0; i < MAX_PAGES; i++) {
        size_t pages_needed = 1; // One page per slab
        
        // Check if pages are free
        bool can_allocate = true;
        for (size_t j = 0; j < pages_needed; j++) {
            if (page_allocated[i + j]) {
                can_allocate = false;
                break;
            }
        }

        if (can_allocate) {
            // Mark pages as allocated
            for (size_t j = 0; j < pages_needed; j++) {
                page_allocated[i + j] = true;
            }

            // Create new slab
            slab_t* slab = (slab_t*)malloc(sizeof(slab_t));
            slab->start = &memory_pool[i * PAGE_SIZE];
            slab->total_objects = cache->objects_per_slab;
            slab->free_objects = slab->total_objects;
            slab->bitmap = (bool*)malloc(slab->total_objects * sizeof(bool));
            memset(slab->bitmap, 0, slab->total_objects * sizeof(bool));
            slab->next = cache->slab_list;
            
            // Add slab to cache
            cache->slab_list = slab;

            return slab;
        }
    }

    return NULL; // No memory available
}

// Find a cache for a specific size
cache_t* find_cache(size_t size) {
    for (size_t i = 0; i < num_caches; i++) {
        if (caches[i].object_size >= size) {
            return &caches[i];
        }
    }
    return NULL;
}

void pmm_init(uint32_t total_memory) {
    // Initialize page allocation table
    uint32_t total_pages = total_memory / PAGE_SIZE;
    if (total_pages > MAX_PAGES) {
        total_pages = MAX_PAGES;
    }
    
    for (uint32_t i = 0; i < total_pages; i++) {
        page_allocated[i] = false;
    }

    // Create some default caches
    create_cache(32);
    create_cache(64);
    create_cache(128);
    create_cache(256);
    create_cache(512);
    create_cache(1024);
}

void* pmm_alloc(size_t size) {
    // Find or create an appropriate cache
    cache_t* cache = find_cache(size);
    if (!cache) {
        // If no existing cache, create a new one
        cache = create_cache(size);
    }

    if (!cache) {
        return NULL; // Failed to create cache
    }

    // Try to find a slab with free space
    slab_t* slab = cache->slab_list;
    while (slab) {
        if (slab->free_objects > 0) {
            // Find a free object
            for (uint16_t i = 0; i < slab->total_objects; i++) {
                if (!slab->bitmap[i]) {
                    slab->bitmap[i] = true;
                    slab->free_objects--;
                    
                    return (void*)((uint8_t*)slab->start + (i * cache->object_size));
                }
            }
        }
        slab = slab->next;
    }

    // No free space, allocate a new slab
    slab = allocate_slab(cache);
    if (!slab) {
        return NULL; // Out of memory
    }

    // Mark first object as allocated
    slab->bitmap[0] = true;
    slab->free_objects--;
    return slab->start;
}

void pmm_free(void* ptr) {
    // Find which cache and slab the pointer belongs to
    for (size_t cache_idx = 0; cache_idx < num_caches; cache_idx++) {
        cache_t* cache = &caches[cache_idx];
        slab_t* slab = cache->slab_list;

        while (slab) {
            // Check if pointer is within this slab
            if (ptr >= slab->start && 
                ptr < (void*)((uint8_t*)slab->start + (PAGE_SIZE))) {
                
                // Calculate object index
                size_t index = ((uint8_t*)ptr - (uint8_t*)slab->start) / cache->object_size;
                
                // Mark as free
                if (slab->bitmap[index]) {
                    slab->bitmap[index] = false;
                    slab->free_objects++;
                }
                return;
            }
            slab = slab->next;
        }
    }
}