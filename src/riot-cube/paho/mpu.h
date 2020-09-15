#ifndef RIOT_MPU_H
#define RIOT_MPU_H

#include <stdio.h>
#include <stdlib.h>

#include "xtimer.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "mpu9x50.h"

#include "board.h"
#include "mutex.h"
#include "cube_functions.h"

#define I2C_INTERFACE I2C_DEV(0)    /* I2C interface number */
#define mpu9x50_hw_addr_t 			/* I2C MPU address on sensor */
#define mpu9x50_comp_addr_t
#define INTERVAL (100000U)    /* set interval to 1 seconds */

int mpu_init(void);
void mpu_handler(mpu9x50_t);
void shake_handler(int);
void answer_handler(int);

#endif //RIOT_CUBE_PAHO_H
