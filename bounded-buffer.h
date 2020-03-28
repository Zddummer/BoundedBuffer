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
void parseCommandLineArgs(int argc, char * argv[]);

bool isNumber(const char *strToCheck);

void printError();

void printGlobalVariables();

void initBuffer();

void printBuffer();

void createProducerThreads();

void createConsumerThreads();

void *produce(void *threadId);

void *consume(void *threadId);

int getRandomNumber();

int sleepRandomLength();

int buffer_add(int item);

int buffer_get(int *item);

void printAndExit();