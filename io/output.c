#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "output.h"
#include "list.h"

static List* pReceived;
static pthread_t thread_output;

void output_initialize()
{
    pReceived = List_create();  // create a shared list

    if(pReceived == NULL){
        printf("Error: failed to create a shared list in output.c\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&thread_output), NULL, output_print, NULL) != 0){
        printf("Error: failed to create a output thread.\n");
        exit(EXIT_FAILURE);
    }
}

void output_consume()
{
    
}

void *output_print(char* msgTx)
{
    
}

void output_terminate()
{
    
}