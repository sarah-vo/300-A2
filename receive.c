#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "list.h"
#include "receive.h"
#include "soc.h"
#include "global_var.h"

void receive_add_to_list(char* rxMsg);

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
        exit(EXIT_FAILED);
    }

    // create a pthread_t thread_receive and check if it's successfully completed
    if(pthread_create(&thread_receive, NULL, receive_produce, NULL) != 0){
        perror("Error: failed to create a output thread.\n");
        exit(EXIT_FAILED);
    }
}

void* receive_produce(void* unused)
{
    struct sockaddr_in rSin;
    unsigned int sin_len = sizeof(rSin);
    char rx[MAX_LEN];

    while(1){
        memset(&rSin, 0, sizeof(rSin));
        rSin.sin_family = AF_INET;
        rSin.sin_addr.s_addr = htonl(INADDR_ANY);
        int bytesRx = recvfrom(sd, rx, MAX_LEN, 0, (struct sockaddr *) &rSin, &sin_len);
        if(bytesRx < 0){
            perror("Error: failed to receive message.\n");
            exit(EXIT_FAILED);
        }
        strcpy(rxMsg, rx);
        receive_add_to_list(rxMsg);
        // sin_out.sin_port = htons();
    }
}

void receive_add_to_list(char* rxMsg){
    // check if any other thread is reading/writing the shared list
    pthread_mutex_lock(&mutex_out);
    {
        if(List_prepend(pReceived, rxMsg) != 0){
            perror("Error: failed to produce in receive_thread.\n");
            exit(EXIT_FAILED);
        }

        if(List_count(pReceived) != 0){    // signal the send_thread if there's anything consumable in the list
            pthread_cond_signal(&empty_out);
        }
    }
    pthread_mutex_unlock(&mutex_out);
}

void receive_print_msg(){
    char* msg;

    // check if any other thread is reading/writing the shared list
    pthread_mutex_lock(&mutex_out);
    {
        // Use a condition variable to check if there's anything to consume
        if(List_count(pReceived) == 0){
            pthread_cond_wait(&empty_out, &mutex_out);
        }
        msg = List_remove(pReceived);        
    }
    pthread_mutex_unlock(&mutex_out);

    if(puts(msg) < 0){
        perror("Error: failed to print the output to the screen.\n");
        exit(EXIT_FAILED);
    }
    // return msg;
}

void receive_free_list(void* msg){
    msg = NULL;
}

void receive_terminate()
{
    pthread_cancel(thread_receive);

    FREE_FN pFreeFn = &receive_free_list;
    List_free(pReceived, receive_free_list);
}