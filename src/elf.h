#include "lib/types.h"

// ELF Header
struct elf_header {
    uint32_t magic;        // Magic number
    uint8_t  bit_class;    // 1 = 32-bit, 2 = 64-bit
    uint8_t  endianness;   // 1 = little-endian, 2 = big-endian
    uint8_t  version;      // ELF version
    uint8_t  os_abi;       // OS ABI
    uint8_t  abi_version;  // ABI version
    uint8_t  padding[7];   // Padding bytes
    uint16_t type;         // Object file type
    uint16_t machine;      // Architecture
    uint32_t version2;     // Version
    uint32_t entry;        // Entry point address
    uint32_t ph_offset;    // Program header table offset
    uint32_t sh_offset;    // Section header table offset
    uint32_t flags;        // Processor-specific flags
    uint16_t eh_size;      // ELF header size
    uint16_t ph_size;      // Program header size
    uint16_t ph_count;     // Number of program headers
    uint16_t sh_size;      // Section header size
    uint16_t sh_count;     // Number of section headers
    uint16_t sh_str_index; // Section header string table index
} __attribute__((packed));

// Program Header
struct elf_program_header {
    uint32_t type;   // Segment type
    uint32_t offset; // Segment file offset
    uint32_t vaddr;  // Segment virtual address
    uint32_t paddr;  // Segment physical address
    uint32_t file_size; // Size of segment in file
    uint32_t mem_size;  // Size of segment in memory
    uint32_t flags;      // Segment flags
    uint32_t align;      // Segment alignment
} __attribute__((packed));