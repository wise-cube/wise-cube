#ifndef RIOT_CUBE_PAHO_H
#define RIOT_CUBE_PAHO_H

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

#include "utils.h"

unsigned get_qos(const char *str);
void _on_msg_received(MessageData *data);
int _cmd_discon(int argc, char **argv);
int _cmd_con(int argc, char **argv);
int _cmd_pub(int argc, char **argv);
int _cmd_sub(int argc, char **argv);
int _cmd_unsub(int argc, char **argv);

#endif //RIOT_CUBE_PAHO_H