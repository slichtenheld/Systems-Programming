#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"
#include <pthread.h>

Fifo_T fifo;
struct testStruct {
	int num;
	char letter;
};


void * producer(void * arg) {

	//while(1){
		struct testStruct* tS1_p = malloc(sizeof(struct testStruct));
		tS1_p->num = 1;
		tS1_p->letter ='A';
		Fifo_push(fifo,tS1_p);

		tS1_p = malloc(sizeof(struct testStruct));
		tS1_p->num = 2;
		tS1_p->letter ='B';
		Fifo_push(fifo,tS1_p);

		tS1_p = malloc(sizeof(struct testStruct));
		tS1_p->num = 3;
		tS1_p->letter ='C';
		Fifo_push(fifo,tS1_p);

		tS1_p = malloc(sizeof(struct testStruct));
		tS1_p->num = 4;
		tS1_p->letter ='D';
		Fifo_push(fifo,tS1_p);

		tS1_p = malloc(sizeof(struct testStruct));
		tS1_p->num = 5;
		tS1_p->letter ='D';
		Fifo_push(fifo,tS1_p);
	
	tS1_p = malloc(sizeof(struct testStruct));
	tS1_p->num = 6;
	tS1_p->letter ='E';
	Fifo_push(fifo,tS1_p);
	//}
}

int main(int argc, char * argv[]){
	fifo = Fifo_new(5);
	pthread_t prodThread;

	pthread_create( &prodThread,NULL,producer,NULL);
	//pthread_join(prodThread,NULL);
	while (1){
		struct testStruct *temp = Fifo_pop(fifo);
		printf("%d%c\n",temp->num,temp->letter);
		free(temp);
	}
	
}