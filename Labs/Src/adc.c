/*
 * adc.c
 *
 *  Created on: Feb 10, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Final Project, do something cool
 *
 *      Guitar Signal Processor
 */

#include "adc.h"
#include "registers.h"
#include <stdint.h>

static volatile uint32_t * const adc1 = (volatile uint32_t *) ADC1_BASE;
static volatile uint32_t * const tim5 = (volatile uint32_t *) TIM5_BASE;

/*
 * Initializes ADC using TIM5 to trigger with the specified frequency
 * Parameters: int frequency, frequency for sample rate
 * Returns: None
 */
void adc_init(int frequency)
{
    //RCC to ADC
    volatile uint32_t * const rcc = (volatile uint32_t *) RCC_BASE;
    rcc[APB2ENR] |= 1<<ADCEN;

    //Turn on ADC
    adc1[ADC_CR2] |= 1<<ADC1EN;

    //Set external trigger to Tim5 CH 1
    adc1[ADC_CR2] |= ADCEXT_TIM5 << ADCEXT1;

    //set toggle mode
    adc1[ADC_CR2] |= ADCTGMTCH << ADC1_TGMODE;

    //set channel 0
    adc1[ADC_SQR3] = 0;

    //RCC to Timer 5
    rcc[APB1ENR] |= 1 << TIM5EN;

    //Set ARR and CCR to determine frequency
    uint32_t period = SYS_CLK / frequency;
    tim5[ARR] = period;
    tim5[CCR1] = period;

    //set output mode to toggle
    tim5[CCMR1] = TIM_TGMTCH << TIM_OCM1;

    //compare output enable
    tim5[CCER] = 1;
    //Enable timer and start ADC
    tim5[CR1] = 1;
}

/*
 * Gets 12-bit data from the ADC 
 * **BLOCKS*
 * Parameters: None
 * Returns: 12-bit Voltage ratio 0-4095 corresponding to 0-3.3V 
 */
uint16_t adc_getData(void)
{
    while(!((adc1[ADC_SR] >> 1) & 1));
    return adc1[ADC_DR];
}

