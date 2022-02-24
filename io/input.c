#include "input.h"
#include "list.h"
#include <pthread.h>
#include <stdio.h>

static List* input_list;
static pthread_t threadPID;


void input_initialize(){
    input_list = List_create();
    if(input_list == NULL){
        printf("Input thread error!");
    }
    
}

void input_to_the_list(char* msgRx)
{

}

void input_terminate()
{

} 

