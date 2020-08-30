#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "xtimer.h"
#include "shell.h"
#include "thread.h"
#include "mutex.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "jsmn.h"


#define BUF_SIZE                        1024
#define MQTT_VERSION_v311               4       /* MQTT v3.1.1 version is 4 */
#define COMMAND_TIMEOUT_MS              4000

#ifndef DEFAULT_MQTT_CLIENT_ID
#define DEFAULT_MQTT_CLIENT_ID          ""
#endif

#ifndef DEFAULT_MQTT_USER
#define DEFAULT_MQTT_USER               ""
#endif

#ifndef DEFAULT_MQTT_PWD
#define DEFAULT_MQTT_PWD                ""
#endif

/**
 * @brief Default MQTT port
 */
 
#define DEFAULT_MQTT_PORT               1884

/**
 * @brief Keepalive timeout in seconds
 */
#define DEFAULT_KEEPALIVE_SEC           10

#ifndef MAX_LEN_TOPIC
#define MAX_LEN_TOPIC                   100
#endif

#ifndef MAX_TOPICS
#define MAX_TOPICS                      4
#endif

#define IS_CLEAN_SESSION                1
#define IS_RETAINED_MSG                 0

static int topic_cnt = 0;

extern MQTTClient client;
extern Network network;

extern int group_id;
extern int player_id;


static char _topic_to_subscribe[MAX_TOPICS][MAX_LEN_TOPIC];

int discon();

int con();

int pub(char* topic, char* payload);

int sub(char* topic);

int _cmd_unsub(int argc, char **argv);

void new_group_req();

void new_player_req(char* group_id);

void new_player_accept_event(char* group_id, char* player_id);

void resume_group_req(char* token);

void new_game(char* game_id);

void player_req(char* group_id);

void new_question(char* game_id, char* point);

void new_answer(char* answer_id, char* answer_val);


