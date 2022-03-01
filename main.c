#include <stdio.h>
#include "input.h"
#include "send.h"
#include "receive.h"
#include "output.h"
#include "soc.h"

void main_terminate(){
    send_terminate();
    output_terminate();
    input_terminate();
    receive_terminate();
    socket_close();
    exit(EXIT_SUCCESS);
}

int main(int argc,char *argv[]) {
    if(argc != 4){
        printf("Error: make sure to type main [your port number][remote machine name][remote port number]\n");
        return 1;
    }

    char* port = argv[1];
    char* rHostName = argv[2];
    char* rPort = argv[3];

    socket_initialize(port);
    input_initialize();
    send_initialize(rPort, rHostName);
    receive_initialize();
    output_initialize();
    pthread_exit(NULL);
    
}
