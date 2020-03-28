/*
 * 
 *
 * CS 441/541: Bounded Buffer (Project 4)
 *
 * @author Zach Dummer
 *
 */
#include "bounded-buffer.h"

int main(int argc, char * argv[])
{
	printf("--Start--\n");

	parseCommandLineArgs(argc, argv);
	printGlobalVariables();
	initBuffer();

	printf("Initial Buffer:                      ");
	printBuffer();

	createProducerThreads();
	createConsumerThreads();
	
	sleep(g_intTimeToLive);

	printAndExit();
	printf("--Finish--\n");
    return 0;
}

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

void printGlobalVariables()
{
	printf("Buffer Size               :   %d\n", g_intBufferSize);
	printf("Time To Live (seconds)    :   %d\n", g_intTimeToLive);
	printf("Number of Producer Threads:   %d\n", g_intNumProducerThreads);
	printf("Number of Consumer Threads:   %d\n", g_intNumConsumerThreads);
	printf("-------------------------------\n");
}

void initBuffer()
{
	g_oBuffer = malloc(sizeof(buffer) + g_intBufferSize);

	semaphore_create(&g_oBuffer->mutex, 1);

	int index;
	for(index = 0; index < g_intBufferSize; index++)
	{
		g_oBuffer->arrBufferArray[index] = -1;
	}

}

void printBuffer()
{
	printf("[");

	int index;
	for(index = 0; index < g_intBufferSize; index++)
	{
		printf(" %d", g_oBuffer->arrBufferArray[index]);

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

void *produce(void *threadId)
{
	int intRandomNumber = getRandomNumber();

	semaphore_wait(&g_oBuffer->mutex);

	int intSleepLength = sleepRandomLength();
	printf("Producer  %d: ", intSleepLength);

	buffer_add(intRandomNumber);
	printf("Total   %d, Value  %d     ", g_intTotalProduced, intRandomNumber);
	printBuffer();

	semaphore_post(&g_oBuffer->mutex);

	pthread_exit(NULL);
}

void *consume(void *threadId)
{
	// int tid = (intptr_t)threadId;
	// printf("Consumer thread: #%d\n", tid);

	pthread_exit(NULL);
}

int getRandomNumber()
{
	return rand() % 10;
}

int sleepRandomLength()
{
	int intRandomNumber = rand() % 2;

	sleep(intRandomNumber);

	return intRandomNumber;
}

int buffer_add(int item)
{
	g_oBuffer->arrBufferArray[g_intProducerIndex] = item;
	g_intProducerIndex++;
	g_intTotalProduced++;

	if(g_intProducerIndex == g_intBufferSize)
	{
		g_intProducerIndex = 0;
	}

	return 0;
}

int buffer_get(int item)
{
	return 0;
}

void printAndExit()
{
	printf("-----------+---------\n");
	printf("Produced   |   %d\n", g_intTotalProduced);
	printf("Consumed   |   %d\n", g_intTotalConsumed);
	printf("-----------+---------\n");

	exit(0);
}