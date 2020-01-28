/*
 * uart_driver.h
 *
 *  Created on: Nov 8, 2016
 *      Author: barnekow
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include <inttypes.h>


#define GPIOAEN 0		// GPIOA Enable is bit 0 in RCC_APB1LPENR
#define USART2EN 17  // USART2 enable is bit 17 in RCC_AHB1LPENR



// CR1 bits
#define UE 13 //UART enable
#define TXEIE 7 //TXE interrupt enable
#define RXNEIE 5 //RXNE Interrupt Enabvle
#define TE 3  // Transmitter enable
#define RE 2  // Receiver enable


// Status register bits
#define TXE 7  // Transmit register empty
#define RXNE 5  // Receive register is not empty..char received

// Function prototypes
extern void init_usart2(uint32_t baud, uint32_t sysclk);
extern char usart2_getch();
extern void usart2_putch(char c);

// syscalls overrides
int _read(int file, char *ptr, int len);
int _write(int file, char *ptr, int len);


#endif /* UART_DRIVER_H_ */
