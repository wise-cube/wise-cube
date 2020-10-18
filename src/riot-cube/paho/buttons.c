//
// Created by di3go on 2020-09-04.
//
#include "periph/gpio.h"
#include "board.h"

#include "utils.h"
#include "buttons.h"
#include "led.h"
#include "mqtt_wrapper.h"


#define BUTTON_GPIO 2


xtimer_t* button_press_timer;
kernel_pid_t  button_thread_pid;
kernel_pid_t  timer_thread_pid;

// threshold to distinguish long/short press
uint64_t long_press_thresh = (1000 * 1000);
// if 0 button has not been pressed or is invalid
uint64_t button_down_time = 0;


int buttons_init(void){
    int res = 0;
    res |= gpio_init_int( BUTTON_GPIO, GPIO_OUT, GPIO_BOTH, button_interrupt_handler, NULL);

    gpio_irq_enable(BUTTON_GPIO);

    button_press_timer = malloc(sizeof(xtimer_t));

    char* button_thread_stack = malloc(THREAD_STACKSIZE_MAIN);
    char* timer_thread_stack = malloc(THREAD_STACKSIZE_MAIN);

    button_thread_pid = thread_create( button_thread_stack,
                    THREAD_STACKSIZE_MAIN  ,
                    20,
                    THREAD_CREATE_STACKTEST,
                    button_thread_handler ,
                    NULL, "button_thread");
    timer_thread_pid = thread_create( timer_thread_stack,
                THREAD_STACKSIZE_MAIN  ,
                13,
                THREAD_CREATE_STACKTEST,
                timer_thread_handler ,
                NULL, "timer_thread");


    res |= timer_thread_pid < 1;
    res |= button_thread_pid < 1;
    wlog_res("Button init", res);
    return res;
}

void long_press_event(void){
    pub_long_press_event();
    led_blink(GREEN);
}
void short_press_event(void){
    pub_short_press_event();
    led_flash(GREEN);
}

void button_interrupt_handler(void* data){
    gpio_irq_disable(BUTTON_GPIO	)	;
    int res = thread_wakeup(button_thread_pid);

//    wlog_res("Wake up interrupt thread", !res);
    (void)res;
    gpio_irq_enable(BUTTON_GPIO)	;

}
void* button_thread_handler(void* data){
    while(1){
        thread_sleep();
        if (!button_down_time){
            // Case button press
            printf("[EVENT] button down\n");
            button_down_time = xtimer_now_usec();
            xtimer_set_wakeup (button_press_timer, long_press_thresh, timer_thread_pid);
        } else {
            // Case button release
            printf("[EVENT] button up\n");
            //printf("Timer time left: %lld\n", xtimer_left_usec(button_press_timer) );
            if (xtimer_left_usec(button_press_timer) > 0){
                // Case timer not expired yet
                xtimer_remove(	button_press_timer);
                short_press_event();
            }
            button_down_time = 0;
        }

    }
    return NULL;

}
void* timer_thread_handler(void* data){
        while(1){
            thread_sleep ();
            printf("[EVENT] button timer\n");
            long_press_event();

    }
}

