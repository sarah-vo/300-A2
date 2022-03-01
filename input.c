#include "input.h"
#include "list.h"
#include "global_var.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static List* input_list;
static pthread_mutex_t inputMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t inputThread;
pthread_cond_t inputWait = PTHREAD_COND_INITIALIZER;




//Reads message from last position of list and delete it. The last one is the oldest message.
char* input_read(){
    char *msg = NULL;
    pthread_mutex_lock(&inputMutex);
    {
        //wait until there is input from computer
        if(List_count(input_list) <= 0){
            pthread_cond_wait(&inputWait, &inputMutex);
        }
        else{
            msg = List_trim(input_list);
        }
    }
    pthread_mutex_unlock(&inputMutex);
    return msg;
}

void input_prepend(char* msg) {
    pthread_mutex_lock(&inputMutex);
    {
        int PREPEND_SUCCESS = (List_prepend(input_list, msg) == 0);
        if (!PREPEND_SUCCESS) {
            printf("Failed to prepend to inputRoutine list!\n");
            free(msg);
            exit(EXIT_LIST_FAIL);
        }
        else if(List_count(input_list) == 1){
            pthread_cond_signal(&inputWait);
        }
    }
    pthread_mutex_unlock(&inputMutex);
}

void *inputRoutine(){
    while(1){
        //read user inputRoutine
        char msg[MAX_LEN];
        if (!fgets(msg, sizeof msg, stdin)) {
            printf("Reading message failed!");
            exit(EXIT_READ_FAIL);
        }

        int len = (int)strlen(msg);
        char* pMessage = (char*)malloc(len + 1);
        strcpy(pMessage, msg);
        //push to list
        input_prepend(pMessage);
        sendSignalChange(UNLOCK);

    }
}

void input_initialize(){
    input_list = List_create();
    if(input_list == NULL){
        printf("Input list failed to initialize!\n");
        exit(EXIT_LIST_FAIL);
    }
    if(pthread_create(&inputThread, NULL, inputRoutine, NULL) != 0){
        printf("Input thread failed to initialize!\n");
        exit(EXIT_THREAD_FAIL);
    }


}

void freeItem(void* item){
    free(item);
}

void input_terminate() {
    List_free(input_list, (freeItem));
    pthread_cancel(inputThread);
    int threadJoin = pthread_join(inputThread, NULL) == 0;
    if (threadJoin != 0) {
        printf("failed to join thread!\n");
        exit(EXIT_THREAD_FAIL);

    }

}

////TODO TEST FUNCTION
//int main(){
//    input_initialize();
//    pthread_join(inputThread, NULL);
//    return 1;
//}
