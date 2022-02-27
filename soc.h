#ifndef A2_SOC_H
#define A2_SOC_H

int sd;     // socket descriptor

void socket_initialize(char* port);

void socket_close();

#endif //A2_SOC_H
