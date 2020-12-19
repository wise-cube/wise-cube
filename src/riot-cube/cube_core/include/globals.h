#ifndef CUBE_GLOBALS_H
#define CUBE_GLOBALS_H


// #define SUB_TOPIC "/to_cube/1"
// #define PUB_TOPIC "/from_cube/1"
// #define LWT_MSG "{ \"msg_type\" : \"disconnected\" }"
// #define DEFAULT_MQTT_CLIENT_ID "cube_1"

#ifndef BROKER_HOST
#define BROKER_HOST "::1"
#endif

#define I2C0_SPEED     I2C_SPEED_FAST


// values defined here override default values for GPIOS
#define BUTTON_GPIO 15
#define GPIO_LED_R 13
#define GPIO_LED_G 14
#define GPIO_LED_B 12  



// #define I2C0_SPEED     I2C_SPEED_FAST
// #define I2C0_SDA       GPIO4
// #define I2C0_SCL       GPIO5

// #define I2C_CKL 4
// #define I2C_SDA 5
// #define SPI_SCLK 14
// #define SPI_MISO 12
// #define SPI_MOSI 13

// GPIO 9/10 create fault on boot
// GPIO 15 appears to be reserved to spi

#endif //RIOT_CUBE_GLOBALS_H


