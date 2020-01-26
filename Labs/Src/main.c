/*
 * main.c
 *
 *  Created on: Dec 22, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Lab 3: LCD and Keypad API
 *
 *      Implements a timer program testing the functionality of the LCD and Keypad APIs
 */

#include <stdlib.h>
#include <stdio.h>

#include "delays.h"
#include "leds.h"
#include "lcd.h"
#include "keypad.h"
#include "speaker.h"


 /* Gets time to set timer for from user
 * Arguments: char time[], reference to string to take the user input
 * Returns: none, modifies data passed by reference
 */
static void getTime(char time[]);

/*
 * Shifts the time string left according to pattern "hh:mm:ss"
 * Arguments: char string[], string to shift
 * Returns: none
 */
static void shiftLeft(char string[]);

/*
 * Timer function of program, delays and prints time to lcd
 * Exits when timer is finished and user stops alarm or user clears timer
 * Arguments: int seconds, number of seconds to delay
 * 			  int minutes, number of minutes to delay
 * 			  int hours, number of hours to delay
 * 	Returns: none
 */
static void time(int seconds, int minutes, int hours);

/*
 * Short beep used as feedback to user input and delay to prevent double key presses
 * Arguments: none
 * Returns: none
 */
static void beep(void);


/*
 * Main entry to program
 * Parameters: none
 * Returns: int (not used, program always runs)
 */
int main(void)
{
	lcd_init(0); //init LCD with no blink
	keypad_init();
	speaker_init();

	lcd_print_string("Precision\nTimer");
	delay_s(1);

	while (1)
	{
		int seconds = 0;
		int minutes = 0;
		int hours = 0;
		char timestr[8] = "00:00:00";

		//get time to delay from keypad in string
		getTime(timestr);

		//get time components from string
		sscanf(timestr,"%d:%d:%d", &hours, &minutes, &seconds);

		beep();

		//start timing
		time(seconds, minutes, hours);

		//timer completed or cancelled, restart loop
		beep();
	}
}

/*
 * Gets time to set timer for from user
 * Arguments: char time[], reference to string to take the user input
 * Returns: none, modifies data passed by reference
 */
static void getTime(char time[])
{
	lcd_clear();

	lcd_print_string("Time: (* entr)");

	lcd_set_position(1, 0);
	lcd_print_string(time); //print 00:00:00

	//gets up to 8 digits from user
	for (int i = 0; i < 8; i++)
	{
		char c;

		//gets characters until input is valid
		for (c = keypad_getchar(); c == 'A' || c == 'B' || c == 'C' || c == 'D' ||
			c == '#' || (c == '*' && i == 0); c = keypad_getchar());

		//returns and starts timer if '*' pressed
		if (c == '*')
		{
			return;
		}

		//shift current time left
		shiftLeft(time);
		//set last character equal to the inputted char
		time[7] = c;

		//print currently set time
		lcd_set_position(1, 0);
		lcd_print_string(time);

		//beep for feedback and time delay
		beep();
	}

	while(keypad_getchar() != '*'); //wait for * character
}

/*
 * Shifts the time string left according to pattern "hh:mm:ss"
 * Arguments: char string[], string to shift
 * Returns: none
 */
static void shiftLeft(char string[])
{
	string[0] = string[1];
	string[1] = string[3];
	string[3] = string[4];
	string[4] = string[6];
	string[6] = string[7];
}

/*
 * Timer function of program, delays and prints time to lcd
 * Exits when timer is finished and user stops alarm or user clears timer
 * Arguments: int seconds, number of seconds to delay
 * 			  int minutes, number of minutes to delay
 * 			  int hours, number of hours to delay
 * 	Returns: none
 */
static void time(int seconds, int minutes, int hours)
{
	int decis = 0; //number of deciseconds initialize
	int pos = 0; //keeps track of LCD position

	lcd_clear();
	lcd_print_string("Timing: (C clr)\n");

	//hours loop
	do
	{
		//print and decrement hours
		pos += lcd_print_num_digits(hours--, 2);
		pos += lcd_print_char(':');

		//minutes loop
		do
		{
			//print and decrement minutes
			pos += lcd_print_num_digits(minutes--, 2);
			pos += lcd_print_char(':');

			//seconds loop
			do
			{
				//print and decrement seconds
				pos += lcd_print_num_digits(seconds--, 2);
				pos += lcd_print_char('.');

				//deciseconds loop
				do
				{
					//print and decrement deciseconds
					lcd_set_position(1, pos);
					lcd_print_num_digits(decis--, 2);

					//delay for 1 decisecond
					delay_ms(10);

					//implements 'C' to clear functionality
					if (keypad_getkey_noblock() == 11)
					{
						return; //stop and reset timer
					}

				} while (decis + 1);

				decis = 99; //reset decis
				pos -= 3;
				lcd_set_position(1, pos);

			} while (seconds + 1);

			seconds = 59; //reset seconds
			pos -= 3;
			lcd_set_position(1, pos);

		} while (minutes + 1);

		minutes = 59;
		pos -= 3;
		lcd_set_position(1, pos);

	} while (hours + 1);

	//timer done, alarm until input
	lcd_clear();
	lcd_print_string("Done\nPress any key");
	speaker_alarm(1000);
}

/*
 * Short beep used as feedback to user input and delay to prevent double key presses
 * Arguments: none
 * Returns: none
 */
static void beep() {
	speaker_play_tone(1000);
	delay_ms(200);
	speaker_stop();
}
