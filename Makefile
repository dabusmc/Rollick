# Toolchain
TOOLCHAIN = ../toolchain/tools/bin
CC = $(TOOLCHAIN)/i686-elf-gcc
AS = $(TOOLCHAIN)/i686-elf-as

# Emulator
QEMU = qemu-system-i386

# Directories
BUILD_DIR = build

# Output
KERNEL = $(BUILD_DIR)/rollick.elf

# Source files
ASM_SOURCES = src/boot/boot.s
C_SOURCES = src/kernel/kernel.c

# Object files
ASM_OBJECTS = $(BUILD_DIR)/boot.o
C_OBJECTS = $(BUILD_DIR)/kernel.o

OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

# Compiler flags
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra \
         -fno-stack-protector -fno-pic

# Linker flags
LDFLAGS = -T linker.ld -ffreestanding -nostdlib \
          -Wl,--build-id=none

.PHONY: all clean run

# Default target
all: $(KERNEL)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Assemble boot.s
$(BUILD_DIR)/boot.o: $(ASM_SOURCES) | $(BUILD_DIR)
	$(AS) --32 $< -o $@

# Compile kernel.c
$(BUILD_DIR)/kernel.o: $(C_SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel
$(KERNEL): $(OBJECTS) linker.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) -lgcc

# Run kernel in QEMU
run: $(KERNEL)
	$(QEMU) -kernel $(KERNEL)

# Remove generated files
clean:
	rm -rf $(BUILD_DIR)
