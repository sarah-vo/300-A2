#include "send.h"
#include "list.h"
#include "global_var.h"
#include "input.h"
#include "main.h"
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

//    //initializing and parsing IP address to remoteAddress
//    bzero(&remoteAddress, sizeof(remoteAddress));
//    remoteAddress.sin_family = AF_INET;
//    remoteAddress.sin_port = htons(atoi(rPort));
//    remoteAddress = (struct sockaddr_in)(addrInfo->ai_addr);
//    if(inet_pton(AF_INET, addrInfo->ai_addr, &remoteAddress.sin_addr) != 1){
//        printf("Parsing IP data failed!\n");
//        exit(EXIT_FAILURE);
//    }


//    //freeing addrInfo
//    freeaddrinfo(addrInfo);
}

void* sendRoutine(){
    while(true){

        char msg[MAX_LEN];
        // TODO TEST FUNCTION
        if (!fgets(msg, sizeof msg, stdin)) {
            printf("Reading message failed!");
            exit(EXIT_READ_FAIL);
        }
        //TODO TEST FUNCTION END
        long bytes_sent = sendto(sd, msg, strlen(msg), 0, addrInfo->ai_addr, addrInfo->ai_addrlen);
        if (bytes_sent == -1){
            printf("Send routine failed! \n");
            exit(EXIT_FAILED);
        }
        else{
            printf("Send successfully!");
        }
        //Terminate if "!" is detected
        if(strcmp(msg,"!\n") == 0){
            //free(msg);
            //TODO FILL IN MAIN_TERMINATE
            //main_terminate();
        }
        else {
            //free(msg);
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
////////TODO TEST FUNCTION
int main(int argc, char* args[]){
    char* localPort = args[1];
    char* remoteName = args[2];
    char* remotePort = args[3];
    socket_initialize(localPort);
    //input_initialize();
    send_initialize(remotePort, remoteName);
    struct addrInfo *test = (struct addrInfo *) addrInfo;
    pthread_join(sendThread, NULL);

}

void send_terminate(){
    pthread_cancel(sendThread);
    int threadJoin = pthread_join(sendThread, NULL) == 0;
    if(threadJoin != 0){
        printf("failed to join thread!\n");
        exit(EXIT_THREAD_FAIL);
    }
}

