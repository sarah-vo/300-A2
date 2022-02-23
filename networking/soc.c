#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "soc.h"

// this is the whole code by Brian Fraser

#define MAX_UDP 65535   // a theoretical limit for a UDP datagram
#define PORT 22110

int static sd; // socket descriptor

void socket_initialize(int port){
    // Address structure (create address):
    // Below here can be in socket_initialize() function
    struct sockaddr_in socketAdd;
    memset(&socketAdd, 0, sizeof(socketAdd));
    socketAdd.sin_family = AF_INET;
    socketAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    socketAdd.sin_port = htons(port);

    // Create/open socket and bind to port
    sd = socket(PF_INET, SOCK_DGRAM, 0);

    if(sd == -1){
        perror("Failed to create a socket\n");
        exit(EXIT_FAILURE);
    }

    if(bind(sd, (struct sockaddr*) &socketAdd, sizeof(socketAdd)) == -1){
        perror("Failed to bind the socket to the port\n");
        exit(EXIT_FAILURE);
    }
}

void socket_receive(){

}

int main()
{


    socket_initialize(PORT);

    // socket_receive()
    while(1){
        struct sockaddr_in sinRemote;   // important if you have a server to report back to
        unsigned int sin_len = sizeof(sinRemote);
        char msgRx[MAX_UDP];  // buffer for Rx
        // fill the buffer with the message received
        int bytesRX = recvfrom(sd, msgRx, MAX_UDP, 0,
            (struct sockaddr*) &sinRemote, &sin_len);

        // Null terminated (string):
        int terminatedldx = (bytesRX < MAX_UDP) ? bytesRX : MAX_UDP - 1;
        msgRx[terminatedldx] = 0;
        // printf("Message received (%d bytes): %s'\n", 
        //     bytesRX, msgRx);

        // Extract the value from the message:
        // (process the msg any way your app requires):
        // int incMe = atoi(msgRx);    // ascii to int

        // socket_reply() - need to watch for buffer overflow
        // create reply
        // char msgTx[MAX_UDP];
        printf("%s", msgRx);   // msgTx = array of char = strinig

        // send reply
        // sin_len = sizeof(sinRemote);
        // sendto(sd, msgTx, strlen(msgTx), 0, (struct sockaddr *) &sinRemote, sin_len);
        //(struct socketaddr *) &sinRemote, sin_len) = where to
    }

    // close socket when done: maybe make socket_close() as well?
    // close(sd);


}