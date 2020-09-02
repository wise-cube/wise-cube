#ifndef RIOT_CUBE_UTILS_H
#define RIOT_CUBE_UTILS_H

#endif //RIOT_CUBE_UTILS_H
#include "jsmn.h"

#include <stdio.h>
#include <string.h>
//#include <stdbool.h>
//#include "xtimer.h"
//#include "shell.h"
//#include "thread.h"
//#include "mutex.h"
//#include "paho_mqtt.h"
//#include "MQTTClient.h"

#define EXIT_SUCCESS 0

#define EXIT_FAILURE 1


int jsoneq(const char *json, jsmntok_t *tok, const char *s);
int json_conv(char* msg);