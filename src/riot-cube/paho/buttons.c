//
// Created by di3go on 2020-09-04.
//
#include "periph/gpio.h"
#include "board.h"

#include "utils.h"
#include "buttons.h"
#include "led.h"
#include "mqtt_wrapper.h"


gpio_t button_ok_pin = 14;
gpio_t button_ko_pin = 0;
mutex_t buttons_mutex;
xtimer_t* release_lock_timer;


int buttons_init(void){
    int res = gpio_init_int( button_ok_pin, GPIO_OUT, GPIO_RISING, button_ok_handler, NULL);
    res |= gpio_init_int( button_ko_pin, GPIO_OUT, GPIO_RISING, button_ko_handler, NULL);
    mutex_init(&buttons_mutex);

    gpio_irq_enable(button_ko_pin);
    gpio_irq_enable(button_ok_pin);
    release_lock_timer = malloc(sizeof(xtimer_t));


//    gpio_init ( button_ko_pin, GPIO_OUT );
//    gpio_init ( button_ok_pin, GPIO_OUT);

//    gpio_clear(button_ko_pin);
//    gpio_clear(button_ok_pin);

//    printf("Buttons init: %d\n",res);
    wlog_res("Button init", res);
    return res;
}

void release_lock_handler(void* arg){
    mutex_unlock(&buttons_mutex);
//    xtimer_t* release_lock_timer = (xtimer_t* )arg;
//    free(release_lock_timer);
}
void schedule_release_lock_handler(void){
//    xtimer_t* release_lock_timer = malloc(sizeof(xtimer_t));
    memset(release_lock_timer,0,sizeof(xtimer_t));
    release_lock_timer -> callback = release_lock_handler;
    release_lock_timer-> arg = release_lock_timer;
    xtimer_set(release_lock_timer, 100000 );
}
void button_ok_handler(void* arg){
    int acquired = mutex_trylock(&buttons_mutex);
    if (acquired){
        printf("Button ok pressed\n");
        fflush(stdout);
        led_blink(GREEN);
//        pub_button_ok_event();
        schedule_release_lock_handler();

    } else {
        printf("Interrupt already running\n");
    }
}
void button_ko_handler(void* arg){
    int acquired = mutex_trylock(&buttons_mutex);
    if (acquired){
        printf("Button ko pressed\n");
        led_blink(RED);
        fflush(stdout);
//        pub_button_ko_event();
        schedule_release_lock_handler();
    } else {
        printf("Interrupt already running\n");
    }
}
