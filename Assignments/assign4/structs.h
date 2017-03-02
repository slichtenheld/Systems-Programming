#ifndef STRUCTS_H
#define STRUCTS_H

#include <semaphore.h>
//#include "linkedlist/list.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)

//extern List_T list;

struct balance {
	int currentValue;
	sem_t m;
};

struct acct {
	int acctNo;
	struct balance b;
};


extern void initAcct (struct acct* a, int acctNo, int balance){
	a->acctNo = acctNo;
	a->b.currentValue = balance;
	sem_init(&(a->b.m),0,1); // initialized to 1 for mutual exclusion
}

extern void printAcct(void *temp){
	struct acct* acct = (struct acct*) temp;
	printf("%d %d\n",acct->acctNo,acct->b.currentValue );
}

struct transfer {
	int acctNoFrom;
	int acctNoTo;
	int amount;
	int poison;
};

extern void printTransfer(struct transfer* trans){
	printf("%ld: acctNoFrom: %d, acctNoTo: %d, amount: %d\n",gettid(),trans->acctNoFrom,trans->acctNoTo,trans->amount );
}

#endif