/* led.c
   .__               .___
   |  |    ____    __| _/
   |  |  _/ __ \  / __ |
   |  |__\  ___/ / /_/ |
   |____/ \___  >\____ |
              \/      \/
   This file contains the code that handles the led
*/
#include "led.h"

#include "stdlib.h"
#include "utils.h"
#include "periph/gpio.h"


gpio_t led_r = LED_R_GPIO;
gpio_t led_g = LED_G_GPIO;
gpio_t led_b = LED_B_GPIO;

#define BLINK_INTERVAL_USEC  100 * 1000
#define FLASH_INTERVAL_USEC  1000 * 1000
#define LONG_FLASH_INTERVAL_USEC  3 *1000 * 1000

int current_color;
int led_init(void){
    int err = 0;
    err |= gpio_init ( led_r, GPIO_OUT );
    err |= gpio_init ( led_g, GPIO_OUT );
    err |= gpio_init ( led_b, GPIO_OUT );

    led_burst();
    wlog_res("Led init", err);
    return err;
}
void led_off(void){
    gpio_clear(led_r);
    gpio_clear(led_g);
    gpio_clear(led_b);
}
void led_on(void){
    gpio_write(led_r, current_color & 0b100);
    gpio_write(led_g, current_color & 0b010);
    gpio_write(led_b, current_color & 0b001);
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

