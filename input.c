#include "input.h"
#include "list.h"
#include "global_var.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static List* input_list;
static pthread_mutex_t sendMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t emptyCond = PTHREAD_COND_INITIALIZER;
static pthread_t threadPID;


void input_initialize(){
    input_list = List_create();
    if(input_list == NULL){
        printf("Input list failed to initialize!\n");
        exit(EXIT_LIST_FAIL);
    }
    if(pthread_create(&threadPID, NULL, input, NULL) != 0){
        printf("Input thread failed to initialize!\n");
        exit(EXIT_THREAD_FAIL);
    }
    //TODO TEST FUNCTION
    //pthread_join(threadPID,NULL);

}

//Reads message from last position of list and delete it. The last one is the oldest message.
char* input_read(){
    char *msg = NULL;
    pthread_mutex_lock(&sendMutex);
    {
        if(List_count(input_list) <= 0){
            pthread_cond_wait(&emptyCond, &sendMutex);
        }
        else{
            msg = List_trim(input_list);
        }
    }
    pthread_mutex_unlock(&sendMutex);
    return msg;
}

void input_prepend(char* msg) {
    pthread_mutex_lock(&sendMutex);
    {
        int PREPEND_SUCCESS = (List_prepend(input_list, msg) == 0);
        if (!PREPEND_SUCCESS) {
            printf("Failed to prepend to input list!\n");
            free(msg);
            exit(EXIT_LIST_FAIL);
        }
        else if(List_count(input_list) == 1){
            pthread_cond_signal(&emptyCond);
        }
    }
    pthread_mutex_unlock(&sendMutex);
}

void *input(){
    while(1){
        //read user input
        char msg[MAX_LEN];
        if (!fgets(msg, sizeof msg, stdin)) {
            printf("Reading message failed!");
            exit(EXIT_READ_INPUT);
        }

        int len = (int)strlen(msg);
        char* pMessage = (char*)malloc(len + 1);
        strcpy(pMessage, msg);
        //push to list
        input_prepend(msg);
        //TODO TEST FUNCTION
        //printf("%s", input_read());
    }
}
//TODO TEST FUNCTION
//int main(){
//    input_initialize();
//    return 1;
//}






void freeItem(void* item){
    free(item);
}

void input_terminate(){
    List_first(input_list);
    List_free(input_list, (freeItem));
    pthread_cancel(threadPID);
} 

