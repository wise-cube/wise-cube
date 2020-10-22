#ifndef RIOT_CUBE_UTILS_H
#define RIOT_CUBE_UTILS_H

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include "globals.h"

void wlog_res(char* action, int status);
unsigned get_qos(const char *str);

enum state {
    error = -1,
    uninitialized = 0,
    disconnected = 1,
    connected = 2,
    paired = 4,
};

void wlog(char* msg);
#endif //RIOT_CUBE_UTILS_H