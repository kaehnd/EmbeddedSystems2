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

	 } //while (1) program loop
 }
 }
