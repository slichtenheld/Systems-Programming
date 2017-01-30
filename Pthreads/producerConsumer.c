/* example of solving the producer consumer problems */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BufferSize

struct circularBuffer{
	int in;
	int out;
	int numItems;
	int buffer[BufferSize];
};

void * producer(void * arg) {
	
}

void * consumer(void * arg) {

}

int main (int argc, char* argv[]) {

	if (argc != 2) {
		perror("ERROR: ./program <size of buffer>");
		exit(-1);
	}

	char *endptr;
	int bufferlength = strtol(argv[1],&endptr,10);

	printf("Buffer is of size %d\n", bufferlength );

	struct circularBuffer cBuf;
	pthread_t prodThread,consThread;

	printf("Creating producer thread\n");
	if ( pthread_create( &prodThread,NULL,producer,(void*)&cBuf ) ) { 
		perror("error creating producer thread");
		exit(-1);
	}

	printf("Creating consumer thread\n");

	if ( pthread_create( &consThread,NULL,consumer,(void*)&cBuf ) ) { 
		perror("error creating consumer thread");
		exit(-1);
	}

	pthread_exit(NULL); /* wait for all threads to terminate */
}