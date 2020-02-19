/*
 * registers.h
 *
 *  Created on: Dec 17, 2019
 *      Author: kaehnd
 */

#ifndef REGISTERS_H_
#define REGISTERS_H_

#define SYS_CLK 16000000

// RCC Peripheral
#define RCC_BASE 0x40023800

// RCC Regs
#define AHB1ENR  12
#define APB1ENR 16
#define APB2ENR 17

#define TIM3EN 1

//AHB1ENR
#define GPIOAEN	0
#define GPIOBEN	1
#define GPIOCEN	2
#define GPIODEN	3
#define GPIOEEN	4
#define GPIOFEN	5
#define GPIOGEN	6
#define GPIOHEN	7
#define CRCEN	12
#define BKPSRAMEN	18
#define DMA1EN	21
#define DMA2EN	22
#define OTGHSEN	29
#define OTGHSULPIEN	30

//APB2ENR
#define ADCEN 8
#define SYSCFGEN 14

//APB1ENR
#define TIM5EN 3
#define DACEN 29


// GPIO peripherals
#define GPIOA_BASE 0x40020000
#define GPIOB_BASE 0x40020400
#define GPIOC_BASE 0x40020800
#define GPIOD_BASE 0x40020C00
#define GPIOE_BASE 0x40021000
#define GPIOF_BASE 0x40021400
#define GPIOG_BASE 0x40021800
#define GPIOH_BASE 0x40021C00


// GPIO registers
#define MODER 0
#define OTYPER 1
#define OSPEEDR 2
#define PUPDR 3
#define IDR 4
#define ODR 5
#define BSRR 6
#define LCKR 7
#define AFRL 8
#define AFRH 9

// GPIO Bits
#define DR0 0
#define DR1 1
#define DR2 2
#define DR3 3
#define DR4 4
#define DR5 5
#define DR6 6
#define DR7 7
#define DR8 8
#define DR9 9
#define DR10 10
#define DR11 11
#define DR12 12
#define DR13 13
#define DR14 14
#define DR15 15

// GPIO MODER settings and bits
#define INPUT 0
#define OUTPUT 1
#define ALTERNATE 2
#define ANALOG 3

#define MODER0 0
#define MODER1 2
#define MODER2 4
#define MODER3 6
#define MODER4 8
#define MODER5 10
#define MODER6 12
#define MODER7 14
#define MODER8 16
#define MODER9 18
#define MODER10 20
#define MODER11 22
#define MODER12 24
#define MODER13 26
#define MODER14 28

//GPIO Alternate Function bits
#define AF0 0
#define AF1 1
#define AF2 2
#define AF3 3
#define AF4 4
#define AF5 5
#define AF6 6
#define AF7 7
#define AF8 8
#define AF9 9
#define AF10 10
#define AF11 11
#define AF12 12
#define AF13 13
#define AF14 14
#define AF15 15

//Alternate function low register
#define AFLR0 0
#define AFLR1 4
#define AFLR2 8
#define AFLR3 12
#define AFLR4 16
#define AFLR5 20
#define AFLR6 24
#define AFLR7 28

//Alternate funtion high register
#define AFHR8 0
#define AFHR9 4
#define AFHR10 8
#define AFHR11 12
#define AFHR12 16
#define AFHR13 20
#define AFHR14 24
#define AFHR15 28

#define MODER15 30

// GPIO PUPDR settings and bits
#define NO_PU_PD 0
#define PULL_UP 1
#define PULL_DOWN 2
#define RESERVED 3

#define PUPDR0 0
#define PUPDR1 2
#define PUPDR2 4
#define PUPDR3 6
#define PUPDR4 8
#define PUPDR5 10
#define PUPDR6 12
#define PUPDR7 14
#define PUPDR8 16
#define PUPDR9 18
#define PUPDR10 20
#define PUPDR11 22
#define PUPDR12 24
#define PUPDR13 26
#define PUPDR14 28
#define PUPDR15 30





//Systick peripheral
#define STK_BASE 0xE000E010
#define STK_CTRL 0
#define STK_LOAD 1
#define STK_VAL 2
#define STK_CALIB 3


//Timer peripheral bases
#define TIM3_BASE 0x40000400
#define TIM5_BASE 0x40000C00
#define TIM8_BASE 0x40010400


//Timer registers
#define CR1 0
#define CCMR1 6
#define CCER 8
#define ARR 11
#define CCR1 13



#define NVIC_BASE 0xE000E100
#define ISER0 0
#define ISER1 1

#define USART_2_NVIC_POS 6
#define ICPR0 96 //0x180 bytes, 96d 32-bit words

#define USART2_BASE 0x40004400

#define USART_SR 0
#define USART_DR 1
#define USART_BRR 2
#define USART_CR1 3
#define USART_CR2 4
#define USART_CR3 5
#define USART_GTPR 6


#define ADC1_BASE 0x40012000
#define ADC_SR 0
#define ADC_CR1 1
#define ADC_CR2 2
#define ADC_SQR3 13
#define ADC_DR 19


#define DAC1_BASE 0x40007400
#define DAC_CR 0
#define DAC_SWTRIGR 1
#define DAC_DHR12R1 2
#define DAC_DHR12R2 5


//External interrupt values
#define EXTI_BASE 0x40013C00
#define EXTI_IMR 0
#define EXTI_FTSR 3
#define EXTI_PR 5

//sytem config values
#define SYSCFG_BASE 0x40013800
#define SYSCFG_MEMRMP 0
#define SYSCFG_PMC 1
#define SYSCFG_EXTICR1 2
#define SYSCFG_EXTICR2 3
#define SYSCFG_EXTICR3 4
#define SYSCFG_EXTICR4 5
#define SYSCFG_CMPCR 6
#define SYSCFG_CFGR 7






#endif /* REGISTERS_H_ */
