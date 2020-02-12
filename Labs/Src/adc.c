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

void adc_init(int frequency)
{
    //RCC to ADC
    volatile uint32_t * const rcc = (volatile uint32_t *) RCC_BASE;
    rcc[APB2ENR] |= 1 <<8;
    //USING PIN A4!!!!*****
    //do stuff with pin initializationnnnnn

    //Turn on ADC
    adc1[ADC_CR2] |= 1<<0;

    //Set external trigger to Tim5 CH 1
    adc1[ADC_CR2] |= 0b1010 << 24;

    //set toggle mode
    adc1[ADC_CR2] |= 0b11 << 28;

    //set channel 0
    adc1[ADC_SQR3] = 0 << 0;



    //RCC to Timer 5
    rcc[APB1ENR] |= 1 << 3;

    //Set ARR and CCR to determine frequency
    uint32_t period = 16000000 / frequency;
    tim5[ARR] = period;
    tim5[CCR1] = period;

    //set output mode to toggle
    tim5[CCMR1] = 0b11 << 4;

    //compare output enable
    tim5[CCER] = 1;
    //Enable timer and start ADC???
    tim5[CR1] = 1;
}

uint16_t adc_getData()
{
    while(!((adc1[ADC_SR] >> 1) & 1));
    return adc1[ADC_DR];
}

