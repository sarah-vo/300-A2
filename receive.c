#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "list.h"
#include "receive.h"
#include "list.h"

static pthread_t thread_output;
static pthread_cond_t cond_out = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex_out = PTHREAD_MUTEX_INITIALIZER;
static int sd_out;
static char* rxMsg;
static List* pReceived;

void receive_initialize()
{
    pReceived = List_create();
    if(pReceived == NULL){
        printf("Error: failed to create a shared list in output.c\n");
        exit(EXIT_FAILURE);
    }
}

void receive_produce(char* msgRx)
{

}

void receive_terminate()
{
    
}