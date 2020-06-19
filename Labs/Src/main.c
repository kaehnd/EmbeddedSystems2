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

//system values
#define F_CPU 16000000UL
#define BAUD_R 19200







/*
 * Main program entry
 */
 int main(void)
 {
	init_usart2(BAUD_R, F_CPU);
	irDecodeInit();

	while (1) {	
		while(!getIRDataValid());
		printf("%d", getIRData());
	}
 }
