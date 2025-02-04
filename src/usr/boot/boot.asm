format elf

public main
extrn KERNEL_MAIN

;; set flags to 0
FLAGS = 0

;; set magic number to 0x1BADB002 to identified by bootloader 
MAGIC = 0x1BADB002

;; set the checksum
CHECKSUM = -(MAGIC + FLAGS)

;; set multiboot enabled
section '.multiboot' executable
	;; define type to long for each data defined as above
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section '_CODE' executable

main:

	;; assign current stack pointer location to stackTop
	lea esp, [stackTop]

	;; call the kernel main source
	call KERNEL_MAIN

	cli


	;; put system in infinite loop
hltLoop:
	hlt
	jmp hltLoop

size: rb $ - main


section '_BSS' writeable

	;; set the stack bottom 
	stackBottom:

	;; define the maximum size of stack to 512 bytes
	rb 512

	;; set the stack top which grows from higher to lower
	stackTop: