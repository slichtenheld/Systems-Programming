#ifndef STRUCTS_H
#define STRUCTS_H

#include <semaphore.h>
//#include "linkedlist/list.h"

#include <stdio.h>
#include <unistd.h>
//#include <sys/syscall.h>
//#define gettid() syscall(SYS_getpid)
#include <sys/types.h>
//#include <unistd.h>

//extern List_T list;

struct balance {
	int currentValue;
	sem_t m;
};

struct acct {
	int acctNo;
	struct balance b;
};


extern void initAcct (struct acct* a, int acctNo, int balance);


extern void printAcct(void *temp);

struct transfer {
	int acctNoFrom;
	int acctNoTo;
	int amount;
	int poison;
};

extern void printTransfer(struct transfer* trans);

#endif