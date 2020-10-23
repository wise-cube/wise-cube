#include "state_updater.h"
#include "led.h"
#include "paho.h"
#include "utils.h"
#include "mpu.h"
#include "buttons.h"

enum state current_state;
pid_t state_updater_pid;

void state_update_internal(void){

     switch (current_state) {
            case error:
                led_set_color(VIOLET);
                break;
            case uninitialized:
                led_set_color(RED);
                break;
            case disconnected:
                led_set_color(YELLOW);
                connect();
                break;
            case connected:
                led_set_color(GREEN);
                break;
            case paired:
                led_set_color(WHITE);
                break;
            default:
                break;
    }
}

void* state_updater_thread_handler(void* data){
    while(true) {
        state_update_internal();
        xtimer_sleep(1);
    }
}


int state_updater_init(void) {
    char * status_updater_thread_stack = malloc(THREAD_STACKSIZE_MEDIUM);
    state_updater_pid = thread_create( status_updater_thread_stack,
            THREAD_STACKSIZE_MEDIUM  ,
            3,
            THREAD_CREATE_STACKTEST,
            state_updater_thread_handler ,
            NULL, "state_updater_thread");

    int err = state_updater_pid < 0;
    wlog_res("State updater init", err);
    return err;
}


void connect(void){
    int err;
    err = con();
    if (!err){
        current_state = connected;
    } else {
        current_state = disconnected;
    }

}
