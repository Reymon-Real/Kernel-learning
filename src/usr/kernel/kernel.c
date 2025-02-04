#include <kernel.h>

void KERNEL_MAIN() {
	InitTerminal();
	DisplayIntro();

	performArithmatic();
	stringOperations();
	printNewLine();

	printColoredString("   -:  Linked List elements are  :-", VGA_COLOR_LIGHT_GREEN);
	printNewLine();
	
	performLinkedListOperations();
}