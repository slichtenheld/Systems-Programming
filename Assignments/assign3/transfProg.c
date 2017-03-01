#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "circBuf/fifo.h"
#include "structs.h"
#include "linkedlist/list.h"
#include "workerThread.h"

#define FIFOSIZE 5

List_T list;


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
		if ( pthread_create( &worker_pt[i],NULL,workerThread,(void*)fifo[i] ) ) { // pass appropriate queue to each workerThread
			perror("error creating consumer thread");
			exit(EXIT_FAILURE);
		}
	}
	
	/*** INSTANTIATE ACCOUNT ADT ***/
	list = List_new(sizeof(struct acct));

	
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
	
	clock_t begin = clock();
	
	/*** READ FILE ***/
	int counter = 0; // used to keep track for round robin
	while ((read = getline(&line, &len, stream)) != -1) { // until EOF
		/* PARSE FILE*/
		char *temp = malloc(read);
		memcpy(temp,line,read);
		char *pch = strtok (line," "); 
		int check = strcmp(pch,"Transfer");
		if (check!=0) { // PARSE ACCT INITIALIZATION
			/* INITIALIZE ACCT */
			struct acct* acct_p = malloc(sizeof(struct acct));
			char * endptr;
			int acctNo = strtol(pch,&endptr,10);
			pch = strtok (NULL," ");
			int initBalance = strtol(pch,&endptr,10);
			initAcct(acct_p,acctNo,initBalance);
			/* ADD ACCT TO LIST OF ACCTS */
			List_append(list, acct_p);
			free(temp);
		}
		else{
			Fifo_push(fifo[counter++%numWorkers],temp);
		}
		
	}

	free(line);
	fclose(stream);

	/*** POISON ALL THREADS ONCE DONE WITH FILE***/
	int s = sizeof("POISON ");
	for (int i = 0; i < numWorkers; i++){
		char *temp2 = malloc(s);
		char temp3[] = "POISON ";
		memcpy(temp2, temp3, s);
		//printf("%s\n", pch2);
		//struct transfer* trans_p = malloc(sizeof(struct transfer));
		//trans_p->poison = 1;
		Fifo_push(fifo[i],temp2);
	}

	/*** WAIT FOR ALL THREADS TO TERMINATE ***/
	for (int i = 0; i < numWorkers; i++)
		pthread_join(worker_pt[i],NULL);

	clock_t end = clock();

	/*** PRINT FINAL BALANCES ***/
	List_printAll(list,printAcct);

	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("TIME SPENT: %f\n",time_spent);
	/*** CLEANUP ***/
	List_free(list);
	exit(EXIT_SUCCESS);
}