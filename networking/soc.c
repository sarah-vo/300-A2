#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "soc.h"

// this is the whole code by Brian Fraser

#define MAX_UDP 65535   // a theoretical limit for a UDP datagram
#define PORT 22110

int main()
{
    // Address structure (create address):
    // Below here can be in socket_initialize() function
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family= AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);

    // Create/open socket and bind to port
    int sd = socket(PF_INET, SOCK_DGRAM, 0);    // socket descriptor
    bind(sd, (struct sockaddr*) &sin, sizeof(sin));

    // socket_receive()
    struct sockaddr_in sinRemote;   // important if you have a server to report back to
    unsigned int sin_len = sizeOf(sinRemote);
    char msgRx[MAX_UDP];  // buffer for Rx
    // fill the buffer with the message receiived
    int bytesRX = recvfrom(sd, msgRx, MAX_UDP, 0,
        (struct sockaddr*) &sinRemote, &sin_len);

    // Null terminated (string):
    int terminatedldx = (bytesRX < MAX_UDP) ? bytesRX : MAX_UDP - 1;
    msgRx[terminatedldx] = 0;
    printf("Message received (%d bytes): %s'\n", 
        bytesRX, msgRx);

    // Extract the value from the message:
    // (process the msg any way your app requires):
    int incMe = atoi(msgRx);    // ascii to int

    // socket_reply() - need to watch for buffer overflow
    // create reply
    char msgTx[MAX_UDP];
    sprintf(msgTx, "Math: %d + 1 = %d\n", incMe, incMe +1);   // msgTx = array of char = strinig

    // send reply
    sin_len = sizeof(sinRemote);
    sendto(sd, msgTx, strlen(msgTx), 0,
        (struct socketaddr *) &sinRemote, sin_len);
    //(struct socketaddr *) &sinRemote, sin_len) = where to

    // close socket when done: maybe make socket_close() as well?
    close(sd);


}