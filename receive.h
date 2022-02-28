#ifndef A2_RECEIVE_H
#define A2_RECEIVE_H

void receive_initialize();

void* receive_produce(void* unused);

char* receive_print_msg();

void receive_terminate();

#endif //A2_RECEIVE_H
