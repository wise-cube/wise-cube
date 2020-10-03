#include "mqtt_wrapper.h"
#include "utils.h"

int pub_shake_event(void){
    char         msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\"}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "shake_event");
    return pub(msg_buf, NULL);
}

int pub_button_ok_event(void){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\"}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "button_ok_event");
    return pub(msg_buf, NULL);
}

int pub_button_ko_event(void){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\"}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "button_ko_event");
    return pub(msg_buf, NULL);
}

int pub_answer_event(int ans_num){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\", \"num\":%d }";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "new_answer", ans_num);
    return pub(msg_buf, NULL);
}

int cmd_pub_shake_event(int argc, char **argv){
    return pub_shake_event();
}

int cmd_pub_button_ok_event(int argc, char **argv){
    return pub_button_ok_event();
}

int cmd_pub_button_ko_event(int argc, char **argv){
    return pub_button_ok_event();
}

int cmd_pub_answer_event(int argc, char **argv){
    return pub_answer_event(0);
}
int cmd_mqtt_init(int argc, char **argv){
    return mqtt_init();
}
