# Compiler settings
CC = gcc
AS = nasm
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -c
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T scripts/linker.ld

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source files
C_SRCS = $(wildcard $(SRC_DIR)/*.c)
ASM_SRCS = $(wildcard $(SRC_DIR)/*.asm)

# Object files
C_OBJS = $(C_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
ASM_OBJS = $(ASM_SRCS:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.o)

# Default target
all: zenos.iso

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p iso/boot/grub

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# Compile Assembly files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	$(AS) $(ASFLAGS) $< -o $@

# Link kernel
kernel.bin: $(C_OBJS) $(ASM_OBJS)
	ld $(LDFLAGS) -o iso/boot/kernel.bin $(C_OBJS) $(ASM_OBJS)

# Create ISO
zenos.iso: $(BUILD_DIR) kernel.bin
	cp config/grub.cfg iso/boot/grub/
	grub-mkrescue -o zenos.iso iso

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	rm -rf iso
	rm -f zenos.iso

# Run in QEMU
run: zenos.iso
	qemu-system-i386 -cdrom zenos.iso

.PHONY: all clean run