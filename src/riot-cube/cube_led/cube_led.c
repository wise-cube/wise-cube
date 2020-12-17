/* led.c
   .__               .___
   |  |    ____    __| _/
   |  |  _/ __ \  / __ |
   |  |__\  ___/ / /_/ |
   |____/ \___  >\____ |
              \/      \/
   This file contains the code that handles the led
*/
#include <stdlib.h>
#include <log.h>
#include <periph/gpio.h>
#include <xtimer.h>

#include "cube_led.h"

#ifndef GPIO_LED_R
#define GPIO_LED_R 13
#endif

#ifndef GPIO_LED_G
#define GPIO_LED_G 14
#endif

#ifndef GPIO_LED_B
#define GPIO_LED_B 12  
#endif

#define BLINK_INTERVAL_USEC  100 * 1000
#define FLASH_INTERVAL_USEC  1000 * 1000
#define LONG_FLASH_INTERVAL_USEC  3 *1000 * 1000

int current_color;

int led_init(void){
    int err = 0;
    err |= gpio_init ( (gpio_t) GPIO_LED_R, GPIO_OUT );
    err |= gpio_init ( (gpio_t) GPIO_LED_G, GPIO_OUT );
    err |= gpio_init ( (gpio_t) GPIO_LED_B, GPIO_OUT );

    led_burst();
    LOG_INFO("Led init %d", err);
    return err;
}
void led_off(void){
    gpio_clear(GPIO_LED_R);
    gpio_clear(GPIO_LED_G);
    gpio_clear(GPIO_LED_B);
}
void led_on(void){
    gpio_write(GPIO_LED_R, current_color & 0b100);
    gpio_write(GPIO_LED_G, current_color & 0b010);
    gpio_write(GPIO_LED_B, current_color & 0b001);
}
void led_set_color(int color){
    if (color != current_color){
        current_color = color;
        led_off();
        led_on();
        }
}
void led_blink(int times){

    for (int i=0; i < times; i++){
        led_off();
        xtimer_usleep(BLINK_INTERVAL_USEC);
        led_on();
        xtimer_usleep(BLINK_INTERVAL_USEC);
    }
}
void led_flash_color(int color){
    int previous_color = current_color;

    led_off();
    xtimer_usleep(BLINK_INTERVAL_USEC);
    led_set_color(previous_color);
    xtimer_usleep(FLASH_INTERVAL_USEC);
    led_off();
    xtimer_usleep(BLINK_INTERVAL_USEC);


    led_set_color(previous_color);


}
void led_burst(void ){
    for(int i=0; i<8; i++){
        led_set_color(i);
        xtimer_usleep(BLINK_INTERVAL_USEC);
    }
    led_off();
}
int cmd_led_burst(int argc, char** argv ){
    led_burst();
    return 0;
    
}

