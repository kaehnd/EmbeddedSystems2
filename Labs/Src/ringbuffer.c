/*
 * ringbuffer.c
 *
 *  Created on: Jan 20, 2020
 *      Author: kaehnd
 *
 *  	Implements functions and tests used with RingBuffer struct
 */

#include "ringbuffer.h"
#include <stdio.h>
#include <string.h>

/*
 * Puts a char in the specified RingBuffer
 * **Blocks if the RingBuffer is full
 * Parameters: RingBuffer * buffer, pointer to a RingBuffer struct
 * 			   char element, character to be added
 * Returns: none
 */
void put(RingBuffer * buffer, char element)
{
	while (!hasSpace(buffer));


	buffer->buffer[buffer->put] = element;
	buffer->put = (buffer->put + 1) % BUF_SIZE;	
}

void putNoBlock(RingBuffer * buffer, char element)
{
	if (hasSpace(buffer))
	{
		buffer->buffer[buffer->put] = element;
		buffer->put = (buffer->put + 1) % BUF_SIZE;	
	}
	//else drop
}



/*
 * Gets the oldest character from specified RingBuffer
 * **Blocks if there is nothing in the buffer
 * Parameters: RingBuffer * buffer, pointer to a RingBuffer struct
 * Returns: char, char gotten from buffer
 */
char get(RingBuffer * buffer)
{
	while (!hasElement(buffer));
	char toReturn = buffer->buffer[buffer->get];
	buffer->get = (buffer->get + 1) % BUF_SIZE;
	return toReturn;
}

/*
 * Returns whether the specified buffer is full
 * Parameters: RingBuffer * buffer, pointer to a RingBuffer struct
 * Returns: int, 0 if full, not 0 if not full
 */
int hasSpace(RingBuffer * buffer)
{
	return !(((buffer->put +1) % BUF_SIZE) == buffer->get);
}

/*
 * Returns whether the specified buffer has any element or if it is empty
 * Parameters: RingBuffer * buffer, pointer to RingBuffer struct
 * Returns: int, 0 if empty, not 0 if not empty
 */
int hasElement(RingBuffer * buffer)
{
	return !(buffer->put == buffer->get);
}

void backSpace(RingBuffer * buffer)
{
	if (hasElement(buffer))
	{
		buffer->put = (buffer->put -1) % BUF_SIZE;
		if (buffer->put == -1)
		{
			buffer->put = BUF_SIZE;
		}
	}
}



/***************Tests********************/

/*
 * Helper function that prints success and returns 0 for true condition
 * and prints fail and returns 1 for false condition
 * Parameters: int condition, condition to be asserted
 * Returns: int, number of errors (false conditions) encountered (0 or 1 for one call)
 */
static int assert(int condition)
{
	if (condition)
	{
		printf("Success\n\n");
		return 0;
	}
	else
	{
		printf("FAIL!!\n\n");
		return 1;
	}
}

//testHasSpaceTrueWhenNoElements
static int testHasSpaceTrueWhenNoElements(void)
{
	printf("Testing hasSpace when no elements exist...\n");
	RingBuffer buffer = {0, 0};

	return assert(hasSpace(&buffer));
}

//testHasSpaceTrueWhenHasElementsAndNotFull
static int testHasSpaceTrueWhenHasElementsAndNotFull(void)
{
	printf("Testing hasSpace when elements are present...\n");
	RingBuffer buffer = {0, 0};
	put(&buffer, 'C');
	return assert(hasSpace(&buffer));
}

//testHasSpaceTrueWhenFull
static int testHasSpaceTrueWhenFull(void)
{
	printf("Testing hasSpace when buffer is full...\n");
	RingBuffer buffer = {0, 0};
	for (int i = 0; i < BUF_SIZE-1; i++)
	{
		put(&buffer, 'C');
	}
	return assert(!hasSpace(&buffer));
}

