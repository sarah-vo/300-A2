
#ifndef A2_GLOBAL_VAR_H
#define A2_GLOBAL_VAR_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//byte limit of a UDP packet
static int MAX_LEN = 65535; 

//exit code
//static int EXIT_SUCCESS = 0;
static int EXIT_FAILED = -1;
static int EXIT_THREAD_FAIL = -2;
static int EXIT_LIST_FAIL = -3;
static int EXIT_READ_FAIL = -4;

//signals
#endif //A2_GLOBAL_VAR_H
