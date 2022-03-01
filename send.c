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
struct sockaddr_in remoteAddress;
socklen_t remoteAddress_len;
struct addrinfo *addrInfo;

pthread_mutex_t sendMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  sendCond = PTHREAD_COND_INITIALIZER;

//using sd from soc.h

//TODO TEST FUNC
pthread_mutex_t mutexTEST = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condTEST = PTHREAD_COND_INITIALIZER;

void addr_initialize(char* rPort, char* rHostname)
{

    //applying getaddrinfo

    if(getaddrinfo(rHostname,rPort, NULL, &addrInfo) != 0){
        printf("Getting address info failed!\n");
        exit(EXIT_FAILURE);
    }
    remoteAddress_len = addrInfo->ai_addrlen;
}

void* sendRoutine(){
    while(true){
        pthread_mutex_lock(&sendMutex);
        {
            pthread_cond_wait(&sendCond, &sendMutex);
        }
        pthread_mutex_unlock(&sendMutex);

        char* msg = input_read();
        // TODO TEST FUNCTION
//        if (!fgets(msg, sizeof msg, stdin)) {
//            printf("Reading message failed!");
//            exit(EXIT_READ_FAIL);
//        }
        //TODO TEST FUNCTION END
        long bytes_sent = sendto(sd, msg, strlen(msg), 0, addrInfo->ai_addr, addrInfo->ai_addrlen);
        if (bytes_sent == -1){
            printf("Send routine failed! \n");
            exit(EXIT_FAILED);
        }
        //TODO TEST FUNCTION
//        else{
//            printf("Printed: %s", msg);
//        }
        //Terminate if "!" is detected
        bool terminateChat = strcmp(msg,"!\n") == 0;
        if(terminateChat){
            printf("Keystroke ! detected");
            free(msg);
            //TODO FILL IN MAIN_TERMINATE
            main_terminate();
        }
        else {
            free(msg);
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

void sendSignalChange(enum SIGNAL signal){
    if(signal == UNLOCK){
        pthread_cond_signal(&sendCond);
    }
}
//////////TODO TEST FUNCTION
//int main(int argc, char* args[]){
//    char* localPort = args[1];
//    char* remoteName = args[2];
//    char* remotePort = args[3];
//    socket_initialize(localPort);
//    input_initialize();
//    send_initialize(remotePort, remoteName);
//    struct addrInfo *test = (struct addrInfo *) addrInfo;
//    pthread_exit(NULL);
//}

void send_terminate(){
    pthread_cancel(sendThread);
    int threadJoin = pthread_join(sendThread, NULL) == 0;
    if(threadJoin != 0){
        printf("failed to join thread!\n");
        exit(EXIT_THREAD_FAIL);
    }
}

