#include <kernel.h>

int8_t DIGIT_ASCII_CODES[10] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
uint16_t* TERMINAL_BUFFER;
uint32_t VGA_INDEX;


static int Y_INDEX = 1;
uint16_t MEM_SIZE = 0;


static uint16_t VGA_DefaultEntry(unsigned char to_print) {
	return (uint16_t) to_print | (uint16_t)VGA_COLOR_WHITE << 8;
}

static uint16_t VGA_ColoredEntry(unsigned char to_print, uint8_t color) {
	return (uint16_t) to_print | (uint16_t)color << 8;
}


static void Clear_VGA_Buffer(uint16_t **buffer)
{
	for(int i = 0; i<BUFSIZE; i++)
		(*buffer)[i] = '\0';
  Y_INDEX = 1;
  VGA_INDEX = 0;
}


void InitTerminal()
{
  TERMINAL_BUFFER = (uint16_t*) VGA_ADDRESS;
  Clear_VGA_Buffer(&TERMINAL_BUFFER);
}


static int strlen(const char* str)
{
  int length = 0;
  while(str[length])
	length++;
  return length;
}


static void strcat(char *str_1, char *str_2)
{
  int index_1 = strlen(str_1);
  int index_2 = 0;
  while(str_2[index_2]){
	str_1[index_1] = str_2[index_2];
	index_1++;
	index_2++;
  }
  str_1[index_1] = '\0';
}


static void memcpy(char *str_dest, char *str_src)
{
  int index = 0;
  while(str_src[index]){
	str_dest[index] = str_src[index];
	index++;
  }
  str_dest[index] = '\0';
}


static int digitCount(int num)
{
  int count = 0;
  if(num == 0)
	return 1;
  while(num > 0){
	count++;
	num = num/10;
  }
  return count;
}


static void itoa(int num, char *number)
{
  int digit_count = digitCount(num);
  int index = digit_count - 1;
  char x;
  if(num == 0 && digit_count == 1){
	number[0] = '0';
	number[1] = '\0';
  }else{
	while(num != 0){
	  x = num % 10;
	  number[index] = x + '0';
	  index--;
	  num = num / 10;
	}
	number[digit_count] = '\0';
  }
}


static uint8_t IN_B(uint16_t port)
{
  uint8_t ret;
  asm volatile("inb %1, %0" :"=a"(ret) :"Nd"(port) );
  return ret;
}


static char getInputCode() {
  char ch = 0;
  do{
	if(IN_B(0x60) != ch) {
	  ch = IN_B(0x60);
	  if(ch > 0)
		return ch;
	}
  }while(1);
}


void printNewLine()
{
  if(Y_INDEX >= 55){
	Y_INDEX = 0;
	Clear_VGA_Buffer(&TERMINAL_BUFFER);
  }
  VGA_INDEX = 80*Y_INDEX;
  Y_INDEX++;
}

static void printN_NewLine(int n)
{
  for(int i=0;i<n;i++)
	printNewLine();
}


static void printString(char *str)
{
  int index = 0;
  while(str[index]){
	TERMINAL_BUFFER[VGA_INDEX] = VGA_DefaultEntry(str[index]);
	index++;
	VGA_INDEX++;
  }
}

static void printInt(int num)
{
  char str_num[digitCount(num)+1];
  itoa(num, str_num);
  printString(str_num);
}

void printColoredString(char *str, uint8_t color)
{
  int index = 0;
  while(str[index]){
	TERMINAL_BUFFER[VGA_INDEX] = VGA_ColoredEntry(str[index], color);
	index++;
	VGA_INDEX++;
  }
}


static void printCharN(char ch, int n)
{
  int i = 0;
  while(i <= n){
	TERMINAL_BUFFER[VGA_INDEX] = VGA_DefaultEntry(ch);
	i++;
	VGA_INDEX++;
  }
}

/*
static void printColoredCharN(char ch, int n, uint8_t color)
{
  int i = 0;
  while(i <= n){
	TERMINAL_BUFFER[VGA_INDEX] = VGA_ColoredEntry(ch, color);
	i++;
	VGA_INDEX++;
  }
} */


static void printColored_WCharN(uint16_t ch, int n, uint8_t color)
{
  int i = 0;
  while(i <= n){
	TERMINAL_BUFFER[VGA_INDEX] = VGA_ColoredEntry(ch, color);
	i++;
	VGA_INDEX++;
  }
}


//creating a linked list
typedef struct list_node{
  int data;
  struct list_node *next;
}LIST_NODE;


