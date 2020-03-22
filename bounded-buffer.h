/*
 * 
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
typedef struct buffer {
	int intMutex;
	int arrBufferArray[];
} buffer;

/*****************************
 * Global Variables
 *****************************/
int g_intBufferSize = 10;
int g_intTimeToLive = 0;
int g_intNumProducerThreads = 0;
int g_intNumConsumerThreads = 0;

buffer *g_oBuffer;

/*****************************
 * Function Declarations
 *****************************/
void parseCommandLineArgs(int argc, char * argv[]);
bool isNumber(const char *strToCheck);
void printError();
void printGlobalVariables();
void initBuffer();
void printBuffer();