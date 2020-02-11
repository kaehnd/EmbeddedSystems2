/*
 * uart_driver.c
 *
 *  Created on: Nov 8, 2016
 *      Author: barnekow
 * 		Edit: Daniel Kaehn
 * 	
 * 		UART driver edited for use with UART interrupts
 */
#include "uart_driver.h"
#include "registers.h"
#include "ringbuffer.h"
#include <inttypes.h>
#include <stdio.h>

//peripheral pointers
static volatile uint32_t * const gpioa = (volatile uint32_t *) GPIOA_BASE;
static volatile uint32_t * const usart2 = (volatile uint32_t *) USART2_BASE;

//I/O buffers
static RingBuffer  bufIn = {0, 0};
static RingBuffer  bufOut = {0, 0};

/*
 * Gets from the input ringBuffer
 * Blocks
 * Parameters: none
 * Returns: char gotten from buffer
 */
static char bufGet();


/*
 * Puts to the output ringBuffer
 * Blocks
 * Parameters: char to be put to buffer
 * Returns: none
 */
static void bufPut(char c);


/*
 * Initializes Usart2 Peripheral to be used with serial console and interrupts
 * Call to this function enables all console I/O
 * Parameters: uint32_t baud, the baud rate desired of the console
 *			   uint32_sysclk, the system clock rate
 * Returns: none
 */
void init_usart2(uint32_t baud, uint32_t sysclk) {
	// Enable clocks for GPIOA and USART2
	volatile uint32_t * const rcc = (volatile uint32_t * const) RCC_BASE;
	
	rcc[AHB1ENR] |= (1<<GPIOAEN);
	rcc[APB1ENR] |= (1<<USART2EN);

	// Function 7 of PORTA pins is USART
	gpioa[AFRL] &= (0xFFFF00FF); // Clear the bits associated with PA3 and PA2
	gpioa[AFRL] |= (0b01110111<<8);  // Choose function 7 for both PA3 and PA2
	gpioa[MODER] &= (0xFFFFFF0F);  // Clear mode bits for PA3 and PA2
	gpioa[MODER] |= (0b1010<<4);  // Both PA3 and PA2 in alt function mode

	//Set up interrupt in NVIC
	volatile uint32_t * const nvic = (volatile uint32_t * const) NVIC_BASE;
	
	// Set up USART2
	//USART2_init();  //8n1 no flow control
	// over8 = 0..oversample by 16
	// M = 0..1 start bit, data size is 8, 1 stop bit
	// PCE= 0..Parity check not enabled
	// Using TXE and RXNE interrupts
	usart2[USART_CR1] = (1<<UE)|(1<<TE)|(1<<RE); // Enable UART, Tx and Rx
	usart2[USART_CR2] = 0;  // This is the default, but do it anyway
	usart2[USART_CR3] = 0;  // This is the default, but do it anyway
	usart2[USART_BRR] = sysclk/baud;

	/* I'm not sure if this is needed for standard IO*/
	 //setvbuf(stderr, NULL, _IONBF, 0);
	 //setvbuf(stdin, NULL, _IONBF, 0);
	 setvbuf(stdout, NULL, _IONBF, 0);

	//enable read interrupt as final step
	usart2[USART_CR1] |=(1<<RXNEIE);
	nvic[ISER1] |= (1<<USART_2_NVIC_POS); 
}

/*
 * Handles all USART2 Line interrupts
 * Writes to DR when TXIE = 1 and there is something in bufOut
 * Turns off transmit interrupt when output buffer is full
 * Reads from DR to bufIN when RXE = 1
 */
void USART2_IRQHandler()
{
	uint32_t sr = usart2[USART_SR];
	if (sr & (1<<TXE))
	{
		if (hasElement(&bufOut))
		{
			usart2[USART_DR] = get(&bufOut);
		}
		else 
		{
			usart2[USART_CR1] &= ~(1<<TXEIE);
			usart2[USART_DR] = 0;
		}
	}
	if (sr & (1<<RXNE))
	{
		char readChar = usart2[USART_DR];
		//check status of both buffers so what user sees and what happens align
		if (readChar == '\b')
		{
			if (hasSpace(&bufOut))
			{
				bufPut('\b');
				bufPut(' ');
				bufPut('\b');
				put(&bufIn, readChar);
			}
		}
		else if (hasSpace(&bufIn) && hasSpace(&bufOut))
		{
			put(&bufIn, readChar);		
			bufPut(readChar); //echo
		}
	}
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
		//*ptr++ = bufGet();
		*ptr = bufGet();
		if (*ptr == '\b') 
		{
			*ptr = '\0';
			ptr--;
			byteCnt --;
		}
		else 
		{
			if(*ptr == '\n') break;
			ptr++;
		}
	}

	//return len;
	return byteCnt; // Return byte count
}

//system-called _write function
int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		bufPut(*ptr++);
	}
	return len;
}

/*
 * Gets from the input ringBuffer
 * Blocks
 * Parameters: none
 * Returns: char gotten from buffer
 */
static char bufGet()
{
	char c = get(&bufIn);	
	if (c == '\r'){  // If character is CR
		bufPut('\n');  // send it
		c = '\n';   // Return LF. fgets is terminated by LF
	}
	return c;
}

/*
 * Puts to the output ringBuffer
 * Blocks
 * Parameters: char to be put to buffer
 * Returns: none
 */
static void bufPut(char c)
{
	usart2[USART_CR1] |= (1<<TXEIE);
	put(&bufOut, c); //will block if full
}

