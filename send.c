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
//using sd from soc.h

void addr_initialize(char* rPort, char* rHostname){

    //initializing hints
    struct addrinfo hints;
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_canonname = rHostname;
    hints.ai_protocol = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    //applying getaddrinfo
    struct addrinfo *addrInfo;
    if(getaddrinfo(rHostname,rPort, &hints, &addrInfo) != 0){
        printf("Getting address info failed!\n");
        exit(EXIT_FAILURE);
    }

    //initializing and parsing IP address to remoteAddress
    bzero(&remoteAddress, sizeof(remoteAddress));
    remoteAddress.sin_family = AF_INET;
    remoteAddress.sin_port = htons(atoi(rPort));
    if(inet_pton(AF_INET, addrInfo->ai_addr->sa_data, &remoteAddress.sin_addr) != 1){
        printf("Parsing IP data failed!\n");
        exit(EXIT_FAILURE);
    }
    remoteAddress_len = addrInfo->ai_addrlen;

    //freeing addrInfo
    freeaddrinfo(addrInfo);
}


void* sendRoutine(){
    while(true){

        char* msg = input_read();
        long bytes_sent = sendto(sd, msg, strlen(msg), 0, (struct sockaddr*)&remoteAddress, remoteAddress_len);
        if (bytes_sent == -1){
            printf("Send routine failed! \n");
            exit(EXIT_FAILED);
        }
        //Terminate if "!" is detected
        if(strcmp(msg,"!\n") == 0){
            free(msg);
            main_terminate();
        }
        else {
            free(msg);
        }
    }
}

void send_initialize(char* rPort, char* rHostname){
    addr_initialize(rPort, rHostname);

    bool threadCreated = (pthread_create(&sendThread, NULL, sendRoutine, NULL) == 0);
    if(!threadCreated){
        printf("Thread creation failed!\n");
        exit(EXIT_THREAD_FAIL);
    }


}
////TODO TEST FUNCTION
//int main(int argc, char* args[]){
//    char* localPort = args[1];
//    char* remoteName = args[2];
//    char* remotePort = args[3];
//    socket_initialize(localPort);
//    input_initialize();
//    send_initialize(remotePort, remoteName);
//}

void send_terminate(){
    pthread_cancel(sendThread);
    int threadJoin = pthread_join(sendThread, NULL) == 0;
    if(threadJoin != 0){
        printf("failed to join thread!\n");
        exit(EXIT_THREAD_FAIL);
    }
}