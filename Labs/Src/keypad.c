/*
 * keypad.c
 *
 *  Adapted on: Dec 23, 2019
 *      Author: Daniel Kaehn (also Dr. Rolthe)
 *      Class: CE 2812-021
 *      Assignment: Lab 3: LCD and Keypad API
 *
 *      Implements API for keypad peripheral
 */

#include "keypad.h"
#include "registers.h"
#include "delays.h"
#include "lcd.h"


#include <stdio.h>
#include <strings.h>


#define COL_IN_ROW_OUT 0b0101010100000000
#define COL_OUT_ROW_IN 0b0000000001010101



/* Executes a single scan of the keypad.
 * Arguments:  none
 * Returns: int, code of the scanned key or -1 if no key detected
 */
static int scan();


static volatile uint32_t * const gpioc = (uint32_t *) GPIOC_BASE;

//array containing current use keypad key values
static const char keys[] = {	'1','2','3','A',
								'4','5','6','B',
								'7','8','9','C',
								'*','0','#','D' };


/* Initialize the I/O port used to interact with keypad.
 * Arguments:  none
 * Returns: void
 */
void keypad_init(void)
{
	// RCC is only used here, so no file-scope declaration
	volatile uint32_t * const rcc = (uint32_t *) RCC_BASE;

	// enable GPIOC's clock
	rcc[AHB1ENR] |= (1 << GPIOCEN);

	// Set the rows to output, cols to input
	uint32_t temp = gpioc[MODER];

	// Just in case, clear all mode bits for PC0-PC7
	temp &= ~(0xFFFF);
	// PC3-PC7 to output ('01')
	gpioc[MODER] |= temp | (OUTPUT<<MODER7) | (OUTPUT<<MODER6) |
								(OUTPUT<<MODER5) |(OUTPUT<<MODER4);
	// Enable pullups on cols - just in case, clear first for PC0-PC3
	temp = gpioc[PUPDR];
	temp &= ~(0xFFFF);
	gpioc[PUPDR] |= (PULL_UP<<PUPDR3)|(PULL_UP<<PUPDR2)|
						(PULL_UP<<PUPDR1)|(PULL_UP<<PUPDR0);
	gpioc[PUPDR] |= (PULL_UP<<PUPDR7)|(PULL_UP<<PUPDR6)|
							(PULL_UP<<PUPDR5)|(PULL_UP<<PUPDR4);

}

/* Returns key scan code (0-15) of the keypad.  Return value is an
 * int to be compatible with libc's getchar although EOF is not returned
 * in this version.  This method will block until a key is pressed.
 * Arguments:  none
 * Returns: int key code
 */
int keypad_getkey(void)
{
	int key = -1;

	while(key == -1)
	{
		key = keypad_getkey_noblock();
	}

	return key;
}

/* Returns key scan code (0-15) of the keypad if a key is currently
 * pressed otherwise returns EOF to indicate no key is pressed.  Return
 * value is an int to be compatible with libc's getkey.  This method
 * will not block.
 * Arguments:  none
 * Returns: int key code
 */
int keypad_getkey_noblock(void)
{
	return scan();
}

/* Similar to keypad_getkey except the ASCII value of the key is returned
 * promoted to int similar to libc's getchar.  Blocks.  No buffering is
 * offered in this version (unlike libc's getchar).
 * Arguments:  none
 * Returns: int ascii value of key
 */
int keypad_getchar(void)
{
	return keys[keypad_getkey()];
}

/*
 * Gets number from keypad with specified number of digits
 * Arguments: int digits, number of digits to obtain from the keypad
 * 			  int printBuf, '1' to enable printing buffered digits to lcd, '0' to disable
 * Returns: integer number
 */
int keypad_getnum(int digits, int printBuf)
{
	char buf[digits];

	//get digits number of characters and places them into a buffer
	for (int i = 0; i < digits; i++)
	{
		buf[i] = keypad_getchar(); //get char

		//print buffered char if buffer printing is enabled
		if (printBuf)
		{
			lcd_print_char(buf[i]);
		}
		while (keys[keypad_getkey_noblock()] == buf[i]); //delay until key is released

		delay_us(10); //protects from accidental double presses, acts as a debouncer
	}
	int number = 0;
	sscanf(buf, "%d", &number); //parses the chars into integer number
	return number;
}

/* Executes a single scan of the keypad.
 * Arguments:  none
 * Returns: int, code of the scanned key or -1 if no key detected
 */
static int scan()
{
	/* This routine will drive rows low, and check columns for low. */
	//Cols out rows in
	gpioc[MODER] &= ~(0xFFFF); //clear bits
	// PC0-PC3 to output ('01')
	gpioc[MODER] |= (OUTPUT<<MODER3) | (OUTPUT<<MODER2) |
								(OUTPUT<<MODER1) |(OUTPUT<<MODER0);

	//drive cols low
	gpioc[BSRR] = (0b1111<<(DR0+16));

	//wait for propagation
	delay_us(1);

	//read row IDR & store value
	uint8_t rowKey = (gpioc[IDR]>>DR4) & 0b1111;

	//end if all '1's
	if (rowKey == 0b1111)
	{
		return -1;
	}

	//copy row IDR to row ODR
	gpioc[ODR] = rowKey << DR4;

	//set rows output and cols input
	gpioc[MODER] &= ~(0xFFFF); //clear bits
	// PC7-PC4 to output ('01')
	gpioc[MODER] |= (OUTPUT<<MODER7) | (OUTPUT<<MODER6) |
								(OUTPUT<<MODER5) |(OUTPUT<<MODER4);

	//wait for propagation
	delay_us(1);

	uint8_t colKey = gpioc[IDR] & 0b1111;

	//find key from row and column
	int key = 0;

	while (rowKey & 1)
	{
		rowKey >>= 1;
		key += 4; //add four to correspond to skipping one row
	}
	while (colKey & 1)
	{
		colKey >>= 1;
		key++;
	}
	return key;
}
