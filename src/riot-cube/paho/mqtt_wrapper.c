#include "mqtt_wrapper.h"
#include "utils.h"

extern char current_face;
int pub_shake_event(void){
    char         msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\"}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "shake_event");
    return pub(msg_buf, NULL);
}
int pub_short_press_event(void){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\"}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "short_press_event");
    return pub(msg_buf, NULL);
}
int pub_long_press_event(void){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\"}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "long_press_event");
    return pub(msg_buf, NULL);
}
int pub_answer_event(int ans_num){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\", \"num\":%d }";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "answer_event", ans_num);
    return pub(msg_buf, NULL);
}
int pub_face_change_event(int ans_num){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\", \"face\":'%c' }";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "new_face", current_face);
    return pub(msg_buf, NULL);
}

int cmd_pub_shake_event(int argc, char **argv){
    return pub_shake_event();
}

int cmd_pub_short_press_event(int argc, char **argv){
    return pub_short_press_event();
}

int cmd_pub_long_press_event(int argc, char **argv){
    return pub_long_press_event();
}

int cmd_pub_answer_event(int argc, char **argv){
    return pub_answer_event(0);
}

