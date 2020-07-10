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
#include "registers.h"
#include "irdecode.h"
#include "ac_send.h"

//system values
#define F_CPU 16000000UL
#define BAUD_R 19200


/*
 * Main program entry
 */
 int main(void)
 {
	init_usart2(BAUD_R, F_CPU);
	//ac_sendCode(AC_POWER);

	while(1) {
		printf("AC_REMOTE!");
		char cmd[50];
		gets(cmd);

		if (strcmp(cmd, "power") == 0) {

			ac_sendCode(AC_POWER);

		} else if (strcmp(cmd, "cool") == 0) {

			ac_sendCode(AC_COOL);

		} else if (strcmp(cmd, "fan") == 0) {

			ac_sendCode(AC_FAN);

		} else if (strcmp(cmd, "tempUp") == 0) {

			ac_sendCode(AC_TEMP_UP);

		} else if (strcmp(cmd, "tempDown")== 0) {

			ac_sendCode(AC_TEMP_DOWN);

		} else if (strcmp(cmd, "delayOn")==0) {

			ac_sendCode(AC_DELAY_ON);

		} else if (strcmp(cmd, "delayOff") == 0) {

			ac_sendCode(AC_DELAY_OFF);

		} else if (strcmp(cmd, "high") == 0) {

			ac_sendCode(AC_HIGH);

		} else if (strcmp(cmd, "low") == 0) {

			ac_sendCode(AC_LOW);

		} else if (strcmp(cmd, "med") == 0) {

			ac_sendCode(AC_MED);

		} else {

			printf("oops\n\n");
		}
	}
 }
