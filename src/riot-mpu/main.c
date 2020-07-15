/*
 * Copyright (C) 2016 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v3. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 *
 * @file
 * @brief       Sample application using the low-level I2C peripheral driver
 * with SAMR21-XPRO xplained pro extension.
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 * @}
 */

#include <stdio.h>
#include <stdlib.h>

#include "xtimer.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "mpu9x50.h"


#define I2C_INTERFACE I2C_DEV(0)    /* I2C interface number */
#define mpu9x50_hw_addr_t 			/* I2C MPU address on sensor */
#define mpu9x50_comp_addr_t
#define INTERVAL      (1000U)    /* set interval to 1 seconds */


int main(void)
{
	mpu9x50_status_t conf = {0x01,0x01,0x01,0x03,0x03,1000,100,0x00,0x00,0x00};
	mpu9x50_params_t params = {I2C_INTERFACE,0x68,0x0C,1000};
	mpu9x50_t dev = {params,conf};
	mpu9x50_results_t res_g = {0};
	mpu9x50_results_t res_a = {0};
	int gyro[] = {0};
	int acc[] = {0};
    puts("Read Atmel SAMR21 Io-Xplained-Pro temperature sensor on I2C bus\n");

    /* Initialise the I2C serial interface as master */
    i2c_init(I2C_INTERFACE);

    int init= mpu9x50_init(&dev, &params);
    
    if (init == -1) {
        puts("if given I2C is not enabled in board config\n");
        return 1;
    }
    else {
        printf("mpu init on success\n");
    }
   
    for (;;){
		
		mpu9x50_results_t  acc_buf = {0};
		mpu9x50_results_t  gyr_buf = {0};
		
		for (int i = 0 ; i < 10 ; i++){
			
			int read_gyro =  mpu9x50_read_gyro(&dev, &res_g);
			if (read_gyro == -1) {
				printf("if given I2C is not enabled in board config\n");
				return 1;
			}
			else if(read_gyro == -2){
				printf(" -2 if gyro full-scale range is configured wrong\n");
				return 1;
			}
			else {
				gyr_buf.x_axis += res_g.x_axis;
				gyr_buf.y_axis += res_g.y_axis;
				gyr_buf.z_axis += res_g.z_axis;
				//printf("read gyroscope on success\n");
			}
		
		
			int read_accel =  mpu9x50_read_accel(&dev, &res_a);				
			if (read_accel == -1) {
				printf("if given I2C is not enabled in board config\n");
				return 1;
			}
			else if(read_accel == -2){
				printf(" -2 if gyro full-scale range is configured wrong\n");
				return 1;
			}
			else {
				acc_buf.x_axis += res_a.x_axis;
				acc_buf.y_axis += res_a.y_axis;
				acc_buf.z_axis += res_a.z_axis;
				//printf("read accelerometer on success\n");
				
			}		
		}
		
		acc[0] = acc_buf.x_axis/10;
		acc[1] = acc_buf.y_axis/10;
		acc[2] = acc_buf.z_axis/10;
		
		gyro[0] = gyr_buf.x_axis/10;
		gyro[1] = gyr_buf.y_axis/10;
		gyro[2] = gyr_buf.z_axis/10;
		if (acc[0] != 2000){
			printf("accelerometer: %d, %d, %d <<<<<-----\n", acc[0], acc[1], acc[2]);
			printf("gyroscope: %d, %d, %d\n", gyro[0], gyro[1], gyro[2]);
		}
		else{
			printf("accelerometer: %d, %d, %d\n", acc[0], acc[1], acc[2]);
			printf("gyroscope: %d, %d, %d\n", gyro[0], gyro[1], gyro[2]);
		}
		xtimer_usleep(INTERVAL);
		
	}
    return 0;
}
