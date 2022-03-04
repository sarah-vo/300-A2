#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "soc.h"
#include "global_var.h"

static struct sockaddr_in sockadd;

/**
 * @brief Creates a socket and binds the local port to the local address
 * 
 * @param port 
 */
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

/**
 * @brief Closes the socket
 * 
 */
void socket_close()
{
    close(sd);
}