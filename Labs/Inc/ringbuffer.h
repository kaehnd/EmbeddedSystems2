/*
 * ringbuffer.h
 *
 *  Created on: Jan 20, 2020
 *      Author: kaehnd
 *
 *      Function definitions and typedef for ringbuffer.c and RingBuffer struct
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

//Max length of buffer is BUF_SIZE - 1 due to get and put location extremes
#define BUF_SIZE 50

//Definition of RingBuffer
typedef struct {
	unsigned int put; //index to put at
	unsigned int get; //index to get from
	char buffer[BUF_SIZE];
} RingBuffer;

/*
 * Puts a char in the specified RingBuffer
 * **Blocks if the RingBuffer is full
 * Parameters: RingBuffer * buffer, pointer to a RingBuffer struct
 * 			   char element, character to be added
 * Returns: none
 */
void put(RingBuffer * buffer, char element);

/*
 * Gets the oldest character from specified RingBuffer
 * **Blocks if there is nothing in the buffer
 * Parameters: RingBuffer * buffer, pointer to a RingBuffer struct
 * Returns: char, char gotten from buffer
 */
char get(RingBuffer * buffer);

/*
 * Returns whether the specified buffer is full
 * Parameters: RingBuffer * buffer, pointer to a RingBuffer struct
 * Returns: int, 0 if full, not 0 if not full
 */
int hasSpace(RingBuffer * buffer);

/*
 * Returns whether the specified buffer has any element or if it is empty
 * Parameters: RingBuffer * buffer, pointer to RingBuffer struct
 * Returns: int, 0 if empty, not 0 if not empty
 */
int hasElement(RingBuffer *);

/*
 * Public function that runs all RingBuffer tests and prints results to stdout
 * Parameters: none
 * Returns: none
 */
void ringBufferTests(void);

#endif /* RINGBUFFER_H_ */
