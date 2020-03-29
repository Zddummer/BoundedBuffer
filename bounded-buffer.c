/*
 * This program is an implementation of the producer-consumer finite buffer solutions
 * found in the Little Book of Semaphores.
 *
 * CS 441/541: Bounded Buffer (Project 4)
 *
 * @author Zach Dummer
 *
 */
#include "bounded-buffer.h"

/* Starting point for the program */
int main(int argc, char * argv[])
{

	/* Valid arguments from the commandline */
	parseCommandLineArgs(argc, argv);

	/* Print out the set up from the arguments passed in */
	printGlobalVariables();

	/* Buffer setup */
	initBuffer();
	printf("Initial Buffer:                     ");
	printBuffer();

	/* create the specified number of threads */
	createProducerThreads();
	createConsumerThreads();
	
	/* Sleep for the specified ammount of time before exiting */
	sleep(g_intTimeToLive);

	/* Print out the number of items produced and consumer and exits */
	printAndExit();
    return 0;
}

/*
* Checks if the right ammount of arguments where passed in,
* validates those arguments, and populates global variables.
*/
void parseCommandLineArgs(int argc, char * argv[])
{

	if(argc != 4 && argc != 5)
	{
		printError();
	}

	int index;
	for(index = 1; index < argc; index++)
	{

		if(isNumber(argv[index]))
		{
			switch(index)
			{
				case 1 :
					g_intTimeToLive = atoi(argv[index]);
					if( g_intTimeToLive <= 0)
					{
						printError();
					}
					break;
				case 2 :
					g_intNumProducerThreads = atoi(argv[index]);
					if( g_intNumProducerThreads <= 0)
					{
						printError();
					}
					break;
				case 3 :
					g_intNumConsumerThreads = atoi(argv[index]);
					if( g_intNumConsumerThreads <= 0)
					{
						printError();
					}
					break;
				case 4 :
					g_intBufferSize = atoi(argv[index]);
					if( g_intBufferSize <= 0)
					{
						printError();
					}
					break;
				default :
					break;
			}
		}
		else
		{
			printError();
		}
	}


}

/*
* Checks the string passed in can be converted to a number
*/
bool isNumber (const char *strToCheck) { 
    while (*strToCheck) { 
		if (*strToCheck < '0' || *strToCheck > '9')
		{
			return 0;
		} 
		strToCheck++; 
    } 
    return 1; 
}

/*
* Displays a message for the user if invalid arguments where passed in
*/
void printError()
{
	printf("ERROR: Invalid number of arguments\n");
	printf("-------------------------------------------------------\n");
	printf("Please enter 3 or 4 integers greater than zero:\n");
	printf("\t<integer> - Time to live (seconds)\n");
	printf("\t<integer> - Number of producer threads\n");
	printf("\t<integer> - Number of consumer threads\n");
	printf("\t(optional) <integer> - Buffer size (defaults to 10)\n");
	printf("-------------------------------------------------------\n");

	exit(0);
}

/*
* Displays the initial set up for the program
*/
void printGlobalVariables()
{
	printf("Buffer Size               :   %d\n", g_intBufferSize);
	printf("Time To Live (seconds)    :   %d\n", g_intTimeToLive);
	printf("Number of Producer Threads:   %d\n", g_intNumProducerThreads);
	printf("Number of Consumer Threads:   %d\n", g_intNumConsumerThreads);
	printf("-------------------------------\n");
}

/*
* Initializes the buffer and creates the global semaphores
*/
void initBuffer()
{
	g_arrBuffer = malloc(sizeof(int) * g_intBufferSize);

	int index;
	for(index = 0; index < g_intBufferSize; index++)
	{
		g_arrBuffer[index] = -1;
	}

	semaphore_create(&g_semMutex, 1);
	semaphore_create(&g_semItems, 0);

}

