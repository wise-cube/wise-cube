#include "mpu.h"

int old_pos = '0';
mpu9x50_status_t conf = {0x01,0x01,0x01,0x03,0x03,1000,100,0x00,0x00,0x00};
mpu9x50_params_t params = {I2C_INTERFACE,0x68,0x0C,1000};

mpu9x50_results_t res_g = {0};
mpu9x50_results_t res_a = {0};
int gyro[3] = {0};
int acc[3] = {0};


int mpu_init(void){
	mpu9x50_t dev = {params,conf};
    /* Initialise the I2C serial interface as master */
    i2c_init(I2C_INTERFACE);

    int init= mpu9x50_init(&dev, &params);
    
    if (init == -1) {
        puts("if given I2C is not enabled in board config\n");
        return 1;
    }
    else {
        puts("mpu init on success\n");
    }
    mpu_handler(dev);
    return 0;
}

int mpu_handler(mpu9x50_t dev){
    for (;;){
		puts("in handler\n");
		
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
			xtimer_usleep(INTERVAL);		
		}
				
		acc[0] = acc_buf.x_axis/1000;
		acc[1] = acc_buf.y_axis/1000;
		acc[2] = acc_buf.z_axis/1000;
		
		gyro[0] = gyr_buf.x_axis/10;
		gyro[1] = gyr_buf.y_axis/10;
		gyro[2] = gyr_buf.z_axis/10;
		
		int pos = position(acc);
		answer_handler(pos);
		
		int s=acc[0]*acc[0] + acc[1]*acc[1] + acc[2]*acc[2];
		//printf("x: %d, y: %d, z:%d", acc[0], acc[1], acc[2]);
		//puts("----> val: %d", s);
		shake_handler(s);
	}
		
}

int position(int* acc){
	if (acc[0]>=9 && (acc[1]<=2 && acc[1]>=-2) && (acc[2]<=2 && acc[2]>=-2)){
		//printf("accelerometer: %d, %d, %d\n", acc[0], acc[1], acc[2]);
		return 'X';
	} else if (acc[0]<=-9 && (acc[1]<=2 || acc[1]>=-2) && (acc[2]<=2 || acc[2]>=-2)){
		//printf("accelerometer: %d, %d, %d\n", acc[0], acc[1], acc[2]);
		return 'x';
	}else if (acc[1]>=9 && (acc[0]<=2 && acc[0]>=-2) && (acc[2]<=2 && acc[2]>=-2)){
		//printf("accelerometer: %d, %d, %d\n", acc[0], acc[1], acc[2]);
		return 'Y';
	} else if (acc[1]<=-9 && (acc[0]<=2 || acc[0]>=-2) && (acc[2]<=2 || acc[2]>=-2)){
		//printf("accelerometer: %d, %d, %d\n", acc[0], acc[1], acc[2]);
		return 'y';
	}else  if (acc[2]>=9 && (acc[1]<=2 && acc[1]>=-2) && (acc[0]<=2 && acc[0]>=-2)){
		//printf("accelerometer: %d, %d, %d\n", acc[0], acc[1], acc[2]);
		return 'Z';
	} else if (acc[2]<=-9 && (acc[1]<=2 || acc[1]>=-2) && (acc[0]<=2 || acc[0]>=-2)){
		//printf("accelerometer: %d, %d, %d\n", acc[0], acc[1], acc[2]);
		return 'z';
	}
	return -1;

}

void shake_handler(int s){	
		if (s > 130){
			puts("----> CUBE SHAKE <-----\n");
			pub_shake_event();
		}
}
		
int answer_handler(int pos){
		char answer=0;
		if (old_pos != pos){
			switch (pos){
				case 'X': 
					puts("answer A\n");
					answer='A';
					pub_answer_event();
					break;
				case 'x': 
					puts("answer C\n");
					answer='C';
					pub_answer_event();
					break;
				case 'Y': 
					puts("answer B\n");
					answer='B';
					pub_answer_event();
					break;
				case 'y': 
					puts("answer D\n");
					answer='D';
					pub_answer_event();
					break;
				case 'Z': 
				case 'z': 
					puts("NFC/button\n");
					break;
				default: 
					break;
			}
			
		}
		return (int)answer;
				
}   
int cmd_mpu_init(int argc, char **argv){
    return mpu_init();
}
