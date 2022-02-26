#include "input.h"
#include "list.h"
#include "global_var.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static List* input_list;
static pthread_t threadPID;




void* input_initialize(){
    input_list = List_create();
    if(input_list == NULL){
        printf("Input list failed to initialize!\n");
        exit(EXIT_LIST_FAIL);
    }
    if(pthread_create(&threadPID, NULL, input, NULL) != 0){
        printf("Input thread failed to initialize!\n");
        exit(EXIT_THREAD_FAIL);
    }
    
}

void input_push(char* msg) {
    List_prepend(input_list, msg);
    free(msg);
}

void *input(){
    while(true){
        //read user input
        char msg[MAX_LEN];
//        int counter; //TEST FUNCTION
        if (!fgets(msg, sizeof msg, stdin)) {
            printf("Reading message failed!");
            exit(EXIT_READ_INPUT);
        }
//        //TEST FUNCTION
//        else{
//            counter = 1;
//        }
//        if(counter == 1){
//            printf("%s", msg);
//            counter = 0;
//        }
        int len = strlen(msg);
        char* pMessage = (char*)malloc(len);
        memcpy(pMessage, msg, len);
        //push to list
        input_push(msg);
    }
}

int main(){
    input();
    return 1;
}


char* input_read(){
    char* msg = List_trim(input_list);
    return msg;
}


void freeItem(void* item){
    free(item);
}

void input_terminate(){
    List_first(input_list);
    List_free(input_list, (*freeItem));
    pthread_cancel(threadPID);
} 

