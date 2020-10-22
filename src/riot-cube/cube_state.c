#include "cube_state.h"
#include "led.h"
#include "paho.h"
#include "utils.h"
#include "mpu.h"
#include "buttons.h"

enum state current_state;

void update_state(void){
     switch (current_state) {
            case error:
                led_set_color(VIOLET);
                break;
            case uninitialized:
                init();
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
            xtimer_sleep(1);
        }

void init(void){

    int err;
    err = led_init();
    err = buttons_init();
    err = mpu_init();
    err = mqtt_init();

    if (!err){
        current_state = disconnected;
    }else{
        current_state = error;
    }

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
