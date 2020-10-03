#ifndef RIOT_CUBE_PAHO_H
#define RIOT_CUBE_PAHO_H

#include "paho_mqtt.h"
#include "MQTTClient.h"



#define BUF_SIZE                        1024
#define MQTT_VERSION_v311               4       /* MQTT v3.1.1 version is 4 */
#define COMMAND_TIMEOUT_MS              4000
#define DEFAULT_MQTT_USER               ""
#define DEFAULT_MQTT_PWD                ""
#define DEFAULT_MQTT_PORT               1884
#define DEFAULT_KEEPALIVE_SEC           10
#define MAX_LEN_TOPIC                   100
#define MAX_TOPICS                      4
#define IS_CLEAN_SESSION                1
#define IS_RETAINED_MSG                 0

int mqtt_init(void);
int con(void);
int discon(void);
int is_con(void);
int pub(char* payload, char* topic);

void _on_msg_received(MessageData *data);

int cmd_con(int argc, char **argv);
int cmd_discon(int argc, char **argv);
int cmd_pub(int argc, char **argv);
int cmd_sub(int argc, char **argv);
int cmd_unsub(int argc, char **argv);
int cmd_mqtt_init(int argc, char **argv);

#endif //RIOT_CUBE_PAHO_H