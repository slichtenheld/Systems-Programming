#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "circBuf/fifo.h"
#include "structs.h"
#include "linkedlist/list.h"

#define FIFOSIZE 5


void * workerThread(void* arg){
	
	Fifo_T fifo = (Fifo_T) arg;
	// check queue
	while (1){
		struct transfer *temp = Fifo_pop(fifo);
		if (temp->poison){
			free(temp);
			break;
		}
		printTransfer(temp);
		free(temp);
	}
	pthread_exit(NULL);
}


int main(int argc, char * argv[]){

	/*** MAKE SURE PROPER INPUTS ***/
	if (argc != 3) {
		perror("ERROR: ./program <inputfile> <numWorkerThreads>");
		exit(EXIT_FAILURE);
	}

	/*** DECLARE WORKER THREADS ***/
	char *endptr;
	int numWorkers = strtol(argv[2],&endptr,10); // convert string from input to int
	pthread_t worker_pt[numWorkers];
	
	/*** DECLARE EACH WORKER THREAD QUEUE ***/
	Fifo_T fifo[numWorkers];
	
	/*** INSTANTIATE QUEUES AND WORKER THREADS ***/
	for (int i = 0; i < numWorkers; i++){
		fifo[i] = Fifo_new(FIFOSIZE);
		if ( pthread_create( &worker_pt[i],NULL,workerThread,(void*)fifo[i] ) ) { // TODO: pass appropriate queue to each workerThread
			perror("error creating consumer thread");
			exit(EXIT_FAILURE);
		}
	}
	
	/*** INSTANTIATE ACCOUNT ADT ***/
	List_T list = List_new(sizeof(struct newAcct));

	
	/*** FILE ARGUMENTS***/
	char* filename = argv[1];
	FILE *stream;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	
	/*** OPEN FILE ***/
	stream = fopen(filename,"r"); // file only for reading
	if (stream==NULL){ // exit if wasn't able to read file
		perror("error opening file");
		exit(EXIT_FAILURE);
	}
	
	
	/*** READ FILE ***/
	int counter = 0; // used to keep track for round robin
	while ((read = getline(&line, &len, stream)) != -1) { // until EOF
		/* PARSE FILE*/
		char *pch = strtok (line," ");
		int check = strcmp(pch,"Transfer");
		if (check!=0) { // PARSE ACCT INITIALIZATION
			/* INITIALIZE ACCT */
			struct newAcct* newAcct_p = malloc(sizeof(struct newAcct));
			char * endptr;
			newAcct_p->acctNo = strtol(pch,&endptr,10);
			pch = strtok (NULL," ");
			newAcct_p->initBalance = strtol(pch,&endptr,10);
			/* ADD ACCT TO LIST OF ACCTS */
			List_append(list, newAcct_p);
		}
		else{
			/* ASSIGN TRANSFERS TO WORKERS */
			struct transfer* trans_p = malloc(sizeof(struct transfer));
			char * endptr;
			pch = strtok (NULL," ");
			trans_p->acctNoFrom = strtol(pch,&endptr,10);
			pch = strtok (NULL," ");
			trans_p->acctNoTo = strtol(pch,&endptr,10);
			pch = strtok (NULL," ");
			trans_p->amount = strtol(pch,&endptr,10);
			trans_p->poison = 0;
			/* SEND TRANSFER TO WORKERTHREAD (ROUND ROBIN) BY ADDING TO FIFO */
			Fifo_push(fifo[counter++%numWorkers],trans_p);
		}
		
	}

	free(line);
	fclose(stream);

	/*** POISON ALL THREADS ONCE DONE WITH FILE***/
	for (int i = 0; i < numWorkers; i++){
		struct transfer* trans_p = malloc(sizeof(struct transfer));
		trans_p->poison = 1;
		Fifo_push(fifo[i],trans_p);
	}

	/*** WAIT FOR ALL THREADS TO TERMINATE ***/
	for (int i = 0; i < numWorkers; i++)
		pthread_join(worker_pt[i],NULL);

	/*** PRINT FINAL BALANCES ***/
	List_printAll(list,printNewAcct);

	/*** CLEANUP ***/
	List_free(list);
	exit(EXIT_SUCCESS);
}