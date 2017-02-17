#include "workerThread.h"

#include "structs.h"
#include "circBuf/fifo.h"
#include <stdlib.h>
#include <pthread.h>
//#include <stdio.h> // TODO: REMOVE


extern void * workerThread(void* arg){
	
	Fifo_T fifo = (Fifo_T) arg;
	// check queue
	while (1){
		struct transfer *transTemp = Fifo_pop(fifo);
		if (transTemp->poison){
			free(transTemp);
			break;
		}
		// acquire handle to each from and to
		Iter_T iter = List_makeIter(list); // list is global var
		struct acct* acctFrom = NULL;
		struct acct* acctTo = NULL;
		struct acct* acctTemp;

		for(Iter_rst(iter); Iter_isValid(iter); Iter_next(iter)){
			acctTemp = Iter_currentItem(iter);
			if (acctTemp->acctNo == transTemp->acctNoFrom )
				acctFrom = acctTemp;
			else if (acctTemp->acctNo == transTemp->acctNoTo)
				acctTo = acctTemp;

			if (acctFrom != NULL && acctTo != NULL)
				break;
		}
		
		//printf("AcctTo: %d, AcctFrom: %d\n", acctTo->acctNo, acctFrom->acctNo );

		/*** MAKE SURE ALWAYS ACQUIRE LOCKS IN SAME ORDER, THUS LESSER VALUE FIRST ***/
		if (transTemp->acctNoTo < transTemp->acctNoFrom) {
			sem_wait(&(acctTo->b.m));
			sem_wait(&(acctFrom->b.m));
		} 
		else {
			sem_wait(&(acctFrom->b.m));
			sem_wait(&(acctTo->b.m));
		}
		
		/*** CRITICAL SECTION ***/
		// do transfer
		acctFrom->b.currentValue -= transTemp->amount;
		acctTo->b.currentValue += transTemp->amount;

		// release locks
		sem_post(&(acctFrom->b.m));
		sem_post(&(acctTo->b.m));


		// free transaction temp (was allocated by main, before pushed onto fifo)
		free(transTemp);


		
	}
	pthread_exit(NULL);
}