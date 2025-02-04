# SPDX-License: GPL-2

# Special files
EDU_BIN		:= iso/kernel/EduOS.bin
EDU_ISO		:= EduOS.iso
LINKER 		:= linker/linker.ld

# Search files
SOURCE_C	:= $(shell find src -type f -name '*.c')
SOURCE_ASM	:= $(shell find src -type f -name '*.asm')

OBJECT_C	:= $(patsubst src/%.c,bin/%.o,$(SOURCE_C))
OBJECT_ASM	:= $(patsubst src/%.asm,bin/%.o,$(SOURCE_ASM))

# Tools
CC := gcc
AS := fasm
LD := gold

QEMU := qemu-system-i386

# Tools flags
CFLAGS	:= -Wall -Wextra -Werror -m32 -I include -ffreestanding -fno-builtin -nostdlib -nostartfiles
ASFLAGS := -p 10
LDFLAGS	:= -m elf_i386 -T $(LINKER)

QEMUFLAGS := -cdrom

# Rules

all: $(OBJECT_ASM) $(OBJECT_C) $(EDU_ISO)

run: $(EDU_ISO)
	$(QEMU) $(QEMUFLAGS) $<

# Files

$(EDU_BIN): $(OBJECT_C) $(OBJECT_ASM)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $^

$(EDU_ISO): $(EDU_BIN)
	grub-mkrescue -o $@ iso

# Clean rules

clean:
	$(RM) $(OBJECT_C) $(OBJECT_ASM) $(EDU_BIN) $(EDU_ISO)

# .PHONY rules
.PHONY:	\
		$(CC) $(AS) $(LD) \
		$(CFLAGS) $(ASFLAGS) $(LDFLAGS) \
		all clean

# Patrons

bin/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: src/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< $@