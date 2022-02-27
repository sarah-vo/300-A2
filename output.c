#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "output.h"
#include "receive.h"
#include "list.h"
#include "soc.h"
#include "global_var.h"

static pthread_t thread_output;
static pthread_cond_t empty_out = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex_out = PTHREAD_MUTEX_INITIALIZER;
static int sd_out;
static char* rxMsg;

void output_initialize()
{
    // create a pthread_t thread_output and check if it's successfully completed
    if(pthread_create(&thread_output, NULL, output_consume, NULL) != 0){
        printf("Error: failed to create a output thread.\n");
        exit(EXIT_FAILED);
    }
}

void *output_consume(void* unused)
{

    while(1){
        
        // Check and wait until receive_thread signals to consume and print
        pthread_mutex_lock(&mutex_out);
        {
            // pthread_cond_wait(&empty_out, &mutex_out);
            receive_print_msg();
            // if(puts(rxMsg) < 0){
            //     perror("Error: failed to print the output to the screen.\n");
            //     exit(EXIT_FAILED);
            // }
        }
        pthread_mutex_unlock(&mutex_out);
    }
}

void output_terminate()
{
    pthread_cancel(thread_output);
}
