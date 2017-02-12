#include <stdio.h>
#include "fifo.h"

struct testStruct {
	int num;
	char letter;
};

int main(int argc, char * argv[]){
	Fifo_T fifo = Fifo_new(5);
	struct testStruct tS1 = {1,'A'};
	Fifo_push(fifo,&tS1);

	struct testStruct tS2 = {2,'B'};
	Fifo_push(fifo,&tS2);

	struct testStruct tS3 = {3,'C'};
	Fifo_push(fifo,&tS3);

	struct testStruct tS4 = {4,'D'};
	Fifo_push(fifo,&tS4);

	struct testStruct tS5 = {5,'E'};
	Fifo_push(fifo,&tS5);

	struct testStruct tS6 = {6,'F'};
	Fifo_push(fifo,&tS6);

	for(int i = 0; i < Fifo_size(fifo); i++){
		struct testStruct *temp = Fifo_pop(fifo);
		printf("%d%c\n",temp->num,temp->letter);
	}
}