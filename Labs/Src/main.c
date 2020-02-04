/*
 * main.c
 *
 *  Created on: Jan 18, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Lab 4: LCD and Keypad API
 *
 *      Implements a service menu for console interaction with STM32 board memory
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "delays.h"
#include "leds.h"
#include "lcd.h"
#include "keypad.h"
#include "speaker.h"
#include "uart_driver.h"

//system values
#define F_CPU 16000000UL
#define BAUD_R 19200

//registers used for HardFault handling
#define NVIC_ICPR0 0xE000E280
#define SCB_MMFAR 0xE000ED34

//flag for unsafe memory access
static int unsafeMem;

 /*
 * Reads from specified memory address
 * Arguments: char * savePtr, save pointer for strtok_r
 * Returns: none
 */
static void readMem(char * savePtr);

/*
* Writes to specified memory address
* Arguments: char * savePtr, save pointer for strtok_r
* Returns: none
*/
static void writeMem(char * savePtr);

/*
* Prints from memory specified by inst string to console
* Arguments: char * savePtr, save pointer for strtok_r
* Returns: none
*/
static void dumpMem(char * savePtr);

/*
 * Prints help to console
 * Arguments: char * savePtr, save pointer for strtok_r
 * Returns: none
 */
static void printHelp(char * savePtr);


/*
 * Main program entry
 */
 int main(void)
 {
 	 init_usart2(BAUD_R, F_CPU);

	 while (1)
	 {
		 printf("\nSTM32_Support_Main_Menu: ");
		 char inst[50];
		 fgets(inst, 50, stdin); //get instruction string

		 char * saveptr = 0; //used to tokenize the rest of the inst string

		 char * type = strtok(inst, "\n"); //remove newline from end
		 type = strtok_r(type, " ", &saveptr);

		 //find instruction type
		 if (strcmp(type, "rm") == 0)
		 {
 			readMem(saveptr);
		 }
		 else if (strcmp(type, "wm") == 0)
		 {
			writeMem(saveptr);
		 }
		 else if (strcmp(type, "dm") == 0)
		 {
			 dumpMem(saveptr);
		 }
		 else if (strcmp(type, "help") == 0)
		 {
			 printHelp(saveptr);
		 }
		 else if (strcmp(type, "\0") == 0)
		 {
			 //do nothing
		 }
		 else
		 {
			 printf("\n\"%s\" instruction definition not found. Type \"help\" for help\n", type);
		 }
	 } //while (1) program loop
 }

 /*
  * HardFault_Handler used to create 'safe' memory accesses from potentially unsafe accesses
  * Uses unsafeMem flag to determine behavior
  */
  void HardFault_Handler(void)
  {
 	 	 if (unsafeMem)
 	 	 {
 		 *((volatile uint32_t *) NVIC_ICPR0) |= (1<<3); //clear pending interrupt
 		 printf("Memory address %x caused invalid memory access error.", (unsigned int) *((volatile uint32_t *) SCB_MMFAR));
 		 main();
 	 	 }
 	 	 else {
 	 		 printf("Hard Fault");
 	 	 }
  }

 /*
 * Reads from specified memory address
 * Arguments: char * savePtr, save pointer for strtok_r
 * Returns: none
 */
 static void readMem(char * savePtr)
 {
	 char * address = strtok_r(NULL, " ", &savePtr); //string such as "0x20000000"
	 char * pointer = 0;
	 sscanf(address, "%p", &pointer); //place address into pointer
	 if (pointer == 0)
	 {
		 printf("\nIncorrect syntax for 'rm', type 'help rm' for help\n");
	 }
	 else {
		 unsafeMem = 1;
		 printf("\nMem value at %p:\t%x\t%d\n",pointer, *pointer, *pointer);
		 unsafeMem = 0;
	 }
 }

 /*
 * Writes to specified memory address
 * Arguments: char * savePtr, save pointer for strtok_r
 * Returns: none
 */
static void writeMem(char * savePtr)
 {
	 char * address = strtok_r(NULL, " ", &savePtr); //string such as "0x20000000"
	 char * pointer = 0;
	 sscanf(address, "%p", &pointer); //place address into pointer

	 if (pointer == 0)
	 {
		 printf("\nIncorrect syntax for 'wm', type 'help wm' for help\n");
	 }
	 else
	 {
		 char * valueStr = strtok_r(NULL, " ", &savePtr);
		 int value;
		 sscanf(valueStr, "%x", &value);

		 unsafeMem = 1;
		 *pointer = value;
		 unsafeMem = 0;

		 if (*pointer == value)
		 {
			 printf("\nWrite successful!\n");
			 printf("Mem value at %p: 0x%x\n",pointer, *pointer);
		 }
		 else
		 {
			 printf("Error: Something went wrong with the write. Check address and byte value are valid");
		 }
	 }
 }

 /*
 * Prints from memory specified by inst string to console
 * Arguments: char * savePtr, save pointer for strtok_r
 * Returns: none
 */
 static void dumpMem(char * savePtr)
 {
	 char * addressStr = strtok_r(NULL, " ", &savePtr); //string such as "0x20000000"
	 char * pointer;
	 sscanf(addressStr, "%p", &pointer); //place address into pointer

	 if (pointer == 0) {
		 printf("\nIncorrect syntax for 'dm', type 'help dm' for help\n");
	 }
	 else {
		 printf("\n");
		 char * sizeStr = strtok_r(NULL, " ", &savePtr);
		 int size;
		 if (*sizeStr == '\0')
		 {
			 size = 16; //default value if none entered
		 }
		 else
		 {
			 sscanf(sizeStr, "%d", &size);
		 }
		 char * endPtr = pointer + size;
		 while (pointer != endPtr)
		 {
			 printf("%x\t", (unsigned int) pointer);

			 for (int i = 0; i < 8 && pointer != endPtr; i++, pointer++)
			 {
				 unsafeMem = 1;
				 printf("%02x\t", *pointer);
				 unsafeMem = 0;
			 }
			 printf("\n");
		 }
	 }
 }

 /*
  * Prints help to console
  * Arguments: char * savePtr, save pointer for strtok_r
  * Returns: none
  */
static void printHelp(char * savePtr)
 {
	 char * helpType = strtok_r(NULL, " ", &savePtr);

	 if (strcmp(helpType, "rm") == 0)
	 {
		 printf("\nRead Memory: reads and prints byte value at specified hex memory address. "
				 "\nUsage: rm <mem addr (hex)>\n");
	 }
	 else if (strcmp(helpType, "wm") == 0)
	 {
		 printf("\nWrite Memory: writes specified byte value to specified hex memory address. "
				 "\nUsage: wm <mem addr (hex)> <byte (hex)>\n");
	 }
	 else if (strcmp(helpType, "dm") == 0)
	 {
		 printf("\nDump Memory: reads and prints byte values starting at specified hex memory address. "
				 "\nTakes optional decimal argument specifying number of bytes to print, else defaults to 16."
				 "\nUsage: dm <mem addr (hex)> [# bytes (dec)]\n");
	 }
	 else if (strcmp(helpType, "\0") == 0)
	 {
		 printf("\nCommands supported in STM32_Support_Main_Menu:\n\n"
				 "rm: Read from memory location\n"
				 "wm: Write to memory location\n"
				 "dm: Dump memory from location\n\n"
				 "Type \"help\" <command> for help with specific instructions. \n\n");
	 }
	 else
	 {
		 printf("\n\"%s\" instruction definition not found. Type \"help\" for help\n", helpType);
	 }
 }
