#include "structs.h"



void initAcct (struct acct* a, int acctNo, int balance){
	a->acctNo = acctNo;
	a->b.currentValue = balance;
	sem_init(&(a->b.m),0,1); // initialized to 1 for mutual exclusion
}

void printAcct(void *temp){
	struct acct* acct = (struct acct*) temp;
	printf("%d %d\n",acct->acctNo,acct->b.currentValue );
}
void printTransfer(struct transfer* trans){
	printf("%lu: acctNoFrom: %d, acctNoTo: %d, amount: %d\n",getpid(),trans->acctNoFrom,trans->acctNoTo,trans->amount );
}