#include "mqtt_wrapper.h"
#include "utils.h"

extern char current_face;
int pub_shake_event(void){
    char         msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\"}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "shake_event");
    return pub(PUB_TOPIC, msg_buf);
}
int pub_short_press_event(void){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\"}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "short_press_event");
    return pub(PUB_TOPIC, msg_buf);
}
int pub_long_press_event(void){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\"}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "long_press_event");
    return pub(PUB_TOPIC, msg_buf);
}
int pub_answer_event(int ans_num){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\", \"num\":%d }";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "new_answer", ans_num);
    return pub(PUB_TOPIC, msg_buf);
}
int pub_face_change_event(int ans_num){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\", \"face\":'%c' }";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "new_face", current_face);
    return pub(PUB_TOPIC, msg_buf);
}



int cmd_discon(int argc, char **argv){
    (void)argc;
    (void)argv;

    int err = discon();
    wlog_res("cmd discon", err);
    return err;
}
int cmd_con(int argc, char **argv){
//    printf("usage: con <string msg> [broker ip ] [broker port]\n")
//    char * ip_addr = (argc > 1) ? argv[1] : NULL;
//    int  port = (argc > 2) ? atoi(argv[2]): 0;
//    char * ip_addr = (argc > 1) ? argv[1] : NULL;
//    int  port = (argc > 2) ? atoi(argv[2]): 0;

    return con();
}
int cmd_pub(int argc, char **argv){
//    printf("usage: pub <string msg> [topic name] \n")
    if (argc <= 1) {
        printf("usage: pub <string msg> [topic name] \n");
        return -1;
    }
    char* msg = argv[1];
    char* topic = (argc > 2) ? argv[2] : (char*)&PUB_TOPIC;
//    enum Qos qos = (argc > 3) ? get_qos(atoi(argv[3])) : NULL;

    return pub(msg, topic);
}
int cmd_sub(int argc, char **argv){

    if (argc == 1 ) {
        printf("usage: sub [topic name] \n");
        return -1;
    }

    char* topic = (argc > 1) ? argv[1]:(char*)&SUB_TOPIC;

    return sub(topic);

}
int cmd_unsub(int argc, char **argv){
    if (argc <= 1 ) {
        printf("usage: unsub [topic name] \n");
        return -1;
    }
    char* topic = (argc > 1) ? argv[1]:(char*)&SUB_TOPIC;

    return unsub(topic);
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

