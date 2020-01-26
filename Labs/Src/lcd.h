/*
 * lcd.h
 *
 *  Created on: Dec 18, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Lab 3: LCD and Keypad API
 *
 *      Declarations and defines for LCD API
 */

#ifndef LCD_H_
#define LCD_H_

//Pin numbers
#define RS 8
#define RW 9
#define E 10

//LCD instructions

#define LCD_FUNCTION_SET 0x38
#define LCD_DISPLAY_ON 0x0C
#define LCD_CLEAR 0x01
#define LCD_HOME 0x02

//Dbus defines
#define LCD_DBUS 4

#define LCD_DBUS_OUTPUT 0b0101010101010101 << 2*LCD_DBUS
#define LCD_DBUS_INPUT 0b0000000000000000 << 2*LCD_DBUS

//commands for BSRR
#define RS_SET 1<<RS
#define RS_CLR 1<<(RS+16)
#define RW_SET 1<<RW
#define RW_CLR 1<<(RW+16)
#define E_SET 1<<E
#define E_CLR 1<<(E+16)

/*
 * Initializes LCD peripheral, must be called before any other function in this file
 * Arguments: int blink, '1' for blink, '0' for no blink
 * Returns: none
 */
void lcd_init(int blink);

/*
 * Clears the LCD by executing the LCD's clear instruction
 * Arguments: none
 * Returns: none
 */
void lcd_clear(void);

/*
 * Sets the position of the cursor to home by executing the LCD's home instruction
 * Arguments: none
 * Returns: none
 */
void lcd_home(void);

/*
 * Sets the position of the cursor to the specified row and col (0 indexed)
 * Arguments: int row, 0-indexed row 0 or 1
 * 			  int col, 0-indexed col
 * Returns: none
 */
void lcd_set_position(int row, int col);

/*
 * Prints the specified string up to 16 characters to the LCD
 * Arguments: char string[] string to print, 1-16 characters
 * Returns: int number of characters printed
 */
int lcd_print_string(char string[]);

/*
 * Prints one character to the LCD
 * Arguments: char c, character to print
 * Returns: int number of characters printed *ALWAYS 1*
 */
int lcd_print_char(char c);

/*
 * Prints the specified number to the LCD
 * Arguments: int num, up to 16-digit int to print to LCD
 * Returns: int number of characters printed
 */
int lcd_print_num(int num);


/*
 * Prints the specified number to the LCD zero-padded to the
 * specified number of digits
 * Arguments: int num, up to 16-digit int to print to LCD
 * 			  int digits, integer 1-16 specifying # digits to pad to
 * Returns: int number of characters printed
 */
int lcd_print_num_digits(int num, int digits);


#endif /* LCD_H_ */
