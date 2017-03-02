#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include <time.h>

#include "macros.h"
#include "structs.h"



int main (int argc, char * argv[]) {

	// int pageSize = getpagesize();
	
	// printf("Page size on your system = %d bytes\n", pageSize);
	
	// printf("size of acct: %lu\n", sizeof(struct acct));
	// printf("size of int: %lu\n", sizeof(int));
	// printf("size of sem_t: %lu\n", sizeof(sem_t));
	// printf("size of mtx: %lu\n", sizeof(pthread_mutex_t));
	// return 0;
	
	/*** MAKE SURE PROPER INPUTS ***/
	if (argc != 3) {
		perror("ERROR: ./program <inputfile> <numWorkerThreads>");
		exit(EXIT_FAILURE);
	}

	// take in command line inputs
	int numWorkers = strtol(argv[2],NULL,10); // convert string from input to int
	char* filename = argv[1];

	pid_t worker[numWorkers];

	// start timer
	clock_t begin = clock();

	int *test = mmap(NULL, 100*sizeof(struct acct), PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (test == MAP_FAILED)
		exit(EXIT_FAILURE);

	for(int i = 0; ;i++){
		printf("%d: %d\n",i,*(test+i));
	}

	*test = 5;
	for (int i = 0; i < numWorkers; i++){
		int pid = fork();
		if (pid == 0){ // child
			printf("%d: test:  %d\n",getpid(), *test);
			(*test)++;
			return 0;
		}
	}

	//int status;
	for (int i = 0; i < numWorkers; i++)
    	wait(NULL);

    printf("%d: test:  %d\n",getpid(), *test);
    // stop timer
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    DEBUG_PRINT( ("Execution time: %f\n", time_spent ) );
}