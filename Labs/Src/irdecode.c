#include "registers.h"
#include "delays.h"
#include <stdio.h>
#include <stdint.h>

#define HDR_MIN 8000
#define HDR_MAX 9000
#define RCV0_MAX 650

static volatile uint32_t * const gpiob = (uint32_t *) GPIOB_BASE;
static volatile uint32_t * const timer2 = (uint32_t *) TIM2_BASE;
static volatile uint32_t * exti = (uint32_t *) EXTI_BASE;
static volatile uint32_t * nvic = (uint32_t *) NVIC_BASE;

static int dataValid = 0;
static uint16_t data = 0;
static int bitsRead = 0;
static enum {WAIT, HDR_LOW, HDR_HIGH, RCV_LOW, SND_HIGH, PAUSE_LOW, PAUSE_HIGH} state;

int getIRDataValid() {
	return dataValid;
}

uint16_t getIRData() {
	return data;
}

void setIRDataClear() {
	dataValid = 0;
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

void errorHandler() {
	data = 0;
	bitsRead = 0;
	state = WAIT;
}

void EXTI2_IRQHandler() {
	int time = getTime();
	exti[PR] = 1<<2;
	nvic[ICPR0] = 1<<8;

	if (dataValid) return;

	switch (state) {

		case WAIT:
			if (readIR() == 0){
				state = HDR_LOW;
				timer_reset();
			} else {
				errorHandler();
			}
			break;

		case HDR_LOW:
			if (readIR() == 1 && time > HDR_MIN && time < HDR_MAX) {
				state = HDR_HIGH;
			} else {
				errorHandler();
			}
			break;

		case HDR_HIGH:
			if (readIR() == 0) {
				state = RCV_LOW;
			} else {
				errorHandler();
			}
			break;

		case RCV_LOW:
			if (readIR() ==1) {
				state = SND_HIGH;
				timer_reset();
			} else {
				errorHandler();
			}
			break;

		case SND_HIGH:
			if (readIR() == 0) {
				if (time < RCV0_MAX) {
					data = (data<<1) | 0;
				} else {
					data = (data<<1) | 1;
				}
				
				bitsRead++;

				if (bitsRead == 8) {
					state = PAUSE_LOW;
				} else if (bitsRead >= 16) {
					state = WAIT;
					dataValid = 1;
					bitsRead = 0;
				} else {
					state = RCV_LOW;
				}
			} else {
				errorHandler();
			}
			break;

		case PAUSE_LOW:
			if (readIR() == 1) {
				state = PAUSE_HIGH;
			} else {
				errorHandler();
			}
			break;


		case PAUSE_HIGH:
			if (readIR() == 0) {
				state = RCV_LOW;
			} else {
				errorHandler();
			}
			break;

		default: 
			errorHandler();
			break;
	}
}



