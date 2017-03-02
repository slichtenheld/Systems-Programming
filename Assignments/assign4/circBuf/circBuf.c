/* Created by SAMUEL LICHTENHELD */
 
/*********************************
 NOTES
	- 2/14/17 added semaphores to make pushing and popping threadsafe
	- OTHER FUNCTIONS ARE **NOT** THREAD SAFE
		- shouldn't need other functions however, since popping will 
		  block if nothing there
	- WILL BLOCK IF TRYING TO POP WHEN EMPTY
	- WILL BLOCK IF TRYING TO PUSH WHEN FULL
	- MUST USE HEAP WHEN USING MULTIPLE THREADS
	- essentially, right now will only work properly with multiple threads,
	  but don't know when one would be using FIFOs in single threaded applications
**********************************/

/*********************************
IMPLEMENTATION DETAILS
 	- pushing and popping when full and empty respectively
 	 will not affect the underlying buffer. 
	- Pushing when full will return -1. 
	- Popping when full return an old pointer. 
		- This decision was made as garbage data 
		  is better than segfaults.
**********************************/


#include "fifo.h"
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include <sys/mman.h>
//#include "../structs.h"

#include <stdio.h>

#define PSHARED 1 // 1 for using between processes, 0 for between threads

struct Fifo_t {
	int depth;
	int in;
	int out;
	int numItems;
	sem_t m,f,e; // semaphores for mutual exclusion, full, empty respectively
	struct transfer buffer[];
};


Fifo_T Fifo_new(int depth) {

	/* allocate memory */
	printf("FifoSize: %lu\n",sizeof(struct Fifo_t)+depth*sizeof(struct transfer));

	Fifo_T fifo = mmap(NULL, sizeof(struct Fifo_t)+depth*sizeof(struct transfer), PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	assert (fifo != MAP_FAILED);

	/* store_depth */
	fifo->depth=depth;
	fifo->in=0;
	fifo->out=0;
	fifo->numItems=0;
	/* instantiate semaphores */
	sem_init(&(fifo->m),PSHARED,1); //mutual exclusion, must be 1 so that producer can acquire buffer first
	sem_init(&(fifo->f),PSHARED,depth); // full tokens 
	sem_init(&(fifo->e),PSHARED,0); //empty tokens 
	return fifo;
}
void Fifo_del(Fifo_T fifo){
	assert(fifo);
	/* free semaphores */
	sem_destroy(&(fifo->m)); 
	sem_destroy(&(fifo->f));
	sem_destroy(&(fifo->e));
	/* free fifo */
	if (munmap(fifo, sizeof(struct Fifo_t)+fifo->depth*sizeof(struct transfer)) == -1)
		perror("munmap");
}
int Fifo_push(Fifo_T fifo, struct transfer item){
	assert(fifo); 

	// down f // order matters, if down m first, deadlock
	sem_wait(&(fifo->f));
	// down m
	sem_wait(&(fifo->m));
	
	// check if FULL
	// if(fifo->numItems==fifo->depth)
	// 	return -1;

	fifo->buffer[fifo->in] = item;
	fifo->in = (fifo->in + 1) % (fifo->depth);
	fifo->numItems++;

	// up(e) // order doesn't matter, ups don't block	 	
	sem_post(&(fifo->e));
	// up(m) 
	sem_post(&(fifo->m));

	return 1;
}
struct transfer Fifo_pop(Fifo_T fifo){
	assert(fifo);

	// down(e)
	sem_wait(&(fifo->e));
	// down(m)
	sem_wait(&(fifo->m));

	/* check if empty */
	// if (fifo->numItems==0)
	// 	return fifo->buffer[0]; // garbage pointer

	struct transfer temp = fifo->buffer[fifo->out];
	fifo->out = (fifo->out + 1)% (fifo->depth);
	fifo->numItems--;

	// up(f)
	sem_post(&(fifo->f));
	// up (m)
	sem_post(&(fifo->m));
	return temp;
}
int Fifo_depth(Fifo_T fifo){
	assert(fifo);

	return fifo->depth;
}

int Fifo_isEmpty(Fifo_T fifo){
	return (fifo->numItems==0);
}

int Fifo_notEmpty(Fifo_T fifo){
	return !(fifo->numItems==0);
}

int Fifo_isFull(Fifo_T fifo){
	return (fifo->numItems==fifo->depth);
}

int Fifo_numItems(Fifo_T fifo){
	assert(fifo);

	return fifo->numItems;
}