/*
* Prints the contents of the buffer and the location of the next
* place to be produced and consumed
*/
void printBuffer()
{
	printf("[");

	int index;
	for(index = 0; index < g_intBufferSize; index++)
	{
		printf(" %d", g_arrBuffer[index]);

		if(index == g_intProducerIndex)
		{
			printf("%s", g_strProducerPointer);
		}

		if (index == g_intConsumerIndex)
		{
			printf("%s", g_strConsumerPointer);
		}
	}

	printf("]\n");
}

/*
* Creates the producer threads withs a call to the produce method
*/
void createProducerThreads()
{
	pthread_t arrThreads[g_intNumProducerThreads];
	int intReturnCode;
	int index;

	for(index = 0; index < g_intNumProducerThreads; index++)
	{
		intReturnCode = pthread_create(&arrThreads[index], NULL, produce, (void *)(intptr_t)index);

		if(0 != intReturnCode)
		{
			printf("ERROR when creating thread!\n");
			exit(-1);
		}
	}
}

/*
* Creates the consumer threads withs a call to the consume method
*/
void createConsumerThreads()
{
	pthread_t arrThreads[g_intNumConsumerThreads];
	int intReturnCode;
	int index;

	for(index = 0; index < g_intNumConsumerThreads; index++)
	{
		intReturnCode = pthread_create(&arrThreads[index], NULL, consume, (void *)(intptr_t)index);

		if(0 != intReturnCode)
		{
			printf("ERROR when creating thread!\n");
			exit(-1);
		}
	}
}

/*
* Produces items for the buffer as discribed in the Little Book of Semaphores
*/
void *produce(void *threadId)
{
	while (true)
	{
		semaphore_wait(&g_semMutex);

		int intRandomNumber = getRandomNumber();
		int intSleepLength = sleepRandomLength();

		buffer_add(intRandomNumber);
		printf("Producer:  %d: Total %3d, Value  %d   ", intSleepLength, g_intTotalProduced, intRandomNumber);
		printBuffer();

		semaphore_post(&g_semMutex);
		semaphore_post(&g_semItems);
	}

	pthread_exit(NULL);
}

/*
* Consumes items from the buffer as discribed in the Little Book of Semaphores
*/
void *consume(void *threadId)
{
	while (true)
	{
		semaphore_wait(&g_semItems);
		semaphore_wait(&g_semMutex);

		int intNumberFromBuffer;
		int intSleepLength = sleepRandomLength();

		buffer_get(&intNumberFromBuffer);
		printf("Consumer:  %d: Total %3d, Value  %d   ", intSleepLength, g_intTotalConsumed, intNumberFromBuffer);
		printBuffer();

		semaphore_post(&g_semMutex);
	}

	pthread_exit(NULL);
}

/*
* Creates and returns a random number between 0 and 9
*/
int getRandomNumber()
{
	return rand() % 10;
}

/*
* Randomly sleeps for 1 or 0 milliseconds
*/
int sleepRandomLength()
{
	int intRandomNumber = rand() % 2;

	usleep(intRandomNumber * 100000);

	return intRandomNumber;
}

/*
* Adds and item to the buffer
*/
int buffer_add(int item)
{
	g_arrBuffer[g_intProducerIndex] = item;

	g_intTotalProduced++;
	g_intProducerIndex++;

	if(g_intProducerIndex == g_intBufferSize)
	{
		g_intProducerIndex = 0;
	}
	return 0;
}

/*
* Removes and item from the buffer and sets the item passed in to the item removed
*/
int buffer_get(int *item)
{
	*item = g_arrBuffer[g_intConsumerIndex];
	g_arrBuffer[g_intConsumerIndex] = -1;

	g_intTotalConsumed++;
	g_intConsumerIndex++;

	if(g_intConsumerIndex == g_intBufferSize)
	{
		g_intConsumerIndex = 0;
	}
	return 0;
}

/*
* Prints the final statistics of the program and exits
*/
void printAndExit()
{
	printf("-----------+---------\n");
	printf("Produced   |   %d\n", g_intTotalProduced);
	printf("Consumed   |   %d\n", g_intTotalConsumed);
	printf("-----------+---------\n");

	exit(0);
}