//testHasSpaceTrueWhenFullWithGets
static int testHasSpaceTrueWhenFullWithGets(void)
{
	printf("Testing hasSpace when buffer is full and gets have been done...\n");
	RingBuffer buffer = {0, 0};
	for (int i = 0; i < BUF_SIZE-1; i++)
	{
		put(&buffer, 'C');
	}
	get(&buffer);
	put(&buffer, 'C');

	return assert(!hasSpace(&buffer));
}

//testHasSpaceFalseWhenWasFullButCharWasGotten
static int testHasSpaceFalseWhenWasFullButCharWasGotten(void)
{
	printf("Testing hasSpace when buffer is full and gets have been done...\n");
	RingBuffer buffer = {0, 0};
	for (int i = 0; i < BUF_SIZE-1; i++)
	{
		put(&buffer, 'C');
	}
	get(&buffer);
	return assert(hasSpace(&buffer));
}

//testHasElementFalseWhenNoElements
static int testHasElementFalseWhenNoElements(void)
{
	printf("Testing hasSpace when buffer is full and gets have been done...\n");
	RingBuffer buffer = {0, 0};
	return assert(!hasElement(&buffer));
}

//testHasElementTrueWhenHasElementsAndNotFull
static int testHasElementTrueWhenHasElementsAndNotFull(void)
{
	printf("Testing hasSpace when buffer is full and gets have been done...\n");
	RingBuffer buffer = {0, 0};
	put(&buffer, 'C');
	return assert(hasElement(&buffer));
}

//testHasElementTrueWhenHasElementsAndFull
static int testHasElementTrueWhenHasElementsAndFull(void)
{
	printf("Testing hasSpace when buffer is full and gets have been done...\n");
	RingBuffer buffer = {0, 0};
	for (int i = 0; i < BUF_SIZE-1; i++)
	{
		put(&buffer, 'C');
	}
	return assert(hasElement(&buffer));
}

//testHasElementFalseWhenHadElementsButCharWasGotten
static int testHasElementFalseWhenHadElementsButCharWasGotten(void)
{
	printf("Testing hasSpace when buffer is full and gets have been done...\n");
	RingBuffer buffer = {0, 0};
	put(&buffer, 'C');
	get(&buffer);
	return assert(!hasElement(&buffer));
}

//testPutCharsGetChars
static int testPutCharsGetChars(void)
{
	printf("Testing putting chars and getting chars returns correctly and in order...\n");
	const char testString[50] = "This is a test string";
	RingBuffer buffer = {0,0};
	int i = 0;
	while(testString[i] != '\0')
	{
		put(&buffer, testString[i++]);
	}
	static char recievedString[50] = "";

	i = 0;
	while(hasElement(&buffer))
	{
		recievedString[i++] = get(&buffer);
	}
	return assert(strcmp(recievedString, testString) == 0);
}

/*
 * Public function that runs all RingBuffer tests and shows results
 * Parameters: none
 * Returns: none
 */
void ringBufferTests(void)
{
	static const char red[] = "\x1b[31m";
	static const char green[] = "\x1b[32m";
	static const char clrRst[] = "\x1b[0m";

	printf("*********************************************"
			"\nStarting STM32 RingBuffer Test Suite...\n"
			"*********************************************\n\n");

	int numErrors = 0;
	numErrors += testHasSpaceTrueWhenNoElements();
	numErrors += testHasSpaceTrueWhenHasElementsAndNotFull();
	numErrors += testHasSpaceTrueWhenFull();
	numErrors += testHasSpaceTrueWhenFullWithGets();
	numErrors += testHasSpaceFalseWhenWasFullButCharWasGotten();
	numErrors += testHasElementFalseWhenNoElements();
	numErrors += testHasElementTrueWhenHasElementsAndNotFull();
	numErrors += testHasElementTrueWhenHasElementsAndFull();
	numErrors += testHasElementFalseWhenHadElementsButCharWasGotten();
	numErrors += testPutCharsGetChars();


	printf("*********************************************"
				"\nAll RingBuffer Tests Completed! \n"
				"\nTests finished with %s%d%s %s.\n"
				"*********************************************\n\n",
			(numErrors ? red : green), numErrors, clrRst, (numErrors > 1 || !numErrors ? "errors" : "error"));
}
