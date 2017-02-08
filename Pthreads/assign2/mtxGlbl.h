#ifndef MTXGLBL_H
#define MTXGLBL_H

#include <pthread.h>

/* global variables, is this the best way? */
extern pthread_mutex_t mtx; // declare global mutex
extern pthread_cond_t prodCond,consCond ; //declare global conditions
extern pthread_cond_t termCond; // termination condition

#endif