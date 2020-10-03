//
// Created by di3go on 2020-09-04.
//
#include "stdlib.h"
#include "xtimer.h"
#include "periph/gpio.h"

#include "./led.h"




gpio_t led_r = LED_R_GPIO;
gpio_t led_g = LED_G_GPIO;
gpio_t led_b = LED_B_GPIO;


int led_init(void){
    int res = 0;
    res |= gpio_init ( led_r, GPIO_OUT );
    res |= gpio_init ( led_g, GPIO_OUT );
    res |= gpio_init ( led_b, GPIO_OUT );

    led_burst();

    return res;
}

void led_off(void){
    gpio_clear(led_r);
    gpio_clear(led_g);
    gpio_clear(led_b);
}

void led_on(int color){
    led_off();


    gpio_write(led_r, color & 0b100);
    gpio_write(led_g, color & 0b010);
    gpio_write(led_b, color & 0b001);

}
void led_blink(int color){
    led_off();

    led_on(color);
    xtimer_usleep(50000);
    led_off();
    xtimer_usleep(50000);
    led_on(color);
    xtimer_usleep(50000);
    led_off();
    xtimer_usleep(50000);
    led_on(color);
    xtimer_usleep(50000);
    led_off();


}
void led_burst(void ){
    for(int i=0; i<8; i++){
        led_on(i);
        xtimer_usleep(200000);
    }
    led_off();
}

int cmd_led_burst(int argc, char** argv ){
    led_burst();
    return 0;
}

int cmd_led_on(int argc, char** argv ){
    if (argc < 2) {
        printf("Usage led_on <led_col(0-7)>");
    }
    led_on(atoi(argv[1]));
    return 0;
}
