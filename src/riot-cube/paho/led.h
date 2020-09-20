//
// Created by di3go on 2020-09-04.
//

#ifndef RIOT_CUBE_LED_H
#define RIOT_CUBE_LED_H


#define RED 0b100
#define GREEN 0b010
#define BLUE 0b001
#define VIOLET 0b101
#define TEAL 0b011
#define YELLOW 0b110
#define WHITE 0b111
#define BLACK 0b000
#define OFF 0b000

#define LED_R_GPIO 15
#define LED_G_GPIO 12
#define LED_B_GPIO 13

void led_init(void);
void led_off(void);
void led_on(int color);
void led_blink(int color);
void led_burst(void );
int cmd_led_burst(int argc, char** argv );
int cmd_led_on(int argc, char** argv );

#endif //RIOT_CUBE_LED_H