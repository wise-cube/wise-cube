#include "state_updater.h"
#include "led.h"
#include "mqtt.h"
#include "utils.h"
#include "mpu.h"
#include "buttons.h"

// #define STATE_UPDATER_FREQ_USEC _xtimer_ticks_from_usec(1000000)

extern state_t current_state;
pid_t state_updater_pid;

void set_state(state_t new_state){
    current_state = new_state;
    state_update();
}

void state_update(void){
    thread_wakeup(state_updater_pid);
}

void state_update_internal(void){
    printf("Current state: %d\n", current_state);
     switch (current_state) {
            case STATE_ERROR:
                led_set_color(VIOLET);
                break;
            case STATE_UNINITIALIZED:
                led_set_color(RED);
                break;
            case STATE_INITIALIZED:
                led_set_color(YELLOW);
                break;
            case STATE_CONNECTED:
                led_set_color(GREEN);
                break;
            case STATE_PAIRED:
                led_set_color(WHITE);
                break;
            default:
                break;
    }
}

void* state_updater_thread_handler(void* data){
    // xtimer_ticks32_t last_wakeup = xtimer_now();
    while(true) {
        state_update_internal();
        xtimer_sleep(1);
        // xtimer_periodic_wakeup(&last_wakeup,STATE_UPDATER_FREQ_USEC);
    }
    
}


int state_updater_init(void) {
    char * status_updater_thread_stack = malloc(THREAD_STACKSIZE_MEDIUM);
    state_updater_pid = thread_create( status_updater_thread_stack,
            THREAD_STACKSIZE_MEDIUM  ,
            11,
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
        current_state = STATE_CONNECTED;
    } else {
        current_state = STATE_INITIALIZED;
    }

}
void disconnect(void){
    // int err;
    discon();
    current_state = STATE_INITIALIZED;
}

