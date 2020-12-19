#include "log.h"
#include "cube_events.h"
#include "cube_states.h"
#include "cube_messaging.h"

static int shake_event_handler(void){
    char         msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\"}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "shake_event");
    return send( msg_buf);
}

static int short_press_event_handler(void){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\"}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "short_press_event");
    return send( msg_buf);
}

static int long_press_event_handler(void){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\"}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "long_press_event");
    return send( msg_buf);
}

static int nfc_event_handler(int event_arg){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\", arg : %d}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "nfc_event", event_arg);
    return send( msg_buf);
}

static int face_event_handler(int event_arg){
    char        msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\", arg : %d}";

    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "face_event", event_arg);
    return send( msg_buf);
}

void event_handler(event_id_t event_id, char event_arg ){
    LOG_INFO("Event %d arg: %d \n", event_id, event_arg);

    switch (event_id){
        case(EVENT_INIT):
            set_state(STATE_DISCONNECTED);
            break;
        case(EVENT_LPRESS):
            long_press_event_handler();
            break;
        case(EVENT_SPRESS):
            short_press_event_handler();
            break;
        case(EVENT_FACE):
            face_event_handler(event_arg);
            break;
        case(EVENT_SHAKE):
            shake_event_handler();
            break;
        case(EVENT_NFC):
            nfc_event_handler(event_arg);
            break;
        case(EVENT_FACE_ON):
            break;
        case(EVENT_FACE_OFF):
            break;
        case(EVENT_GAME_ON):
            break;
        case(EVENT_GAME_OFF):
            break;
        case(EVENT_PAIR_REQ):
            break;
        case(EVENT_PAIR_OK):
            break;
        case(EVENT_CON):
            if (get_state() < STATE_CONNECTED){
                set_state(STATE_CONNECTED);
            }
            break;
        case(EVENT_DISCON):
            set_state(STATE_DISCONNECTED);
            break;
        // case(EVENT_SUB):
        //     set_state(STATE_SUBSCRIBED);
        //     break;
        case(EVENT_ERROR):
            set_state(STATE_ERROR);
            break;
        default:
            LOG_ERROR("Event id %d undefined", event_id);
    }

}


int cmd_shake_event(int argc, char **argv){
    (void) argc;
    (void) argv;
    event_handler(EVENT_SHAKE, 0);
    return 0;
}

int cmd_short_press_event(int argc, char **argv){
    (void) argc;
    (void) argv;
    event_handler(EVENT_SPRESS, 0);
    return 0;
}

int cmd_long_press_event(int argc, char **argv){
    (void) argc;
    (void) argv;
    event_handler(EVENT_LPRESS, 0);
    return 0;
}

int cmd_answer_event(int argc, char **argv){
    (void) argc;
    (void) argv;
    event_handler(EVENT_FACE, 0);
    return 0;
}
