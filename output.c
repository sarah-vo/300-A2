#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "output.h"
#include "receive.h"
#include "global_var.h"

static pthread_t thread_output;
static pthread_mutex_t mutex_out = PTHREAD_MUTEX_INITIALIZER;
static char* rxMsg;

/**
 * @brief Creates a pthread thread_output
 * 
 */
void output_initialize()
{
    // create a pthread_t thread_output and check if it's successfully completed
    if(pthread_create(&thread_output, NULL, output_consume, NULL) != 0){
        printf("Error: failed to create a output thread.\n");
        exit(EXIT_FAILED);
    }
}

/**
 * @brief Consumes an element from the shared list and prints it on the terminal
 * 
 * @param unused 
 * @return void* 
 */
void *output_consume(void* unused)
{

    while(1){
        
        // Check and wait until receive_thread signals to consume and print
        pthread_mutex_lock(&mutex_out);
        {
            rxMsg = receive_print_msg();    // print msg
            if(strcmp(rxMsg, "!") == 0){    // return null if ! is received
                free(rxMsg);
                return NULL;
            }
            free(rxMsg);
        }
        pthread_mutex_unlock(&mutex_out);
    }
}

/**
 * @brief Cancels and joins pthread thread_output 
 * 
 */
void output_terminate()
{
    if(pthread_cancel(thread_output) != 0){
        printf("Failed to cancel output_thread. error code: %s\n", strerror(thread_output));
    }

    if(pthread_join(thread_output, NULL) != 0){
        printf("failed to join output_thread. error code: %s\n", strerror(thread_output));
    }
}
