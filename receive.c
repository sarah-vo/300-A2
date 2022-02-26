#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "list.h"
#include "receive.h"
#include "list.h"

static pthread_t thread_receive;
static pthread_cond_t cond_out = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex_out = PTHREAD_MUTEX_INITIALIZER;
static List* pReceived;
static int sd_out;
static char* rxMsg;


void receive_initialize()
{
    // Create a list pReceived that will store all the messages received via udp
    pReceived = List_create();
    if(pReceived == NULL){
        perror("Error: failed to create a shared list in output.c\n");
        exit(EXIT_FAILURE);
    }

    // create a pthread_t thread_receive and check if it's successfully completed
    if(pthread_create(&thread_receive, NULL, receive_produce, NULL) != 0){
        perror("Error: failed to create a output thread.\n");
        exit(EXIT_FAILURE);
    }
}

void* receive_produce(void* unused)
{
    while(1){

    }
}

void receive_terminate()
{
    
}