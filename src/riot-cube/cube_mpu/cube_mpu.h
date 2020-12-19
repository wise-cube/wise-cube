#ifndef CUBE_MPU
#define CUBE_MPU


#include <stdio.h>
#include <stdlib.h>
#include "cube_led.h"
#include "mpu9x50.h"

#define I2C_INTERFACE I2C_DEV(0)    /* I2C interface number */
#define mpu9x50_hw_addr_t 			/* I2C MPU address on sensor */
#define mpu9x50_comp_addr_t
#define INTERVAL (100000U)    /* set interval to 1 seconds */

int mpu_start(void);
void mpu_stop(void);
int mpu_init(void);

void* mpu_thread_handler(void* data);
int cmd_mpu_init(int argc, char **argv);
int cmd_mpu_start(int argc, char **argv);
int cmd_mpu_stop(int argc, char **argv);
int cmd_shake_toggle(int argc, char **argv);
int cmd_face_toggle(int argc, char **argv);

void detect_shake(float s);
int detect_face_change(float *acc);
void handle_face_change(char face);

#endif //RIOT_CUBE_PAHO_H
