#ifndef RIOT_CUBE_CUBE_FUNCTIONS_H
#define RIOT_CUBE_CUBE_FUNCTIONS_H

#define MSG_BUF_SIZE 100

#include "paho.h"

int cmd_pub_shake_event(int argc, char **argv);
int cmd_pub_button_ok_event(int argc, char **argv);
int cmd_pub_button_ko_event(int argc, char **argv);
int cmd_pub_answer_event(int argc, char **argv);

int pub_shake_event(void);
int pub_button_ok_event(void);
int pub_button_ko_event(void);
int pub_answer_event(int);

#endif //RIOT_CUBE_CUBE_FUNCTIONS_H
