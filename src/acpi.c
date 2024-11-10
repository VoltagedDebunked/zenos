#include "acpi.h"
#include "lib/string.h"
#include "lib/types.h"
#include "lib/vga.h"

// WARNING: this code is shit. needs to be fixed.

// Global ACPI Subsystem
struct acpi_subsystem acpi_system = {0};

// Checksum Validation
uint8_t acpi_checksum(void* ptr, size_t length) {
    uint8_t* byte = (uint8_t*)ptr;
    uint8_t sum = 0;
    for (size_t i = 0; i < length; i++) {
        sum += byte[i];
    }
    return sum;
}

// Find RSDP (Root System Description Pointer)
struct acpi_rsdp* acpi_find_rsdp(void) {
    // Search Extended BIOS Data Area
    for (uintptr_t addr = EBDA_START; addr < EBDA_END; addr += 16) {
        struct acpi_rsdp* potential_rsdp = (struct acpi_rsdp*)addr;
        if (strncmp(potential_rsdp->signature, ACPI_SIG_RSDP, 8) == 0) {
            if (acpi_checksum(potential_rsdp, sizeof(struct acpi_rsdp)) == 0) {
                return potential_rsdp;
            }
        }
    }

    // Search BIOS ROM Area
    for (uintptr_t addr = BIOS_ROM_START; addr < BIOS_ROM_END; addr += 16) {
        struct acpi_rsdp* potential_rsdp = (struct acpi_rsdp*)addr;
        if (strncmp(potential_rsdp->signature, ACPI_SIG_RSDP, 8) == 0) {
            if (acpi_checksum(potential_rsdp, sizeof(struct acpi_rsdp)) == 0) {
                return potential_rsdp;
            }
        }
    }

    return NULL; // RSDP not found
}

// Find ACPI Table by Signature
struct acpi_header* acpi_find_table(const char* signature) {
    struct acpi_rsdp* rsdp = acpi_find_rsdp();
    if (!rsdp) return NULL;

    struct acpi_rsdt* rsdt = (struct acpi_rsdt*)(uintptr_t)rsdp->rsdt_address;
    for (size_t i = 0; i < (rsdt->header.length - sizeof(struct acpi_header)) / sizeof(uint32_t); i++) {
        struct acpi_header* table = (struct acpi_header*)(uintptr_t)rsdt->table_pointers[i];
        if (strncmp(table->signature, signature, 4) == 0) {
            if (acpi_checksum(table, table->length) == 0) {
                return table;
            }
        }
    }

    return NULL; // Table not found
}

// ACPI Initialization
acpi_status acpi_init(void) {
    // Initialize ACPI subsystem
    acpi_system.rsdp = acpi_find_rsdp();
    if (!acpi_system.rsdp) {
        return ACPI_ERROR_RSDP_NOT_FOUND;
    }

    acpi_system.rsdt = (struct acpi_rsdt*)(uintptr_t)acpi_system.rsdp->rsdt_address;
    acpi_system.madt = (struct acpi_madt*)acpi_find_table(ACPI_SIG_MADT);
    acpi_system.fadt = (struct acpi_fadt*)acpi_find_table(ACPI_SIG_FADT);

    if (!acpi_system.madt) {
        return ACPI_ERROR_TABLE_NOT_FOUND;
    }

    if (!acpi_system.fadt) {
        return ACPI_ERROR_TABLE_NOT_FOUND;
    }

    return ACPI_SUCCESS; // Initialization successful
}

// Parse MADT (Multiple APIC Description Table)
void acpi_parse_madt(void) {
    // Implementation for parsing MADT entries
    struct acpi_madt* madt = acpi_system.madt;
    uint8_t* entry = madt->entries;

    for (size_t i = 0; i < (madt->header.length - sizeof(struct acpi_header));) {
        struct acpi_madt_entry* madt_entry = (struct acpi_madt_entry*)entry;
        switch (madt_entry->type) {
            case ACPI_MADT_LOCAL_APIC: {
                struct acpi_madt_local_apic* local_apic = (struct acpi_madt_local_apic*)madt_entry;
                print_hex(local_apic->processor_id);
                print_hex(local_apic->apic_id);
                print_hex(local_apic->flags);
                break;
            }
            default:
                break;
        }
        i += madt_entry->length;
        entry += madt_entry->length;
    }
}

// Parse FADT (Fixed ACPI Description Table)
void acpi_parse_fadt(void) {
    // Implementation for parsing FADT
    struct acpi_fadt* fadt = acpi_system.fadt;
    print_hex(fadt->firmware_ctrl);
}