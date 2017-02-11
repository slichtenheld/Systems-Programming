/* example of solving the producer consumer problems */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <semaphore.h> 

#define gettid() syscall(SYS_gettid)

#define BUFFERSIZE 5

/* global variables, is this the best way? */
sem_t m,f,e; //mutual exclusion, full, empty

int counter = 0;
#define COUNTMAX 20
#define PRODTHREADS 2
#define CONSTHREADS 2

struct circularBuffer{
	int in;
	int out;
	int numItems;
	int buffer[];
}; 


void * producer(void * arg) {
	struct circularBuffer *circbuf = (struct circularBuffer*) arg;
	while(1) {

		// down f // order matters, if down m first, deadlock
		sem_wait(&f);
		// down m
		sem_wait(&m);

		printf("THREAD %ld: adding %d to buffer, in: %d, numItems: %d\n", gettid(), counter,circbuf->in,circbuf->numItems);
		circbuf->buffer[circbuf->in] = counter++;
		circbuf->in  = (circbuf->in + 1) % BUFFERSIZE;
		circbuf->numItems++;

		// up(e) // order doesn't matter, ups don't block	 	
		sem_post(&e);
		// up(m) 
		sem_post(&m);

		if (counter > COUNTMAX) 
			raise(SIGTERM);
		
	}
}

void * consumer(void * arg) {
	struct circularBuffer *circbuf = (struct circularBuffer*) arg;
	while(1) { 

		// down(e)
		sem_wait(&e);
		// down(m)
		sem_wait(&m);

		printf("THREAD %ld: Removing %d from buffer\n", gettid(),circbuf->buffer[circbuf->out] );
		circbuf->out = (circbuf->out + 1) % BUFFERSIZE;
		circbuf->numItems --;

		// up(f)
		sem_post(&f);
		// up (m)
		sem_post(&m);
	}
}


/*****************************************************************/

int main (int argc, char* argv[]) {

	
	if (argc != 2) {
		perror("ERROR: ./program <size of buffer>");
		exit(-1);
	}

	char *endptr;
	int bufferlength = strtol(argv[1],&endptr,10);

	struct circularBuffer *cBuf = malloc(sizeof(struct circularBuffer) + bufferlength * sizeof(int)); 

	printf("Buffer is of size %d\n", bufferlength );
	


	/*make sure to initialize buffer*/
	
	/* declare threads, init semaphores */
	pthread_t prodThread[PRODTHREADS],consThread[CONSTHREADS]; 
	sem_init(&m,0,1); //mutual exclusion, must be 1 so that producer can acquire buffer first
	sem_init(&f,0,bufferlength); // full tokens 
	sem_init(&e,0,0); //empty tokens 

	for (int i = 0; i < PRODTHREADS; i++){
		printf("Creating producer[%d] thread\n",i);
		if ( pthread_create( &prodThread[i],NULL,producer,(void*)cBuf ) ) { 
			perror("error creating producer thread");
			exit(-1);
		}
	}
	for (int i = 0; i < CONSTHREADS; i++){
		printf("Creating consumer[%d] thread\n",i);
		if ( pthread_create( &consThread[i],NULL,consumer,(void*)cBuf ) ) { 
			perror("error creating consumer thread");
			exit(-1);
		}
	}

	//pthread_cond_wait(&consCond,&mtx);

	/* wait for all threads to terminate, destroy semaphores */
	pthread_exit(NULL); 
	free(cBuf);
	
	sem_destroy(&m);
	sem_destroy(&f);
	sem_destroy(&e);
}