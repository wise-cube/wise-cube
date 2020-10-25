#ifndef RIOT_CUBE_CUBE_FUNCTIONS_H
#define RIOT_CUBE_CUBE_FUNCTIONS_H

#define MSG_BUF_SIZE 128

#include "mqtt.h"

int pub_shake_event(void);
int pub_short_press_event(void);
int pub_long_press_event(void);
int pub_answer_event(int ans_num);
int pub_face_change_event(int ans_num);

int cmd_con(int argc, char **argv);
int cmd_discon(int argc, char **argv);
int cmd_pub(int argc, char **argv);
int cmd_sub(int argc, char **argv);
int cmd_unsub(int argc, char **argv);
int cmd_mqtt_init(int argc, char **argv);


int cmd_pub_shake_event(int argc, char **argv);
int cmd_pub_short_press_event(int argc, char **argv);
int cmd_pub_long_press_event(int argc, char **argv);
int cmd_pub_answer_event(int argc, char **argv);

#endif //RIOT_CUBE_CUBE_FUNCTIONS_H
