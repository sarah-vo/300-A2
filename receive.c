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
static pthread_cond_t empty_out = PTHREAD_COND_INITIALIZER;
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
        
    // socket_address
    struct sockaddr_in sin_out;
    memset(&sin_out, 0, sizeof(sin_out));
    sin_out.sin_family = AF_INET;
    sin_out.sin_addr.s_addr = htonl(INADDR_ANY);
    // sin_out.sin_port = htons();


    }
}

void receive_add_to_list(char* rxMsg){
    // check if any other thread is reading/writing the shared list
    pthread_mutex_lock(&mutex_out);
    {
        if(List_prepend(pReceived, rxMsg) != 0){
            perror("Error: failed to produce in receive_thread.\n");
            exit(EXIT_FAILURE);
        }
    }
    pthread_mutex_unlock(&mutex_out);
}

char* receive_get_msg(){
    char* msg;

    // check if any other thread is reading/writing the shared list
    pthread_mutex_lock(&mutex_out);
    {
        msg = List_remove(pReceived);        
    }
    pthread_mutex_unlock(&mutex_out);

    return msg;
}

void receive_terminate()
{
    pthread_cancel(thread_receive);
}