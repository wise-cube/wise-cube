#ifndef RIOT_CUBE_CUBE_FUNCTIONS_H
#define RIOT_CUBE_CUBE_FUNCTIONS_H

#define MSG_BUF_SIZE 128

#include "paho.h"

int pub_shake_event(void);
int pub_short_press_event(void);
int pub_long_press_event(void);
int pub_answer_event(int ans_num);
int pub_face_change_event(int ans_num);

int cmd_pub_shake_event(int argc, char **argv);
int cmd_pub_short_press_event(int argc, char **argv);
int cmd_pub_long_press_event(int argc, char **argv);
int cmd_pub_answer_event(int argc, char **argv);

#endif //RIOT_CUBE_CUBE_FUNCTIONS_H
