#ifndef ACPI_H
#define ACPI_H

#include "lib/types.h"

// ACPI-related constants and memory locations
#define EBDA_START 0x80000
#define EBDA_END 0x9FFFF
#define BIOS_ROM_START 0xE0000
#define BIOS_ROM_END 0xFFFFF

// ACPI Signature Definitions
#define ACPI_SIG_RSDP "RSD PTR "
#define ACPI_SIG_RSDT "RSDT"
#define ACPI_SIG_MADT "APIC"
#define ACPI_SIG_FADT "FACP"

// ACPI Table Entry Types
#define ACPI_MADT_LOCAL_APIC 0

// ACPI Table Parsing Status
typedef enum {
    ACPI_SUCCESS = 0,
    ACPI_ERROR_RSDP_NOT_FOUND,
    ACPI_ERROR_RSDT_INVALID,
    ACPI_ERROR_TABLE_NOT_FOUND,
    ACPI_ERROR_TABLE_CHECKSUM
} acpi_status;

// Generic Address Structure (moved to the top)
struct acpi_generic_address {
    uint8_t address_space;
    uint8_t bit_width;
    uint8_t bit_offset;
    uint8_t access_size;
    uint64_t address;
} __attribute__((packed));

// Common ACPI Table Header
struct acpi_header {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

// Root System Description Pointer
struct acpi_rsdp {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

// Root System Description Table
struct acpi_rsdt {
    struct acpi_header header;
    uint32_t table_pointers[];
} __attribute__((packed));

// MADT Entry Structure
struct acpi_madt_entry {
    uint8_t type;
    uint8_t length;
} __attribute__((packed));

// Local APIC Entry
struct acpi_madt_local_apic {
    struct acpi_madt_entry entry;
    uint8_t processor_id;
    uint8_t apic_id;
    uint32_t flags;
} __attribute__((packed));

// Multiple APIC Description Table
struct acpi_madt {
    struct acpi_header header;
    uint32_t local_apic_address;
    uint32_t flags;
    uint8_t entries[];
} __attribute__((packed));

// FADT (Fixed ACPI Description Table)
struct acpi_fadt {
    struct acpi_header header;
    uint32_t firmware_ctrl;
    uint32_t dsdt;
    uint8_t reserved;
    uint8_t preferred_pm_profile;
    uint16_t sci_interrupt;
    uint32_t smi_cmd_port;
    uint8_t acpi_enable;
    uint8_t acpi_disable;
    uint8_t s4_bios_req;
    uint8_t pstate_control;
    uint32_t pm1a_evt_blk;
    uint32_t pm1b_evt_blk;
    uint32_t pm1a_ctrl_blk;
    uint32_t pm1b_ctrl_blk;
    uint32_t pm2_ctrl_blk;
    uint32_t pm_tmr_blk;
    uint32_t gpe0_blk;
    uint32_t gpe1_blk;
    uint8_t pm1_evt_len;
    uint8_t pm1_ctrl_len;
    uint8_t pm2_ctrl_len;
    uint8_t pm_tmr_len;
    uint8_t gpe0_blk_len;
    uint8_t gpe1_blk_len;
    uint8_t gpe1_base;
    uint8_t cstate_control;
    uint16_t worst_c2_latency;
    uint16_t worst_c3_latency;
    uint16_t flush_size;
    uint16_t flush_stride;
    uint8_t duty_offset;
    uint8_t duty_width;
    uint8_t day_alarm;
    uint8_t month_alarm;
    uint8_t century;
    uint16_t boot_arch_flags;
    uint8_t reserved2;
    uint32_t flags;
    struct acpi_generic_address reset_reg;
    uint8_t reset_value;
    uint8_t reserved3[3];
    uint64_t x_firmware_ctrl;
    uint64_t x_dsdt;
    struct acpi_generic_address x_pm1a_evt_blk;
    struct acpi_generic_address x_pm1b_evt_blk;
    struct acpi_generic_address x_pm1a_ctrl_blk;
    struct acpi_generic_address x_pm1b_ctrl_blk;
    struct acpi_generic_address x_pm2_ctrl_blk;
    struct acpi_generic_address x_pm_tmr_blk;
    struct acpi_generic_address x_gpe0_blk;
    struct acpi_generic_address x_gpe1_blk;
} __attribute__((packed));

// ACPI Subsystem Structure
struct acpi_subsystem {
    struct acpi_rsdp* rsdp;
    struct acpi_rsdt* rsdt;
    struct acpi_madt* madt;
    struct acpi_fadt* fadt;
    
    uint32_t local_apic_address;
};

// Global ACPI Subsystem Declaration
extern struct acpi_subsystem acpi_system;

// Function Prototypes
acpi_status acpi_init(void);
uint8_t acpi_checksum(void* ptr, size_t length);
struct acpi_rsdp* acpi_find_rsdp(void);
struct acpi_header* acpi_find_table(const char* signature);
void acpi_parse_madt(void);
void acpi_parse_fadt(void);

#endif // ACPI_H