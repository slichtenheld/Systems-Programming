
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)


/* global variables, is this the best way? */
pthread_mutex_t mtx; // declare global mutex
pthread_cond_t prodCond,consCond ; //declare global conditions
pthread_cond_t termCond; // termination condition

int counter = 0;
#define COUNTMAX 20
#define PRODTHREADS 1
#define CONSTHREADS 1

struct calendarEvent_t {
	char type;
	char title[11]; // extra char for /0 characeter
	char date[11];
	char time[6];
	char location[11];
};

void printCalendarEvent(struct calendarEvent_t c){
	printf("%c,%s,%s,%s,%s\n",c.type, c.title, c.date, c.time, c.location);
}

void printCalEvDebug(struct calendarEvent_t c){
	printf("Type: %c, Title: %s, Date: %s, Time: %s, location: %s\n\n",c.type, c.title, c.date, c.time, c.location);
}


struct circularBuffer{
	int size;
	int in;
	int out;
	int numItems;
	struct calendarEvent_t buffer[];
}; 

int parseEmail(char * buffer, struct calendarEvent_t *c);

void * producer(void * arg) {

	struct circularBuffer *circbuf = (struct circularBuffer*) arg;

	/* initialize buffer for getline */
	char *buffer = NULL; // getline will allocate space for buffer
	int charsRead = 0;
	long unsigned int len = 0; // length of buffer

	/* initialize variables for parsing */
	struct calendarEvent_t	c; // reusable struct for calendarevents

	while(1) {
		
		/* take in input from stdin */
		char charsRead = getline(&buffer,&len,stdin);
		int parsed = 0;

		/* if !EOF, parse input in buffer and put in struct => else set poison pill */
		if ( charsRead != -1 )
			parsed = parseEmail(buffer, &c); 
		else 
			c.type = 'E'; // poison pill still needs to be sent before can exit thread

		printCalEvDebug(c);

		/* Buffer written to here */
		if (charsRead == -1 || parsed != -1) {	
			/* LOCK MUTEX */
			pthread_mutex_lock(&mtx); // acquire lock
			{

				/* give up lock if buffer is full and wait to be woken up */
				while ( circbuf->numItems == circbuf->size ) // while loop because thread could be woken up due to other conditions
					pthread_cond_wait(&prodCond,&mtx);


				memcpy( &(circbuf->buffer[circbuf->in]), &c, sizeof(struct calendarEvent_t) ); // copy struct to buffer
				circbuf->in  = (circbuf->in + 1) % circbuf->size;
				circbuf->numItems++;

				/*** OPTIMIZATION: CONSUMER WILL ONLY BE SLEEPING IF BUFFER WAS EMPTY, AND BUFFER JUST GOT INCREMENTED BY 1 ***/
			 	if (circbuf->numItems == 1) 
					pthread_cond_signal(&consCond);
			
			}
			/* UNLOCK MUTEX */
			pthread_mutex_unlock(&mtx);
		}
		if (charsRead == -1)
			break;
	}
	/* Cleanup */
	free(buffer);
	pthread_exit(NULL);
}

void * consumer(void * arg) {
	struct circularBuffer *circbuf = (struct circularBuffer*) arg;

	/* loop until poison pill received */
	while(1) { 

		pthread_mutex_lock(&mtx); // acquire lock

		/* give up lock if buffer is full and wait to be woken up */
		while ( circbuf->numItems == 0 ) // while loop because thread could be woken up due to other conditions
			pthread_cond_wait(&consCond,&mtx);

		printf("THREAD %ld: Removing from buffer\n", gettid() );
		circbuf->out = (circbuf->out + 1) % circbuf->size;
		circbuf->numItems --;

		/*** OPTIMIZATION: PRODUCER WILL ONlY BE SLEEPING IF BUFFER WAS FULL, AND NUMITEMS JUST GOT DECREMENTED BY 1 ***/ 
		if (circbuf->numItems == circbuf->size - 1) 
			pthread_cond_signal(&prodCond);

		pthread_mutex_unlock(&mtx);
	}
}


/*****************************************************************/

int main (int argc, char* argv[]) {

	
	if (argc != 2) {
		perror("ERROR: ./program <size of buffer>");
		exit(-1);
	}

	char *endptr;
	int bufferlength = strtol(argv[1],&endptr,10); // convert string from input to int

	struct circularBuffer *cBuf = malloc(sizeof(struct circularBuffer) + bufferlength * sizeof(struct calendarEvent_t));

	cBuf->size = bufferlength; //store size of buffer in struct so can be passed to threads 

	printf("Buffer is of size %d\n", bufferlength );
	


	/*make sure to initialize buffer*/
	
	/* declare threads, init mutex, and init condition */
	pthread_t prodThread[PRODTHREADS],consThread[CONSTHREADS]; 
	pthread_mutex_init(&mtx, NULL); 
	pthread_cond_init(&prodCond, NULL);
	pthread_cond_init(&consCond, NULL);

	for (int i = 0; i < PRODTHREADS; i++){
		printf("Creating producer[%d] thread\n",i);
		if ( pthread_create( &prodThread[i],NULL,producer,(void*)cBuf ) ) { 
			perror("error creating producer thread");
			exit(-1);
		}
	}
	// for (int i = 0; i < CONSTHREADS; i++){
	// 	printf("Creating consumer[%d] thread\n",i);
	// 	if ( pthread_create( &consThread[i],NULL,consumer,(void*)cBuf ) ) { 
	// 		perror("error creating consumer thread");
	// 		exit(-1);
	// 	}
	// }

	//pthread_cond_wait(&consCond,&mtx);

	/* wait for all threads to terminate, destroy mutex and condition */
	pthread_exit(NULL); 
	free(cBuf);
	pthread_mutex_destroy(&mtx);
	pthread_cond_destroy(&prodCond);
	pthread_cond_destroy(&consCond);
}

int parseEmail(char * buffer, struct calendarEvent_t *c){ // returns -1 if doesn't have everything
	char *pch = strtok (buffer," ");
	unsigned int counter = 0;
	while (pch != NULL){
		
	    switch(counter++) {
	    	case 0: break; //do nothing, ignore "Subject:"
	    	case 1: c->type = pch[0]; break;
	    	case 2: memcpy(c->title,pch, strlen(pch)+1); break;
	    	case 3: memcpy(c->date,pch, strlen(pch)+1); break;
	    	case 4: memcpy(c->time,pch, strlen(pch)+1); break;
	    	case 5: 
	    		memcpy(c->location,pch, strlen(pch)-1); /* stupid fix for newline character */
	    		break;
	    }
	    pch = strtok (NULL, ",");
	    
	}

	if (counter != 6) return -1;
	return 0;
}