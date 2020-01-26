/*
 * lcd.c
 *
 *  Created on: Dec 18, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Lab 3: LCD and Keypad API
 *
 *      Implements API for LCD peripheral
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "delays.h"
#include "registers.h"


//register pointers
static volatile uint32_t * const gpioc = (uint32_t *) GPIOC_BASE;
static volatile uint32_t * const gpioa = (uint32_t *) GPIOA_BASE;


/* Helper function to write one 8-bit instruction to the LCD
 *** Blocks until the LCD is no longer busy**
 * Arguments: uint8_t inst, 8-bit instruction to write to LCD
 * Returns: none
 */
static void writeInstruction(uint8_t inst);

/*Helper function to write one 8-bit ASCII character to the LCD
***Blocks until the LCD is no longer busy**
 * Arguments: uint8_t data, 8-bit ascii character to write to LCD
 * Returns: none
 */
static void writeData(uint8_t data);

/*
 * Helper function that polls the busy flag until the LCD is not busy
 * ***BLOCKS UNTIL LCD IS NOT BUSY***
 * Arguments: none
 * Returns: none
 */
static void pollBusy(void);



/*
 * Initializes LCD peripheral, must be called before any other function in this file
 * Arguments: int blink, '1' for blink, '0' for no blink
 * Returns: none
 */
void lcd_init(int blink)
{
	//Enable GPIOA and GPIOC in AHB1ENR RCC register
	((volatile uint32_t *) RCC_BASE)[AHB1ENR] |= (1<<GPIOAEN)|(1<<GPIOCEN);

	gpioc[MODER] &= ~((0b11<<2*RS)|(0b11<<2*RW)|(0b11<<2*E)); //clear bits
	gpioc[MODER] |= (OUTPUT<<2*RS)|(OUTPUT<<2*RW)|(OUTPUT<<2*E); //set RS, RW, E to be outputs

	gpioa[MODER] &= ~(0xFFFF<<2*LCD_DBUS); //clear bits
	gpioa[MODER] |= (LCD_DBUS_OUTPUT); //set DBUS to be output

	delay_ms(40); //required 40ms delay after power up

	//startup sequence

	writeInstruction(LCD_FUNCTION_SET); //function set
	writeInstruction(LCD_FUNCTION_SET); //function set
	writeInstruction(LCD_DISPLAY_ON|blink); //turn on lcd with desired parameters
	lcd_clear();
	writeInstruction(0x06); //entry mode set
}

/*
 * Clears the LCD by executing the LCD's clear instruction
 * Arguments: none
 * Returns: none
 */
void lcd_clear(void)
{
	writeInstruction(LCD_CLEAR); //Clear instruction
}

/*
 * Sets the position of the cursor to home by executing the LCD's home instruction
 * Arguments: none
 * Returns: none
 */
void lcd_home(void)
{
	writeInstruction(LCD_HOME); //Home instruction
}

/*
 * Sets the position of the cursor to the specified row and col (0 indexed)
 * Arguments: int row, 0-indexed row 0 or 1
 * 			  int col, 0-indexed col
 * Returns: none
 */
void lcd_set_position(int row, int col)
{
	uint8_t dRamAddress =0b10000000; //base DRAM write instruction
	dRamAddress += row * 0x40 + col; //Adds adds specified row and col to address
	writeInstruction(dRamAddress); //writes to DRAM
}

/*
 * Prints the specified string up to 16 characters to the LCD
 * Accepts 1 \n newline character to change position to row 2
 * Arguments: char string[] string to print, 1-16 characters
 * Returns: int number of characters printed
 */
int lcd_print_string(char string[])
{
	int i = -1;

	//prints each char in string in loop
	while(string[++i])
	{
		if (string[i] == '\n')
		{
			lcd_set_position(1, 0);
		} else {
			writeData(string[i]);
		}

	}
	return i; //number of chars printed
}

/*
 * Prints one character to the LCD
 * Arguments: char c, character to print
 * Returns: int number of characters printed *ALWAYS 1*
 */
int lcd_print_char(char c)
{
	writeData(c);
	return 1;
}

/*
 * Prints the specified number to the LCD
 * Arguments: int num, up to 16-digit int to print to LCD
 * Returns: int number of characters printed
 */
int lcd_print_num(int num)
{
	char string[16];
	sprintf(string, "%d", num); //puts num in buf as a string

	return lcd_print_string(string);
}

/*
 * Prints the specified number to the LCD zero-padded to the
 * specified number of digits
 * Arguments: int num, up to 16-digit int to print to LCD
 * 			  int digits, integer 1-16 specifying # digits to pad to
 * Returns: int number of characters printed
 */
int lcd_print_num_digits(int num, int digits)
{
	char buf[16] = "0000000000000000";
	sprintf(buf, "%d", num); //puts num in buf as a string

	char string[digits+1];

	int length = (int) strlen(buf);

	for (int i = 0; i <= digits; i++)
	{
		if (i < digits - length) {
			string[i] = '0';
		} else {
			string [i] = buf[i - digits + length];
		}
	}

	return lcd_print_string(string);
}

/* Helper function to write one 8-bit instruction to the LCD
 *** Blocks until the LCD is no longer busy**
 * Arguments: uint8_t inst, 8-bit instruction to write to LCD
 * Returns: none
 */
static void writeInstruction(uint8_t inst)
{
	gpioc[BSRR] = RS_CLR|RW_CLR; //Inst write pattern
	gpioc[BSRR] = E_SET; //E high to prepare for inst

	gpioa[ODR] &= ~(0xFF<<LCD_DBUS);//Clear bits
	gpioa[ODR] |= (inst<<LCD_DBUS);//Set bits

	gpioc[BSRR] = E_CLR; //E Low to write

	pollBusy(); //poll busy flag
}

/*Helper function to write one 8-bit ASCII character to the LCD
***Blocks until the LCD is no longer busy**
 * Arguments: uint8_t data, 8-bit ascii character to write to LCD
 * Returns: none
 */
static void writeData(uint8_t data)
{
	gpioc[BSRR] = RS_SET|RW_CLR; //Data write pattern
	gpioc[BSRR] = E_SET; //E high to prepare for inst

	gpioa[ODR] &= ~(0xFF<<LCD_DBUS); //Clear bits
	gpioa[ODR] |= (data<<LCD_DBUS); //Set bits

	gpioc[BSRR] = E_CLR; //E low to write

	pollBusy(); //poll busy flag
}

/*
 * Helper function that polls the busy flag until the LCD is not busy
 * ***BLOCKS UNTIL LCD IS NOT BUSY***
 * Arguments: none
 * Returns: none
 */
static void pollBusy(void)
{
	gpioa[MODER] &= ~(0xFFFF<<2*LCD_DBUS); //clear bits
	gpioa[MODER] |= (LCD_DBUS_INPUT); //set DBUS to be input

	gpioc[BSRR] = RS_CLR|RW_SET|E_CLR;

	do
	{
		gpioc[BSRR] = E_SET;
		gpioc[BSRR] = E_CLR;
	} while (gpioa[IDR] & 1<<11); //loop until busy flag goes low

	gpioa[MODER] &= ~(0xFFFF<<2*LCD_DBUS); //clear bits
	gpioa[MODER] |= (LCD_DBUS_OUTPUT); //set DBUS to be output
}
