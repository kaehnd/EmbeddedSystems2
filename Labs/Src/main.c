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
#include "leds.h"

#include "lcd.h"
#include "delays.h"

//system values
#define F_CPU 16000000UL
#define BAUD_R 19200

#define FREQ_RANGE 4095

static volatile uint32_t * const gpioc = (volatile uint32_t *) GPIOC_BASE;

volatile uint32_t * const exti = (volatile uint32_t *) EXTI_BASE;


static int distEn = 0;



static void guitarIn_init()
{
	volatile uint32_t * const rcc = (volatile uint32_t *) RCC_BASE;
	volatile uint32_t * const gpioa = (volatile uint32_t *) GPIOA_BASE;
	rcc[AHB1ENR] |= 1<<0;
	gpioa[MODER] |= ANALOG << MODER0;
	gpioa[MODER] |= ANALOG << MODER5;

}

unsigned int simple_distort(unsigned int a)
{
	if (a < FREQ_RANGE / 3)
	{
		return 2*a;
	}
	else if (a >= FREQ_RANGE / 3 && a < FREQ_RANGE * 2 / 3)
	{
	 return (( (-1*(a/455)*(a)) +12*a-FREQ_RANGE) /3);
	}
	else 
	{
		return FREQ_RANGE;
	}
}

void blueButton_init_IT(void)
{
	volatile uint32_t * const rcc = (volatile uint32_t *) RCC_BASE;
	rcc[APB2ENR] |= 1<<14; //SYSCFGEN
	rcc[AHB1ENR] |= 1<<2; //GPIOCEN
	
	gpioc[MODER] &= ~(11<<MODER13);
	gpioc[MODER] |= (INPUT<<MODER13); //Set blue buttona as input

	gpioc[PUPDR] |= PULL_UP<<PUPDR13;

	volatile uint32_t * const syscfg = (volatile uint32_t *) SYSCFG_BASE;

	syscfg[SYSCFG_EXTICR4] = 0b0010<<4; //EXTI line 13 set to bank C

	volatile uint32_t * const nvic = (volatile uint32_t *) NVIC_BASE;

	nvic[ISER1] |= 1<<8; //Set enable in NVIC, NVIC POS 40

	exti[EXTI_IMR] = 1<<13; //Unmask in EXTI
	exti[EXTI_FTSR] = 1<<13; //Set falling trigger
}

void EXTI15_10_IRQHandler(void)
{
	//Check which interrupt fired

	if (exti[EXTI_PR]>>13 & 1) 
	{
		exti[EXTI_PR] = 1<<13; //clear pending interrupt

		distEn ^= 1; //toggle distortion

	}
}

/*
 * Main program entry
 */
 int main(void)
 {
//	 int i = 0;
	 adc_init(32000);
	 dac_init();
//	 init_usart2(BAUD_R, F_CPU);
	 guitarIn_init();
	 blueButton_init_IT();

//	distEn = 1;
	
	 while (1)
	 {
		int sample = adc_getData();
		if (distEn)
		{
			sample = simple_distort(sample);
		}
		dac_put(sample);
	 } //while (1) program loop
 }