/******************************************************************************
* FILE: hello.c
* DESCRIPTION:
*   A "hello world" Pthreads program.  Demonstrates thread creation and
*   termination.
* AUTHOR: Blaise Barney
* LAST REVISED: 08/09/11
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	5

void *PrintHello(void *threadid) /* must take in null pointer and return null pointer */
{
   long tid;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);
   /* pthread_exit really only needed to pass termination status back */
   pthread_exit(NULL); // can pass a termination status parameter if wanted
}

int main(int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   int rc;
   long t; 
   for(t=0;t<NUM_THREADS;t++){
     printf("In main: creating thread %ld\n", t);
     // t must be passed by value, pass by reference will cause data races
     rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t); //must be passed by value, pass by reference will cause 
     if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
       }
     }

   /* Last thing that main() should do */
   /* causes main thread to block and be kept alive to support the threads it created unitl they all are done */
   pthread_exit(NULL); /* NECESSARY!!!, otherwise main thread will end, and other threads will never be run*/
}