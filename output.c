#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "output.h"
#include "list.h"

static pthread_t thread_output;
static pthread_cond_t cond_out = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex_out = PTHREAD_MUTEX_INITIALIZER;
static int sd_out;
static char* rxMsg;

void* output_print(char* msgRx);

void output_initialize()
{
    if(pthread_create(&thread_output, NULL, output_print, NULL) != 0){
        printf("Error: failed to create a output thread.\n");
        exit(EXIT_FAILURE);
    }
}

void output_consume()
{
    
}

void *output_print(char* msgTx)
{
    // socket_address
    struct sockaddr_in sin_out;
    memset(&sin_out, 0, sizeof(sin_out));
    sin_out.sin_family = AF_INET;
    sin_out.sin_addr.s_addr = htonl(INADDR_ANY);
    // sin_out.sin_port = htons();

    while(1){    
        pthread_mutex_lock(&mutex_out);
        {
            pthread_cond_signal(&cond_out);
        }
        pthread_mutex_unlock(&mutex_out);
    }
}

void output_terminate()
{
    
}
