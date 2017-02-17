#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int randinrange(int, int);

int main (int argc, char * argv[]){


	if (argc != 4) {
		perror("ERROR: ./program < # accounts >< # transactions> < seed #>");
		exit(EXIT_FAILURE);
	}

	char *endptr;
	int numAccts = strtol(argv[1],&endptr,10);
	int numTrans = strtol(argv[2],&endptr,10);
	int seed = strtol(argv[3],&endptr,10);
	srand(seed);

	char* fileName;
	asprintf(&fileName,"input_%d_%d_%d.txt",numAccts,numTrans,seed);

	FILE *fp = fopen(fileName,"w");

	for (int i = 0; i < numAccts; i++){
		char* accountTxt;
		asprintf(&accountTxt,"%d %d\n",i,randinrange(0,1000));
		printf("%s\n",accountTxt );
		//fputs(accountTxt,fp);
	}

}

int randinrange(int HI, int LO) {
        return LO + ( (rand() ) / (RAND_MAX/(HI-LO) ) );
}

