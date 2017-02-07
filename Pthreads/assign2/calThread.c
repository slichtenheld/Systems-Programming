#include "calThread.h"
#include "calendarADT.h"
#include "macros.h"
#include "mtxGlbl.h"
#include "circBuf.h"

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>

void * consumer(void * arg) {
	struct circularBuffer *circbuf = (struct circularBuffer*) arg;

	Calendar_T cal = Calendar_new();
	char type;
		

	/* loop until poison pill received */
	while(1) { 
		DEBUG_PRINT( ("THREAD C: %ld\n", gettid() ) );

		struct CalendarItem_t *item = malloc(sizeof *item);

		/* CRITICAL SECTION */
		pthread_mutex_lock(&mtx); // acquire lock
		{
			/* give up lock if buffer is full and wait to be woken up */
			while ( circbuf->numItems == 0 ) // while loop because thread could be woken up due to other conditions
				pthread_cond_wait(&consCond,&mtx);

			/* copy buffer item to local item */
			memcpy( item, &(circbuf->buffer[circbuf->out].calItem), sizeof(struct CalendarItem_t) ); // copy struct to buffer
			type = circbuf->buffer[circbuf->out].type;
			circbuf->out = (circbuf->out + 1) % circbuf->size;
			circbuf->numItems --;

			/*** OPTIMIZATION: PRODUCER WILL ONlY BE SLEEPING IF BUFFER WAS FULL, AND NUMITEMS JUST GOT DECREMENTED BY 1 ***/ 
			if (circbuf->numItems == circbuf->size - 1) 
				pthread_cond_signal(&prodCond);
		}
		pthread_mutex_unlock(&mtx);
		/* END CRITICAL SECTION */
		
		switch (type){
			case 'E': pthread_exit(NULL); break;
			case 'C': Calendar_add(cal, item);break;
			case 'D': Calendar_del(cal, item);break;
			case 'X': Calendar_mod(cal, item);break;
			default : printf("incompatible calendarEvent type\n");
		}
	}
}