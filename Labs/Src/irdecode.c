#include "registers.h"
#include "delays.h"
#include <stdio.h>
#include <stdint.h>

#define HALF_TICKS_S 8000000 


static volatile uint32_t * const gpiob = (uint32_t *) GPIOB_BASE;
static volatile uint32_t * const timer2 = (uint32_t *) TIM2_BASE;
static volatile uint32_t * exti = (uint32_t *) EXTI_BASE;
static volatile uint32_t * nvic = (uint32_t *) NVIC_BASE;

static int dataValid = 0;
static uint16_t data = 0;
static int bitsRead = 0;
static enum {WAIT, HDR_LOW, HDR_HIGH, RCV_LOW, RCV_HIGH, PAUSE_LOW, PAUSE_HIGH, ERROR} state;

int getIRDataValid() {
	return dataValid;
}

uint16_t getIRData() {
	return data;
}

void irDecodeInit() {
    volatile uint32_t * const rcc = (uint32_t *) RCC_BASE;

	//enable RCC for GPIOB
	rcc[AHB1ENR] |= 1<<GPIOBEN;

	gpiob[MODER] |= INPUT<<MODER2;
	gpiob[PUPDR] |= PULL_UP << PUPDR2;



	//enable RCC for Timer 2
	rcc[APB1ENR] |= 1<<TIM2EN;

	exti[IMR] |= 1<<2;

	exti[RTSR] |= 1<<2;
	exti[FTSR] |= 1<<2;

	rcc[APB2ENR] |= 1<<14;


	uint32_t * syscfg = (uint32_t *) SYSCFG_BASE;
	syscfg[SYSCFG_EXTICR1] |= 1<<8;





	nvic[ISER0] |= 1<<8;

	//enable
	timer2[PSC] = 0x0F;
	timer2[EGR] = 1;
	timer2[ARR] = 0xFFFFFFFF;
	timer2[CR1] = 1;
	timer2[SR] = 0;
}

void timer_reset() {
	timer2[SR] = 0;
	timer2[CNT] = 0;
}

int getTime() {
	return timer2[CNT];
}

int readIR(void) {
	return (gpiob[IDR] >>2) & 0x01;
}

void EXTI2_IRQHandler() {
	int time = getTime();
	exti[PR] = 1<<2;
	nvic[ICPR0] = 1<<8;

	switch (state) {

		case WAIT:
			if (readIR() == 0){
				state = HDR_LOW;
			} else {
				state = ERROR;
			}
			break;

		case HDR_LOW:
			if (readIR() == 1) {
				state = HDR_HIGH;
			} else {
				state = ERROR;
			}
			break;

		case HDR_HIGH:
			if (readIR() == 0) {
				state = RCV_LOW;
			} else {
				state = ERROR;
			}
			break;

		case RCV_LOW:
			if (readIR() ==1) {
				state = RCV_HIGH;
				timer_reset();
			} else {
				state = ERROR;
			}
			break;

		case RCV_HIGH:
			if (readIR() == 0) {
				if (time < 650) {
					data = (data<<1) | 0;
				} else {
					data = (data<<1) | 1;
				}
				
				bitsRead++;

				if (bitsRead == 8) {
					state = PAUSE_LOW;
				} else if (bitsRead >= 16) {
					state = WAIT;
					bitsRead = 0;
				} else {
					state = RCV_LOW;
				}
			} else {
				state = ERROR;
			}
			break;

		case PAUSE_LOW:
			if (readIR() == 1) {
				state = PAUSE_HIGH;
			} else {
				state = ERROR;
			}
			break;


		case PAUSE_HIGH:
			if (readIR() == 0) {
				state = RCV_LOW;
			} else {
				state = ERROR;
			}
			break;

		case ERROR:
			data = 0;
			bitsRead = 0;
			return;

		default: 
			state = ERROR;


	}
	
}

void TIM2_IRQHandler() {
	timer2[SR] = 0;
}

