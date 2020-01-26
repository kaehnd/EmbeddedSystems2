/*
 * keypad.c
 *
 *  Adapted on: Dec 23, 2019
 *      Author: Daniel Kaehn (also Dr. Rolthe)
 *      Class: CE 2812-021
 *      Assignment: Lab 3: LCD and Keypad API
 *
 *      Declarations for API for keypad peripheral
 */

#ifndef KEYPAD_H  /* Compile guard */
#define KEYPAD_H  /* Compile guard */

/* Initialize the I/O port used to interact with keypad.
 * Arguments:  none
 * Returns: void
 */
void keypad_init(void);

/* Returns key scan code (0-15) of the keypad.  Return value is an
 * int to be compatible with libc's getchar although EOF is not returned
 * in this version.  This method will block until a key is pressed.
 * Arguments:  none
 * Returns: int key code
 */
int keypad_getkey(void);

/* Returns key scan code (0-15) of the keypad if a key is currently
 * pressed otherwise returns EOF to indicate no key is pressed.  Return
 * value is an int to be compatible with libc's getkey.  This method
 * will not block.
 * Arguments:  none
 * Returns: int key code
 */
int keypad_getkey_noblock(void);

/* Similar to keypad_getkey except the ASCII value of the key is returned
 * promoted to int similar to libc's getchar.  Blocks.  No buffering is
 * offered in this version (unlike libc's getchar).
 * Arguments:  none
 * Returns: int ascii value of key
 */
int keypad_getchar(void);

/*
 * Gets number from keypad with specified number of digits
 * Arguments: int digits, number of digits to obtain from the keypad
 * 			  int printBuf, '1' to enable printing buffered digits to lcd, '0' to disable
 * Returns: integer number
 */
int keypad_getnum(int digits, int printBuf);


#endif   /* Compile guard */
