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

	for (int i = 1; i <= numAccts; i++){
		char* accountTxt;
		asprintf(&accountTxt,"%d %d\n",i,randinrange(0,1000));
		//printf("%s",accountTxt );
		fputs(accountTxt,fp);
	}
	for (int i = 0; i< numTrans; i++){
		char* transferTxt;
		int acctTo =  randinrange(0,numAccts); //will never be zero idk why
		int acctFrom = randinrange(0,numAccts);
		while(acctTo==acctFrom)
			acctFrom = randinrange(0,numAccts);
		asprintf(&transferTxt,"Transfer %d %d %d\n", acctTo, acctFrom, randinrange(0,10000));
		//printf("%s",transferTxt );
		fputs(transferTxt,fp);
	}

}

int randinrange(int HI, int LO) {
        return LO + ( (rand() ) / (RAND_MAX/(HI-LO) ) );
}

