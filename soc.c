#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "soc.h"
#include "global_var.h"

static struct sockaddr_in sockadd;

void socket_initialize(char* port){
    memset(&sockadd, 0, sizeof(sockadd));
    sockadd.sin_family = AF_INET;
    sockadd.sin_addr.s_addr = htonl(INADDR_ANY);
    sockadd.sin_port = htons(atol(port));

    sd = socket(PF_INET, SOCK_DGRAM, 0);    // create a socket
    if(sd < 0){
        perror("Error: failied to create a socket.\n");
        exit(EXIT_FAILED);
    }

    if(bind(sd, (struct sockaddr *)&sockadd, sizeof(struct sockaddr_in)) < 0){
        perror("Error: failed to bind the port.\n");
        exit(EXIT_FAILED);
    }
}
// void socket_initialize(char* port, char* rMachine){
//     struct addrinfo hints;
//     struct addrinfo *serverInfo;

//     memset(&hints, 0, sizeof(hints));
//     hints.ai_family = AF_INET;          // Force IPV4 only    
//     hints.ai_socktype = SOCK_DGRAM;     // UDP socket
    
//     // getaddrinfo fails to return the information of the remote machine
//     if(getaddrinfo(rMachine, port, &hints, &serverInfo) != 0){
//         printf("Error: Incorrect address or port.\n");
//         exit(EXIT_FAILED);
//     }

//     // Create/open socket and bind to port
//     sd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

//     if(sd == -1){
//         perror("Error: failed to create a socket\n");
//         exit(EXIT_FAILED);
//     }

//     if(bind(sd, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1){
//         perror("Error: failed to bind the socket to the port\n");
//         exit(EXIT_FAILED);
//     }
// }

void socket_close()
{
    close(sd);
}
