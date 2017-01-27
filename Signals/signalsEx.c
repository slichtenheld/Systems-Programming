#include <signal.h>
//#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


// This stuff is all defined in signal.h

//sigsahandler_t signal(int, sigsahandler_t);

/* 
struct sigaction {
	void     (*sa_handler)(int);
	void     (*sa_sigaction)(int, siginfo_t *, void *);
	sigset_t   sa_mask;
	int        sa_flags;
	void     (*sa_restorer)(void);
};
*/


/*
sigaction(int,struct sigaction *new, struct sigaction *old);
*/

void childExit(int s) {
	printf("ChildExit\n");
	int status;
	waitpid(-1,&status,WNOHANG);
}

int main(int argc, char *argv[]){
	struct sigaction CH;
	CH.sa_handler = childExit;
	sigaction(SIGCHLD,&CH,NULL);
	int r = fork();
	if (r>0) { /* parent */
		//for (;!childExit;) <== definitely not blocking indefinitely???
			pause();
	}
	else {
		printf("I am the child\n");
		printf("asasdfasfdsf\n");
		exit(EXIT_SUCCESS);
	}



}