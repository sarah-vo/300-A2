#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "soc.h"

// this is the whole code by Brian Fraser

#define MAX_UDP 65535   // a theoretical limit for a UDP datagram

static int sd; // socket descriptor

void socket_initialize(char* port, char* rMachine){
    struct addrinfo hints;
    struct addrinfo *serverInfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;          // Force IPV4 only    
    hints.ai_socktype = SOCK_DGRAM;     // UDP socket
    
    // getaddrinfo fails to return the information of the remote machine
    if(getaddrinfo(rMachine, port, &hints, &serverInfo) != 0){
        printf("Error: Incorrect address or port.\n");
        exit(EXIT_FAILURE);
    }

    // Create/open socket and bind to port
    sd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

    if(sd == -1){
        perror("Error: failed to create a socket\n");
        exit(EXIT_FAILURE);
    }

    if(bind(sd, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1){
        perror("Error: failed to bind the socket to the port\n");
        exit(EXIT_FAILURE);
    }
}

void socket_close()
{
    close(sd);
}