/*
 * main.c
 *
 *  Created on: Feb 19, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Lab 9: Guitar Effects Processor
 *
 *      Implements a sample-by-sample guitar signal processor using adc and dac
 */

#include <stdio.h>

#include "registers.h"
#include "adc.h"
#include "dac.h"

//system values
#define FREQ_RANGE 4000
#define SAMPLE_RATE 41000

//EXTI definitions
#define EXTICR13 4
#define EXTICR_CBANK 0b0010
#define EXTI13 13
#define EXTI_15_10_NVIC 8

static volatile uint32_t * const gpioc = (volatile uint32_t *) GPIOC_BASE;
volatile uint32_t * const exti = (volatile uint32_t *) EXTI_BASE;

//Flag for distortion enable
static int distEn = 0;

/*
 * Implements simple distortion on sample voltage ratio a 
 * Parameters: unsigned int a, sample voltage in, such that 0<a<FREQ_RANGE
 * Returns: unsigned int, sample voltage out
 */
unsigned int simple_distort(unsigned int a);

/*
 * Initializes blue button with external interrupts
 * Parameters: None
 * Returns: None
 */ 
void blueButton_init_IT(void);

/*
 * Initializes pins for guitar input and output
 * Parameters: none
 * Returns: none
 */
static void guitarIO_init(void);


/*
 * Main program entry
 */
 int main(void)
 {
	 adc_init(SAMPLE_RATE);
	 dac_init();
	 guitarIO_init();
	 blueButton_init_IT();
	 while (1)
	 {
		int sample = adc_getData();  //Input
	
		if (distEn)
		{
			sample = simple_distort(sample);
		}
		dac_put(sample); //Output
	 } //while (1) program loop
 }



/*
 * Initializes pins for guitar input and output
 * Parameters: none
 * Returns: none
 */
static void guitarIO_init(void)
{
	volatile uint32_t * const rcc = (volatile uint32_t *) RCC_BASE;
	volatile uint32_t * const gpioa = (volatile uint32_t *) GPIOA_BASE;
	rcc[AHB1ENR] |= 1<<GPIOAEN;
	gpioa[MODER] |= ANALOG << MODER0;
	gpioa[MODER] |= ANALOG << MODER5;
}

/*
 * Initializes blue button with external interrupts
 * Parameters: None
 * Returns: None
 */ 
void blueButton_init_IT(void)
{
	volatile uint32_t * const rcc = (volatile uint32_t *) RCC_BASE;
	rcc[APB2ENR] |= 1<<SYSCFGEN; //SYSCFGEN
	rcc[AHB1ENR] |= 1<<GPIOCEN; //GPIOCEN
	
	gpioc[MODER] &= ~(11<<MODER13);
	gpioc[MODER] |= (INPUT<<MODER13); //Set blue button as input

	gpioc[PUPDR] |= PULL_UP<<PUPDR13;

	volatile uint32_t * const syscfg = (volatile uint32_t *) SYSCFG_BASE;

	syscfg[SYSCFG_EXTICR4] = EXTICR_CBANK<<EXTICR13; //EXTI line 13 set to bank C

	volatile uint32_t * const nvic = (volatile uint32_t *) NVIC_BASE;

	nvic[ISER1] |= 1<<EXTI_15_10_NVIC; //Set enable in NVIC, NVIC POS 40

	exti[EXTI_IMR] = 1<<EXTI13; //Unmask in EXTI
	exti[EXTI_FTSR] = 1<<EXTI13; //Set falling trigger
}

/*
 * Implements simple clipping distortion on sample voltage ratio a 
 * Parameters: unsigned int a, sample voltage in, such that 0<a<FREQ_RANGE
 * Returns: unsigned int, sample voltage out
 */
unsigned int simple_distort(unsigned int a)
{
	if (a < FREQ_RANGE / 3)
	{
		return 2*a;
	}
	else if (a >= FREQ_RANGE / 3 && a < FREQ_RANGE * 2 / 3)
	{
		return (-1*((9*a)/FREQ_RANGE)*a + 12*a  - FREQ_RANGE) / 3;
	}
	else 
	{
		return FREQ_RANGE;
	}
}

/*
 * Handles interrupts from EXTI 15-10
 * For line 13, toggles distortion enable
 * Parameters: None
 * Returns: None
 */
void EXTI15_10_IRQHandler(void)
{
	//Check which interrupt fired

	if (exti[EXTI_PR]>>13 & 1) 
	{
		exti[EXTI_PR] = 1<<13; //clear pending interrupt
		distEn ^= 1; //toggle distortion
	}
}