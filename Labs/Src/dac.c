/*
 * dac.c
 *
 *  Created on: Feb 10, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Final Project, do something cool
 *
 *      Guitar Signal Processor
 */

#include <stdint.h>

#include "dac.h"
#include "registers.h"

static volatile uint32_t * const dac1 = (volatile uint32_t *) DAC1_BASE;

void dac_init()
{
    
    //Using Pin A4

     //RCC DAC
    volatile uint32_t * const rcc = (volatile uint32_t *) RCC_BASE;
    rcc[APB1ENR] |= 1<<29; //DACEN
   

    //CR1
    dac1[DAC_CR] |= 0b111 << 3; //TSEL1 to SWTRGR
    dac1[DAC_CR] |= 1 <<2; //TEN1 enable channel 1
    dac1[DAC_CR] |= 1<<0; //EN1 DAC CH1 Enable

  

}

void dac_put(uint16_t value)
{
    
    dac1[DAC_DHR12R1] = value;
    dac1[DAC_SWTRIGR] = 1;
}
