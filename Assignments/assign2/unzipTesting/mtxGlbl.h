#ifndef MTXGLBL_H
#define MTXGLBL_H

#include <pthread.h>

/* global variables declared */
extern pthread_mutex_t mtx; 
extern pthread_cond_t prodCond,consCond ; 

#endif