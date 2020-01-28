/*
 * uart_driver.c
 *
 *  Created on: Nov 8, 2016
 *      Author: barnekow
 */
#include "uart_driver.h"
#include "registers.h"
#include <inttypes.h>
#include <stdio.h>


static volatile uint32_t * const gpioa = (volatile uint32_t *) GPIOA_BASE;
static volatile uint32_t * const usart2 = (volatile uint32_t *) USART2_BASE;


void USART2_IRQHandler()
{
	//todo 
}


// These will override _read and _write in syscalls.c, which are
// prototyped as weak
int _read(int file, char *ptr, int len)
{
	int DataIdx;
	// Modified the for loop in order to get the correct behavior for fgets
	int byteCnt = 0;
	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		//*ptr++ = __io_getchar();
		byteCnt++;
		//*ptr++ = usart2_getch();
		*ptr = usart2_getch();
		if(*ptr == '\n') break;
		ptr++;
	}

	//return len;
	return byteCnt; // Return byte count
}

int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		usart2_putch(*ptr++);
	}
	return len;
}



char usart2_getch(){
	char c;
	while((usart2[USART_SR]&(1<<RXNE)) != (1<<RXNE));
	c = ((char) usart2[USART_DR]);  // Read character from usart
	usart2_putch(c);  // Echo back

	if (c == '\r'){  // If character is CR
		usart2_putch('\n');  // send it
		c = '\n';   // Return LF. fgets is terminated by LF
	}

	return c;
}

void usart2_putch(char c){
	while((usart2[USART_SR]&(1<<TXE)) != (1<<TXE));
	usart2[USART_DR] = c;
}

void init_usart2(uint32_t baud, uint32_t sysclk){
	// Enable clocks for GPIOA and USART2
	volatile uint32_t * const rcc = (volatile uint32_t * const) RCC_BASE;
	
	rcc[AHB1ENR] |= (1<<GPIOAEN);
	rcc[AHB1ENR] |= (1<<USART2EN);

	// Function 7 of PORTA pins is USART
	gpioa[AFLR] &= (0xFFFF00FF); // Clear the bits associated with PA3 and PA2
	gpioa[AFLR] |= (0b01110111<<8);  // Choose function 7 for both PA3 and PA2
	gpioa[MODER] &= (0xFFFFFF0F);  // Clear mode bits for PA3 and PA2
	gpioa[MODER] |= (0b1010<<4);  // Both PA3 and PA2 in alt function mode




	// Set up USART2
	//USART2_init();  //8n1 no flow control
	// over8 = 0..oversample by 16
	// M = 0..1 start bit, data size is 8, 1 stop bit
	// PCE= 0..Parity check not enabled
	// no interrupts... using polling
	usart2[CR1] = (1<<UE)|(1<<TE)|(1<<RE); // Enable UART, Tx and Rx
	usart2[USART_CR2] = 0;  // This is the default, but do it anyway
	usart2[USART_CR3] = 0;  // This is the default, but do it anyway
	usart2[USART_BRR] = sysclk/baud;

	/* I'm not sure if this is needed for standard IO*/
	 //setvbuf(stderr, NULL, _IONBF, 0);
	 //setvbuf(stdin, NULL, _IONBF, 0);
	 setvbuf(stdout, NULL, _IONBF, 0);
}

