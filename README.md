# CS441/541 Project 4

## Author(s):

Zach Dummer


## Date:

3/28/2020


## Description:

This program is an implementation of the producer-consumer finite buffer solutions
found in the Little Book of Semaphores.


## How to build the software

To build the software from the commandline:
	1) Navigate to the directory containing the Makefile
	2) Type "make"
	3) hit the "enter" key


## How to use the software

To run the program:
	1) Navigate to the same directory you built the software from
	2) Type ./bounded-buffer followed by 3 or 4 arguments
		- <integer> the ammount time the program should run
		- <integer> the number of Producer threads
		- <integer> the number of Consumer threads
		- (optional)<integer> the size of the buffer
	3) hit the "enter" key

	Example:
	[user@path]$ ./bounded-buffer 2 2 1 4
	 This will run the program for 2 seconds with 1 producer thread, 1 consumer thread, and a buffer size of 4.
	 The default buffer size is 10 if not declared.


## How the software was tested

	This program was tested with a small ammount of buffers and threads at first which gradually increased.
I checked each line the make sure the consumers never got ahead of the producers, and never tried to consume
an item that hadn't been produced yet.

	Since consumers can never get ahead of the producers, I tested the program with more and more
consumer threads and less producer threads to ensure the number of items produced was never lower
than the number of items consumed, even with many more consumer threads than producer threads.


## Known bugs and problem areas

There are no know bugs at this time.

If you find a bug, please raise an issue @ https://bitbucket.org/zdummer/cs441-bounded-buffer/src/master/