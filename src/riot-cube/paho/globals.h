#ifndef RIOT_CUBE_GLOBALS_H
#define RIOT_CUBE_GLOBALS_H

#define BROKER_PORT 1884
#define SUB_TOPIC "/to_cube/1"
#define PUB_TOPIC "/from_cube/1"
#define LWT_MSG "{ \"msg_type\" : \"disconnected\" }"
#define DEFAULT_MQTT_CLIENT_ID "cube_1"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "xtimer.h"
#include "shell.h"
#include "thread.h"
#include "mutex.h"



#endif //RIOT_CUBE_GLOBALS_H


