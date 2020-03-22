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
	printBuffer();

	printf("--Finish--\n");
    return 0;
}

void parseCommandLineArgs(int argc, char * argv[])
{

	if(argc != 4 && argc != 5)
	{
		printf("argc : %d\n", argc);
		printError();
		exit(0);
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
					break;
				case 2 :
					g_intNumProducerThreads = atoi(argv[index]);
					break;
				case 3 :
					g_intNumConsumerThreads = atoi(argv[index]);
					break;
				case 4 :
					g_intBufferSize = atoi(argv[index]);
					break;
				default :
					break;
			}
		}
		else
		{
			printError();
			exit(0);
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
	printf("Please enter 3 or 4 integers:\n");
	printf("\t<integer> - Time to live (seconds)\n");
	printf("\t<integer> - Number of producer threads\n");
	printf("\t<integer> - Number of consumer threads\n");
	printf("\t(optional) <integer> - Buffer size (defaults to 10)\n");
	printf("-------------------------------------------------------\n");
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
	}

	printf("]\n");
}
