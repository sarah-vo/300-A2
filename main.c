#include <stdio.h>
#include "input.h"
#include "send.h"
#include "receive.h"
#include "output.h"


int main(int argc,char **argv[]) {
    if(argc != 4){
        printf("Error: make sure to type main [your port number][remote machine name][remote port number]\n");
        return 1;
    }

    char* port = argv[1];
    char* rMachine = argv[2];
    char* rPort = argv[3];

    socket_initialize();
    
}
