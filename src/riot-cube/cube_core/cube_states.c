
#include "net/sntp.h"
// #include "cube_led/cube_led.h"
// #include "mqtt.h"
// #include "utils.h"
// #include "cube_mpu/cube_mpu.h"
// #include "buttons.h"
#include "log.h"
#include "thread.h"
#include "xtimer.h"
#include "stdlib.h"

#include "cube_states.h"
#include "cube_messaging.h"
#include "globals.h"


#define STATE_NAME_BUF_SIZE 24
#define STATE_NOTIFIER_STACKSIZE 4096 
#define STATE_NOTIFIER_PRIORITY 13 



cube_state_t current_state;
int state_updater_pid;
char state_names[][STATE_NAME_BUF_SIZE] =  {"STATE_ERROR", "STATE_UNINITIALIZED", "STATE_DISCONNECTED", "STATE_CONNECTED", "STATE_SUBSCRIBED", "STATE_PAIR_REQ", "STATE_PAIR_ACK", "STATE_PAIRED", "STATE_GAME", "STATE_FACE" };
char state_handler_thread_stack[STATE_NOTIFIER_STACKSIZE] = {0};
  

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

enum enum_state_t get_state(void){
    return current_state;
}

void get_state_string(char* state_name_buf){
    cube_state_t state_int = get_state();
    char state_name_buf_tmp[STATE_NAME_BUF_SIZE] = {0};

    memcpy((char*)state_name_buf_tmp, (char*)state_names[state_int], STATE_NAME_BUF_SIZE);
    switch (state_int)
    {
    case STATE_UNINITIALIZED:
        snprintf(state_name_buf, 
                STATE_NAME_BUF_SIZE + 10, 
                "%s%s%s",
                ANSI_COLOR_RED, (char*)state_name_buf_tmp , ANSI_COLOR_RESET);
        break;
    case STATE_ERROR:
        snprintf(state_name_buf, 
                STATE_NAME_BUF_SIZE + 10, 
                "%s%s%s",
                ANSI_COLOR_MAGENTA, (char*)state_name_buf_tmp , ANSI_COLOR_RESET);
    break;
    case STATE_DISCONNECTED:
        snprintf(state_name_buf, 
                STATE_NAME_BUF_SIZE + 10, 
                "%s%s%s",
                ANSI_COLOR_YELLOW, (char*)state_name_buf_tmp , ANSI_COLOR_RESET);
    break;
    case STATE_SUBSCRIBED:
    case STATE_CONNECTED:
    snprintf(state_name_buf, 
            STATE_NAME_BUF_SIZE + 10, 
            "%s%s%s",
            ANSI_COLOR_GREEN, (char*)state_name_buf_tmp , ANSI_COLOR_RESET);
    break;
    default:
        snprintf(state_name_buf, 
                STATE_NAME_BUF_SIZE , 
                "%s",
                state_name_buf_tmp);
    }
}

static void print_state(void){
    char state_buf[STATE_NAME_BUF_SIZE + 10] = {0};
    get_state_string(state_buf);
    printf("Current state: %s\n", state_buf);
}

void set_state(enum enum_state_t new_state){
    current_state = new_state;
    thread_wakeup(state_updater_pid);
    print_state();
}

int pub_state(void){
    
    char         msg_buf[MSG_BUF_SIZE];
    const char  msg_template_buf[]    ="{\"msg_type\":\"%s\",\"ts\": %llu, \"state\":%d}";
    long long unsigned int ts = sntp_get_unix_usec();
    snprintf(msg_buf, MSG_BUF_SIZE-1, msg_template_buf, "state_update", (ts ) ,current_state);
    int err =  send(msg_buf);

    return err;
}

void  state_update_internal(void){
    // print_state();
    switch (current_state) {
            case STATE_ERROR:
                #ifdef CUBE_LED
                led_set_color(VIOLET);
                #endif
                break;
            case STATE_UNINITIALIZED:
                #ifdef CUBE_LED
                led_set_color(RED);
                #endif
                break;
            case STATE_DISCONNECTED:
                #ifdef CUBE_LED
                led_set_color(YELLOW);
                #endif
                break;
            case STATE_CONNECTED:
                #ifdef CUBE_LED
                led_set_color(GREEN);
                #endif
                break;
            case STATE_PAIRED:
                #ifdef CUBE_LED
                led_set_color(WHITE);
                #endif
                break;
            default:
                break;
    }
}

void* state_handler_thread(void* data){
    // xtimer_ticks32_t last_wakeup = xtimer_now();
    (void) data;
    LOG_DEBUG("state thread started\n");
    while(true) {
        LOG_DEBUG("state thread awake\n");
        xtimer_sleep(3);
        state_update_internal();
        #ifdef CUBE_MESSAGING
        if (get_state() < STATE_CONNECTED){
            LOG_INFO("Reconnecting\n");
            con();
        } else {
            pub_state();
        }
        #endif
        // xtimer_periodic_wakeup(&last_wakeup,STATE_UPDATER_FREQ_USEC);
    }
    return 0;
}

int state_notifier_init(void) {
    current_state = STATE_UNINITIALIZED;
    print_state();
    // Use sntp to sync with backend, i supposed it was automagic, well, it isn't
    int err = 0;


    state_updater_pid = thread_create( state_handler_thread_stack,
            STATE_NOTIFIER_STACKSIZE ,
            STATE_NOTIFIER_PRIORITY,
            0,
            state_handler_thread ,
            NULL, "cube_state_notifier");

    err = (int)state_updater_pid < 0;
    
    
    if (!err) {
        LOG_INFO("State updater init successful with pid: %d!\n", state_updater_pid);
    } else {
        LOG_ERROR("State updater init failed with err %d\n", state_updater_pid);
        current_state = STATE_ERROR;
    }

    // thread_wakeup(state_updater_pid);
    return err;
}

int cmd_get_state(int argc, char** argv ){
    (void) argc;
    (void) argv;
    print_state();
    return 0;
}