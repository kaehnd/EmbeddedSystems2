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

#include <stdio.h>




#include "registers.h"
#include "uart_driver.h"
#include "adc.h"
#include "dac.h"

//system values
#define F_CPU 16000000UL
#define BAUD_R 19200



static void guitarIn_init()
{
	volatile uint32_t * const rcc = (volatile uint32_t *) RCC_BASE;
	volatile uint32_t * const gpioa = (volatile uint32_t *) GPIOA_BASE;
	rcc[AHB1ENR] |= 1<<0;
	gpioa[MODER] |= ANALOG << MODER0;

	gpioa[MODER] |= ANALOG << MODER4;

}
/*
 * Main program entry
 */
 int main(void)
 {
 	 init_usart2(BAUD_R, F_CPU);
	 guitarIn_init();
	 
	 adc_init(5);
	 dac_init();
	

	 while (1)
	 {
		int data = adc_getData();

		printf("Input: %d", data);
		// stuff = data * 220000;
		//int voltage = stuff / 273;
		//printf("%d\n", voltage);

		dac_put(data);

		

	 } //while (1) program loop
 }