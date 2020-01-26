/*
 * main.c
 *
 *  Created on: Jan 21, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Lab 5: RingBuffer
 *
 *      Runs tests for RingBuffer struct and prints to the console
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ringbuffer.h"
#include "uart_driver.h"

//system values
#define F_CPU 16000000UL
#define BAUD_R 19200

/*
 * Main program entry
 */
 int main(void)
 {
 	 init_usart2(BAUD_R, F_CPU);
 	 ringBufferTests();

	 while (1)
	 {
		
	 } //while (1) program loop
 }

 void SystemInit(void)
 {
	 
 }
