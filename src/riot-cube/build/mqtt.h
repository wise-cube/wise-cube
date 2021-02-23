
#ifndef MQTT_H
#define MQTT_H
#endif


#define BUF_SIZE                        1024
#define MQTT_VERSION_v311               4       /* MQTT v3.1.1 version is 4 */
#define COMMAND_TIMEOUT_MS              4000
#define DEFAULT_MQTT_USER               ""
#define DEFAULT_MQTT_PWD                ""
#define DEFAULT_MQTT_PORT               1883
#define DEFAULT_KEEPALIVE_SEC           10
#define MAX_LEN_TOPIC                   128
#define MAX_TOPICS                      2
#define IS_CLEAN_SESSION                1
#define IS_RETAINED_MSG                 0

int mqtt_init(void);
int con(void);
int discon(void);
int is_con(void);
int pub(char* payload, char* topic);

extern int mqtt_thread_pid;

