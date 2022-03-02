#include "send.h"
#include "list.h"
#include "global_var.h"
#include "input.h"
#include "main.h"
#include "output.h"
#include "receive.h"
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "soc.h"

static pthread_t sendThread;
struct addrinfo *addrInfo;

pthread_mutex_t sendMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  sendCond = PTHREAD_COND_INITIALIZER;

//using sd from soc.h



void addr_initialize(char* rPort, char* rHostname)
{

    //applying getaddrinfo

    if(getaddrinfo(rHostname,rPort, NULL, &addrInfo) != 0){
        printf("Getting address info failed!\n");
        exit(EXIT_FAILURE);
    }
}

void* sendRoutine(){
    while(true){
        pthread_mutex_lock(&sendMutex);
        {
            pthread_cond_wait(&sendCond, &sendMutex);
        }
        pthread_mutex_unlock(&sendMutex);

        char* msg = input_read();

        long bytes_sent = sendto(sd, msg, strlen(msg), 0, addrInfo->ai_addr, addrInfo->ai_addrlen);
        //abnormal case
        if (bytes_sent == -1){
            printf("Send routine failed! \n");
            exit(EXIT_FAILED);
        }
        //normal case
        else{
            //user wants to terminate s-talk
            bool terminateChat = strcmp(msg,"!") == 0;
            if(terminateChat){
                free(msg);
                signal_terminate();
            }
            //normal execution
            else {
                free(msg);
            }
        }

    }
}

void send_initialize(char* rPort, char* rHostname){

    bool threadCreated = (pthread_create(&sendThread, NULL, sendRoutine, NULL) == 0);
    if(!threadCreated){
        printf("Thread creation failed!\n");
        exit(EXIT_THREAD_FAIL);
    }
    addr_initialize(rPort, rHostname);

}

void sendUnlockSignal(enum SIGNAL signal){
    if(signal == UNLOCK){
        pthread_cond_signal(&sendCond);
    }
}


void send_terminate(){
    int cancelThread = pthread_cancel(sendThread);
    if(cancelThread != 0){
        printf("failed to cancel thread! (send). error code: %s\n", strerror(cancelThread));

    }
    pthread_join(sendThread, NULL);
}

