#ifndef A2_INPUT_H
#define A2_INPUT_H

#include "global_var.h"

void input_initialize();


char* input_read();

void input_terminate();

void sendSignalChange(enum SIGNAL signal);
#endif //A2_INPUT_H
