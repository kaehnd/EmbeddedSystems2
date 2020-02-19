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

/*
 * Initializes DAC1 for output
 * Parameters: None
 * Returns: None
 */
void dac_init(void)
{
     //RCC DAC
    volatile uint32_t * const rcc = (volatile uint32_t *) RCC_BASE;
    rcc[APB1ENR] |= 1<<DACEN; //DACEN
   
    dac1[DAC_CR] |= 1<<DACEN1; //EN1 DAC CH1 Enable
}

/*
 * Puts sample to DAC 0-4095 corresponding to 0-3.3V
 * Parameters: uint16_t 12-bit value 0<value<4095
 * Returns: None
 */
void dac_put(uint16_t value)
{
    dac1[DAC_DHR12R1] = value; //CH1 HOLDING REG
}
