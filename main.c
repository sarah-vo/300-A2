#include <stdio.h>
#include "input.h"
#include "send.h"
#include "receive.h"
#include "output.h"
#include "soc.h"

pthread_mutex_t terminateMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  terminateSignal = PTHREAD_COND_INITIALIZER;

void signal_terminate(){
    pthread_cond_signal(&terminateSignal);
}

int main(int argc,char *argv[]) {
    if(argc != 4){
        printf("Error: make sure to type main [your port number][remote machine name][remote port number]\n");
        return 1;
    }

    char* port = argv[1];
    char* rHostName = argv[2];
    char* rPort = argv[3];

    printf("Chat session started!\n\n");
    socket_initialize(port);
    input_initialize();
    send_initialize(rPort, rHostName);
    receive_initialize();
    output_initialize();

    pthread_mutex_lock(&terminateMutex);
    {
        pthread_cond_wait(&terminateSignal, &terminateMutex);
    }
    pthread_mutex_unlock(&terminateMutex);


    output_terminate();
    receive_terminate();
    send_terminate();
    input_terminate();
    socket_close();
    printf("\nChat session ended! Have a good day!\n");
    return 0;
}
