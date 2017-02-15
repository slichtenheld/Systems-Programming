#ifndef STRUCTS_H
#define STRUCTS_H


#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)

struct newAcct {
	int acctNo;
	int initBalance;
};

void printNewAcct(void *temp){
	struct newAcct* acct = (struct newAcct*) temp;
	printf("%d %d\n",acct->acctNo,acct->initBalance );
}

struct transfer {
	int acctNoFrom;
	int acctNoTo;
	int amount;
	int poison;
};
void printTransfer(struct transfer* trans){
	printf("%ld: acctNoFrom: %d, acctNoTo: %d, amount: %d\n",gettid(),trans->acctNoFrom,trans->acctNoTo,trans->amount );
}

#endif