#ifndef RIOT_CUBE_CUBE_FUNCTIONS_H
#define RIOT_CUBE_CUBE_FUNCTIONS_H

#include "paho.h"

void cube_init(void);
int cmd_pub_shake_event(int argc, char **argv);
int cmd_pub_button_ok_event(int argc, char **argv);
int cmd_pub_button_ko_event(int argc, char **argv);
int pub_shake_event(void);
int pub_button_ok_event(void);
int pub_button_ko_event(void);
#endif //RIOT_CUBE_CUBE_FUNCTIONS_H