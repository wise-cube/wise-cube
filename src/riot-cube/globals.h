#ifndef RIOT_CUBE_GLOBALS_H
#define RIOT_CUBE_GLOBALS_H


#define SUB_TOPIC "/to_cube/1"
#define PUB_TOPIC "/from_cube/1"
#define LWT_MSG "{ \"msg_type\" : \"disconnected\" }"
#define DEFAULT_MQTT_CLIENT_ID "cube_1"

#define I2C0_SPEED     I2C_SPEED_FAST

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <thread.h>
#include "xtimer.h"
#include "shell.h"
#include "thread.h"
#include "mutex.h"


#define BUTTON_GPIO 15
#define LED_G_GPIO 14
#define LED_R_GPIO 12
#define LED_B_GPIO 13      
// #define I2C_CKL 4
// #define I2C_SDA 5
// #define SPI_SCLK 14
// #define SPI_MISO 12
// #define SPI_MOSI 13

// GPIO 9/10 create fault on boot
// GPIO 15 appears to be reserved to spi

#endif //RIOT_CUBE_GLOBALS_H