static LIST_NODE *getNewListNode(int data)
{
  LIST_NODE *newnode = (LIST_NODE*)0x00000500 + MEM_SIZE;
  newnode->data = data;
  newnode->next = NULL;
  MEM_SIZE += sizeof(LIST_NODE);
  return newnode;
}

static void addListNode(LIST_NODE **head, int data)
{
  if(*head == NULL){
	*head = getNewListNode(data);
  }else{
	LIST_NODE *current = *head;
	while(current->next != NULL){
	  current = current->next;
	}
	current->next = getNewListNode(data);
  }
}

void displayList(LIST_NODE *head)
{
  while(head != NULL){
	printColoredString("data = ", VGA_COLOR_LIGHT_CYAN); printInt(head->data);
	printColoredString(", current = ", VGA_COLOR_LIGHT_CYAN); printInt((int)head);
	printColoredString(", next = ", VGA_COLOR_LIGHT_CYAN); printInt((int)head->next);
	printNewLine();
	head = head->next;
  }
}


void performArithmatic()
{
  int a=423, b=75, c;

  printCharN(' ', 22);
  printColoredString("32 bit Arithmatic Operations", VGA_COLOR_LIGHT_GREEN);
  printN_NewLine(2);

  printColoredString("  a = ", VGA_COLOR_YELLOW);
  printInt(a);
  printColoredString(", b = ", VGA_COLOR_YELLOW);
  printInt(b);
  printN_NewLine(2);
  
  c = a+b;
  printColoredString("Addition = ", VGA_COLOR_LIGHT_CYAN);
  printInt(c);
  printNewLine();
  
  c = a-b;
  printColoredString("Substraction = ", VGA_COLOR_LIGHT_CYAN);
  printInt(c);
  printNewLine();

  c = a*b;
  printColoredString("Multiplication = ", VGA_COLOR_LIGHT_CYAN);
  printInt(c);
  printNewLine();

  c = a/b;
  printColoredString("Division = ", VGA_COLOR_LIGHT_CYAN);
  printInt(c);
  printNewLine();

  c = a%b;
  printColoredString("Modulus = ", VGA_COLOR_LIGHT_CYAN);
  printInt(c);
  printNewLine();
}


void stringOperations()
{
  char str_1[20] = "Hello", str_2[20] = "World";

  printCharN(' ', 22);
  printColoredString("8 bit String Operations", VGA_COLOR_LIGHT_GREEN);
  printN_NewLine(2);

  printColoredString("  str_1 = ", VGA_COLOR_YELLOW);
  printString(str_1);
  printColoredString(", str_2 = ", VGA_COLOR_YELLOW);
  printString(str_2);
  printN_NewLine(2);

  printColoredString("str_1 length = ", VGA_COLOR_LIGHT_CYAN);
  printInt(strlen(str_1));
  printNewLine();

  strcat(str_1, str_2);
  printColoredString("strcat of str_1 & str_2 = ", VGA_COLOR_LIGHT_CYAN);
  printString(str_1);
  printNewLine();

  memcpy(str_1, str_2);
  printColoredString("memcpy of str_2 to str_1 = ", VGA_COLOR_LIGHT_CYAN);
  printString(str_1);
  printNewLine();

}

void performLinkedListOperations()
{
  LIST_NODE *head = NULL;
  addListNode(&head, 10);
  addListNode(&head, 20);
  addListNode(&head, 30);
  addListNode(&head, 40);
  addListNode(&head, 50);
  displayList(head);
}


void DisplayIntro()
{

  printColored_WCharN(2481,79,VGA_COLOR_RED);
  printNewLine();
  printColored_WCharN(2483,79,VGA_COLOR_LIGHT_GREEN);
  printN_NewLine(4);
  VGA_INDEX += 30;
  printColoredString("My Operating System", VGA_COLOR_YELLOW);
  printN_NewLine(6);
  VGA_INDEX += 28;
  printColoredString("Kernel in C - 16/32 Bit", VGA_COLOR_WHITE);
  printN_NewLine(6);
  VGA_INDEX += 25;
  printColoredString("! Press any key to move next !", VGA_COLOR_BROWN);
  printN_NewLine(6);
  printColored_WCharN(2483,79, VGA_COLOR_LIGHT_GREEN);
  printNewLine();
  printColored_WCharN(2481,79, VGA_COLOR_RED);
  getInputCode();
  Clear_VGA_Buffer(&TERMINAL_BUFFER);
  
}