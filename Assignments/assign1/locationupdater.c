#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

int main (int argc, char * argv[]) {

  int fd[2];
	pid_t cpid1, cpid2;
	char buf[5];

  	/* Create Pipe to be used by children */
  	if (pipe(fd) == -1) { 
    	perror("pipe error");
    	exit(EXIT_FAILURE);
  	}

  
	cpid1 = fork(); /* creates first child */
	if ( cpid1 > 0 ) { /* parent */

  		cpid2 = fork(); /* creates second child */

		if ( cpid2 > 0 ) { /* parent */
  			/* All of parents code should be here so that isn't run by children */
	  		
  			close(fd[0]);  /* Parent does not need access to pipe */
  			close(fd[1]);
  			waitpid(cpid1,0,0);
  			waitpid(cpid2,0,0);
        DEBUG_PRINT(("Parent Exiting\n"));
    		exit(EXIT_SUCCESS);
	  	}
	  	else if ( cpid2 == 0 ) { /* child 2 */
	  		close(fd[1]);		/* Close unused write end, child 2 only reading from pipe */
    		if ( dup2(fd[0], 0) == -1 ) {		/* redirect pipe to stdin */
    			perror("dup2 error child 2");
				exit(EXIT_FAILURE);
	  		}
	  		close(fd[0]);

	  		DEBUG_PRINT(("I am Child 2\n"));
	  		if ( execl("calendarfilter", "calendarfilter", (char *) NULL) == -1 ) {
	  			perror("exec error");
    			_exit(EXIT_FAILURE);
	  		}
	  	}
	  	else if ( cpid2 == -1 ) { /* error */
	  		perror("fork error\n");
    		exit(EXIT_FAILURE);
	  	}

  	}
  	else if ( cpid1 == 0 ) { /* child 1 */
  		DEBUG_PRINT(("I am Child 1\n"));
  		close(fd[0]);     	/* Close unused write end, child 1 only writing */
  		if ( dup2(fd[1],1) == -1 ) { /* redirect stdout to pipe */
  			perror("dup2 error child 1");
			_exit(EXIT_FAILURE);
  		}		
  		close(fd[1]);

  		if ( execl("emailfilter", "emailfilter", (char *) NULL) == -1 ) {
			perror("exec error child 1");
			exit(EXIT_FAILURE);
		}

  	}
  	else if ( cpid1 == -1 ) { /* error */
  		perror("fork error\n");
    	exit(EXIT_FAILURE);
  	}

}