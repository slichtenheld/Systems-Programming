#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "circBuf/fifo.h"
#include "structs.h"

#define FIFOSIZE 5


void * workerThread(void* arg){
	
	Fifo_T fifo = (Fifo_T) arg;
	// check queue
	while (1){
		struct transfer *temp = Fifo_pop(fifo);
		printTransfer(temp);
		free(temp);
	}
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
	int counter = 0;
	/*** READ FILE ***/
	while ((read = getline(&line, &len, stream)) != -1) { // until EOF
		//printf("%s", line);

		/* PARSE FILE*/
		char *pch = strtok (line," ");
		int check = strcmp(pch,"Transfer");
		if (check!=0) {
			/* PARSE ACCT INITIALIZATION */
			//printf("ACCT_INIT\n");
			/* INITIALIZE ACCT */
			struct newAcct* newAcct_p = malloc(sizeof(struct newAcct));
			char * endptr;
			newAcct_p->acctNo = strtol(pch,&endptr,10);
			pch = strtok (NULL," ");
			newAcct_p->initBalance = strtol(pch,&endptr,10);
			//printNewAcct(newAcct_p);
			/* ADD ACCT TO LIST OF ACCTS */
		}
		else{
			/* PARSE TRANSFER */
			//printf("TRANSFER\n");
			struct transfer* trans_p = malloc(sizeof(struct transfer));
			char * endptr;
			pch = strtok (NULL," ");
			trans_p->acctNoFrom = strtol(pch,&endptr,10);
			pch = strtok (NULL," ");
			trans_p->acctNoTo = strtol(pch,&endptr,10);
			pch = strtok (NULL," ");
			trans_p->amount = strtol(pch,&endptr,10);
			//printTransfer(trans_p);
			/* SEND TRANSFER TO WORKERTHREAD (ROUND ROBIN) BY ADDING TO FIFO*/
			Fifo_push(fifo[counter++%numWorkers],trans_p);
		}
		// INITIALIZE ACCT OR ASSIGN TRANSFERS TO WORKERS 
	}



	free(line);
	fclose(stream);

	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
}