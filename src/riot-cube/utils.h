#ifndef RIOT_CUBE_UTILS_H
#define RIOT_CUBE_UTILS_H

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// #define STATE_ERROR (-1)
// #define STATE_UNINITIALIZED 0
// #define STATE_DISCONNECTED 1
// #define STATE_CONNECTED 2
// #define STATE_PAIRED 3

#include "globals.h"

void wlog_res(char* action, int status);
unsigned get_qos(const char *str);

enum enum_state_t {
    STATE_ERROR,
    STATE_UNINITIALIZED,
    STATE_DISCONNECTED,
    STATE_CONNECTED,
    STATE_PAIR_REQ,
    STATE_PAIRED,
};

typedef enum enum_state_t state_t;
void set_state(state_t new_state);

extern state_t current_state ;
void wlog(char* msg);
#endif //RIOT_CUBE_UTILS_H