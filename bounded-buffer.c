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

	printf("Initial Buffer:                     ");
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
	g_arrBuffer = malloc(sizeof(int) * g_intBufferSize);

	int index;
	for(index = 0; index < g_intBufferSize; index++)
	{
		g_arrBuffer[index] = -1;
	}

	semaphore_create(&g_semMutex, 1);
	semaphore_create(&g_semItems, 0);

}

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

int getRandomNumber()
{
	return rand() % 10;
}

int sleepRandomLength()
{
	int intRandomNumber = rand() % 2;

	usleep(intRandomNumber * 100000);

	return intRandomNumber;
}

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

void printAndExit()
{
	printf("-----------+---------\n");
	printf("Produced   |   %d\n", g_intTotalProduced);
	printf("Consumed   |   %d\n", g_intTotalConsumed);
	printf("-----------+---------\n");

	exit(0);
}