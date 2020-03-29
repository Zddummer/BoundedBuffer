/*
 * Header file for bounded-buffer.c
 *
 * CS 441/541: Bounded Buffer (Project 4)
 *
 * @author Zach Dummer
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>

#include "semaphore_support.h"

/*****************************
 * Defines
 *****************************/

/*****************************
 * Structures
 *****************************/


/*****************************
 * Global Variables
 *****************************/
int g_intBufferSize = 10;
int g_intTimeToLive = 0;
int g_intNumProducerThreads = 0;
int g_intNumConsumerThreads = 0;

int * g_arrBuffer;
semaphore_t g_semMutex;
semaphore_t g_semItems;

int g_intProducerIndex = 0;
int g_intTotalProduced = 0;
int g_intConsumerIndex = 0;
int g_intTotalConsumed = 0;

char *g_strProducerPointer = "^";
char *g_strConsumerPointer = "v";

/*****************************
 * Function Declarations
 *****************************/

/*
* Validation for the command line arguments
*
* param: argument count, array of arguments
*/
void parseCommandLineArgs(int argc, char * argv[]);

/*
* Checks if a string is a number
*
* param: string to check
*
* returns: true if string is a number
*/
bool isNumber(const char *strToCheck);

/*
* Prints and error message if invalid arguments
* are passed.
*/
void printError();

/*
* Prints the variables passed in as arguments
*/
void printGlobalVariables();

/*
* Initializes buffer
*/
void initBuffer();

/*
* Prints contents of the buffer
*/
void printBuffer();

/*
* Creates the specified number of producer threads
*/
void createProducerThreads();

/*
* Creates the specified number of consumer threads
*/
void createConsumerThreads();

/*
* Execution of the producer threads
*
* param: thread ID
*/
void *produce(void *threadId);

/*
* Execution of the consumer threads
*
* param: thread ID
*/
void *consume(void *threadId);

/*
* Gets a random number between 0 and 9
*
* returns: the random number
*/
int getRandomNumber();

/*
* Randomly sleeps the thread for 0 or 1 milliseconds
*
* returns: the random time slept
*/
int sleepRandomLength();

/*
* Adds an item to the buffer
*
* param: item to add
*/
int buffer_add(int item);

/*
* Removes an item from the buffer
*
* param: item to fill with removed item
*/
int buffer_get(int *item);

/*
* Prints the stats or the program and exits
*/
void printAndExit();