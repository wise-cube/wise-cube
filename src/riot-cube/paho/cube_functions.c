#include "cube_functions.h"

extern MQTTClient client;
extern Network network;
extern unsigned char buf[BUF_SIZE];
extern unsigned char readbuf[BUF_SIZE];
extern char _topic_to_subscribe[MAX_TOPICS][MAX_LEN_TOPIC];

extern int status;
// 0 - OFFLINE, 1-DISCONNECTED, 1- CONNECTED, 2- PAIRED
extern int _cmd_pub(int argc, char **argv);

void send_message(char* message_string) {
    return;
}
void on_message(char* message_string) {
    return;
}
void cube_init(void) {
#ifdef MODULE_LWIP
    /* let LWIP initialize */
    xtimer_sleep(1);
#endif

    NetworkInit(&network);

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE,
                   readbuf,
                   BUF_SIZE);
    printf("Running mqtt paho example. Type help for commands info\n");

    MQTTStartTask(&client);
    return;

}

void cube_loop(void ) {
    return;
}


int cmd_pub_shake_event(int argc, char **argv){
    char        msg_type_buf[]     ="shake_event";
    const char  msg_template_buf[] ="{\"msg_type\":\"%s\"}";
    char*       msg                =malloc(MSG_BUF_SIZE*sizeof(char));
    memset(msg, 0, MSG_BUF_SIZE);

    snprintf(msg, MSG_BUF_SIZE-1, msg_template_buf, msg_type_buf);

    int  _argc=2;
    char * _argv_buf[2] = {0};
    char ** _argv=(char **)&_argv_buf;
    char * arg1 = NULL;
    char * arg2 = msg;
    _argv[0] = arg1;
    _argv[1] = arg2;

    int status = _cmd_pub(_argc, _argv);
    free(msg);

    return status;
}

int cmd_pub_button_ok_event(int argc, char **argv){
    char        msg_type_buf[]     ="button_ok_event";
    const char  msg_template_buf[] ="{\"msg_type\":\"%s\"}";
    char*       msg                =malloc(MSG_BUF_SIZE*sizeof(char));
    memset(msg, 0, MSG_BUF_SIZE);

    snprintf(msg, MSG_BUF_SIZE-1, msg_template_buf, msg_type_buf);

    int  _argc=2;
    char * _argv_buf[2] = {0};
    char ** _argv=(char **)&_argv_buf;
    char * arg1 = NULL;
    char * arg2 = msg;
    _argv[0] = arg1;
    _argv[1] = arg2;

    int status = _cmd_pub(_argc, _argv);
    free(msg);

    return status;
}

int cmd_pub_button_ko_event(int argc, char **argv){
    char        msg_type_buf[]     ="button_ko_event";
    const char  msg_template_buf[] ="{\"msg_type\":\"%s\"}";
    char*       msg                =malloc(MSG_BUF_SIZE*sizeof(char));
    memset(msg, 0, MSG_BUF_SIZE);

    snprintf(msg, MSG_BUF_SIZE-1, msg_template_buf, msg_type_buf);

    int  _argc=2;
    char * _argv_buf[2] = {0};
    char ** _argv=(char **)&_argv_buf;
    char * arg1 = NULL;
    char * arg2 = msg;
    _argv[0] = arg1;
    _argv[1] = arg2;

    int status = _cmd_pub(_argc, _argv);
    free(msg);

    return status;
}