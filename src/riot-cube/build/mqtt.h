#ifndef MQTT_H
#define MQTT_H
#endif

int mqtt_init(void);
int con(void);
int discon(void);
int is_con(void);
int pub(char* payload, char* topic);

extern int mqtt_thread_pid;

int cmd_connect(int argc, char **argv);
int cmd_disconnect(int argc, char **argv);
int cmd_reg(int argc, char **argv);
int cmd_unreg(int argc, char **argv);
int cmd_pub(int argc, char **argv);
int cmd_sub(int argc, char **argv);
int cmd_unsub(int argc, char **argv);
int cmd_info(int argc, char **argv);

