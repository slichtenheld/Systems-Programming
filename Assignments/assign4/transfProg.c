#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include <time.h>
#include <string.h>


#include "macros.h"
#include "structs.h"
#include "circBuf/fifo.h"


void workProcess(Fifo_T fifo){
	printf("WORKER:  %d\n",getpid());
		while(1){
			struct transfer transTemp = Fifo_pop(fifo);
			printTransfer(&transTemp);
			if (transTemp.poison){
				printf("%lu poisoned\n", getpid());
				Fifo_del(fifo);
				exit(EXIT_SUCCESS);
			}
		}
}

int main (int argc, char * argv[]) {

	/*** MAKE SURE PROPER INPUTS ***/
	if (argc != 3) {
		perror("ERROR: ./program <inputfile> <numWorkerThreads>");
		exit(EXIT_FAILURE);
	}

	// take in command line inputs
	int numWorkers = strtol(argv[2],NULL,10); // convert string from input to int
	
	// declare and instantiate each worker queue
	Fifo_T fifo[numWorkers];
	for (int i = 0; i < numWorkers; i++){
		fifo[i] = Fifo_new(100);
	}

	
	// start timer
	clock_t begin = clock();


	for (int i = 0; i < numWorkers; i++){
		int pid = fork();
		if (pid == 0){ // WORKER PROCESS
			printf("FORKED: %lu\n", fifo[i]);
			workProcess(fifo[i]);
			perror("THIS SHOULD NEVER BE REACHED");
		}
		else{
			
		}
	}

	// MAIN PROCESS
	// file arguments
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
		//printf("%s", line);
		//printf("TEST\n");
		/* PARSE FILE*/
		char *pch = strtok (line," ");
		int check = strcmp(pch,"Transfer");
		if (check!=0) { // PARSE ACCT INITIALIZATION
		// 	// /* INITIALIZE ACCT */
		// 	// struct acct* acct_p = malloc(sizeof(struct acct));
		// 	// char * endptr;
		// 	// int acctNo = strtol(pch,&endptr,10);
		// 	// pch = strtok (NULL," ");
		// 	// int initBalance = strtol(pch,&endptr,10);
		// 	// initAcct(acct_p,acctNo,initBalance);
		// 	// /* ADD ACCT TO LIST OF ACCTS */
		// 	// List_append(list, acct_p);
		}
		else{
			//printf("%s", line);
			/* ASSIGN TRANSFERS TO WORKERS */
			struct transfer trans_p;
			char * endptr;
			pch = strtok (NULL," ");
			trans_p.acctNoFrom = strtol(pch,&endptr,10);
			pch = strtok (NULL," ");
			trans_p.acctNoTo = strtol(pch,&endptr,10);
			pch = strtok (NULL," ");
			trans_p.amount = strtol(pch,&endptr,10);
			trans_p.poison = 0;
			/* SEND TRANSFER TO WORKERTHREAD (ROUND ROBIN) BY ADDING TO FIFO */
			//printf("sending transfer to %d\n", counter%numWorkers);
			//printTransfer(&trans_p);
			Fifo_push(fifo[counter%numWorkers],trans_p);
			counter++;
		}

	}

	fclose(stream);

	/*** POISON ALL THREADS ONCE DONE WITH FILE***/
	for (int i = 0; i < numWorkers; i++){
		struct transfer trans_p;
		trans_p.poison = 1;
		Fifo_push(fifo[i],trans_p);
	}

	// //int status;
	for (int i = 0; i < numWorkers; i++)
    	wait(NULL);

 //    printf("%d: test:  %d\n",getpid(), *test);
    // stop timer
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    DEBUG_PRINT( ("Execution time: %f\n", time_spent ) );
}