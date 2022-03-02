#include "input.h"
#include "list.h"
#include "global_var.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static List* input_list;
static pthread_mutex_t inputMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t inputThread;
pthread_cond_t inputWait = PTHREAD_COND_INITIALIZER;



//Reads message from last position of list and delete it. The last one is the oldest message.
char* input_read(){
    return List_trim(input_list);
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
    }
    pthread_mutex_unlock(&inputMutex);
}

void *inputRoutine(){
    while(1){
        //read user inputRoutine

        char input[MAX_LEN];
        if (!fgets(input, sizeof input, stdin)) {
            printf("Reading message failed!");
            exit(EXIT_READ_FAIL);
        }


        char* msg = input;
        msg[strcspn(msg,"\n")] = '\0';
        int len = (int)strlen(msg);
        char* pMessage = (char*)malloc(len + 1);
        strcpy(pMessage, msg);
        //push to list
        input_prepend(pMessage);
        sendUnlockSignal(UNLOCK);

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
    int cancelThread = pthread_cancel(inputThread);
    if(cancelThread != 0){
        printf("failed to cancel thread! (send). error code: %s\n", strerror(cancelThread));

    }
    int threadJoin = pthread_join(inputThread, NULL) == 0;
    if (threadJoin != 0) {
        printf("failed to join thread! (input), error code: %s\n", strerror(threadJoin));
        exit(EXIT_THREAD_FAIL);

    }

}


