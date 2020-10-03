#include "mpu.h"
#include "utils.h"

#include "periph_conf.h"
#include "periph/i2c.h"

#include "board.h"
#include "mutex.h"

#include "mqtt_wrapper.h"

#include <thread.h>

char old_pos;


char mpu_thread_stack[THREAD_STACKSIZE_MAIN];
kernel_pid_t mpu_pid;
mpu9x50_t mpu_dev;
int mpu_running;

mpu9x50_results_t res_g = {0};
mpu9x50_results_t res_a = {0};
float gyro[3] = {0};
float acc[3] = {0};

static void pub_if_shake(int s){
		if (s > 130){
			puts("----> CUBE SHAKE <-----\n");
			pub_shake_event();
		}
}

static void  mpu_start(void){
    mpu_running = 1;
    thread_wakeup(mpu_pid);
}
static void  mpu_stop(void){
    mpu_running = 0;
}

int mpu_init(void){
    mpu_running = 0;
    old_pos = 0;

    mpu9x50_status_t conf = {0x01,0x01,0x01,0x03,0x03,1000,100,0x00,0x00,0x00};
    mpu9x50_params_t params = {I2C_INTERFACE,0x68,0x0C,1000};
    mpu9x50_t tmp_mpu_dev = {params,conf};
    memcpy(&mpu_dev, &tmp_mpu_dev, sizeof(mpu9x50_t));


    printf("main thread: %d\n" , mpu_pid);


    /* Initialise the I2C serial interface as master */
    i2c_init(I2C_INTERFACE);

    int init= mpu9x50_init(&mpu_dev, &params);


    if (init == -1) {
        puts("if given I2C is not enabled in board config\n");
        return 1;
    }
    else {
        puts("mpu init on success\n");
    }
    mpu_pid = thread_create( mpu_thread_stack,
                    THREAD_STACKSIZE_MAIN ,
                    THREAD_PRIORITY_MAIN - 1,
                    THREAD_CREATE_STACKTEST,
                    mpu_handler ,
                    NULL, "mpu_thread");

    return 0;
}
void* mpu_handler(void* data){
    while (1){
        //da levare se nfc sta su un altro thread e bottoni sono interrupt (se non ce serve la shell)
//        xtimer_usleep(1000 * US_PER_MS);
//		puts("in handler\n");
		mpu9x50_results_t  acc_buf = {0};
		mpu9x50_results_t  gyr_buf = {0};

        if (!mpu_running){
		    thread_sleep();
        }
		
		for (int i = 0 ; i < 10 ; i++){

			
			int read_gyro =  mpu9x50_read_gyro(&mpu_dev, &res_g);

			if (read_gyro == -1) {
				printf("I2C is not enabled in board config\n");
				return NULL;
			}
			else if(read_gyro == -2){
				printf("Gyro full-scale range is configured wrong\n");
				return NULL ;
			}
			else {
				gyr_buf.x_axis += res_g.x_axis;
				gyr_buf.y_axis += res_g.y_axis;
				gyr_buf.z_axis += res_g.z_axis;
				//printf("read gyroscope on success\n");
			}
		
		
			int read_accel =  mpu9x50_read_accel(&mpu_dev, &res_a);
			if (read_accel == -1) {
				printf("if given I2C is not enabled in board config\n");
				return NULL;
			}
			else if(read_accel == -2){
				printf(" -2 if gyro full-scale range is configured wrong\n");
				return NULL;
			}
			else {
				acc_buf.x_axis += res_a.x_axis;
				acc_buf.y_axis += res_a.y_axis;
				acc_buf.z_axis += res_a.z_axis;
				//printf("read accelerometer on success\n");
				
			}
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
		pub_if_shake(s);
		xtimer_usleep(100000);

	}
	printf("mpu handler exited")	;
//	flush(stdout);
	return NULL;
}

int position(float *acc){
    int axis = 0 ;
    char choices[] = "xXyYzZ";
    float max_acc = acc[0];

//    printf("accelerometer: %f, %f, %f\n", acc[0], acc[1], acc[2]);

    if (acc[1]*acc[1] > max_acc * max_acc) {
        max_acc = acc[1];
        axis = 1;
    }

    if (acc[2]*acc[2] > max_acc * max_acc) {
        max_acc = acc[2];
        axis = 2;
    }

    int c = 2*axis + (int)(max_acc > 0);
    return choices[c];


//
//
//	if (acc[0]>=9 && (acc[1]<=2 && acc[1]>=-2) && (acc[2]<=2 && acc[2]>=-2)){
//
//		return 'X';
//	} else if (acc[0]<=-9 && (acc[1]<=2 || acc[1]>=-2) && (acc[2]<=2 || acc[2]>=-2)){
//		//printf("accelerometer: %d, %d, %d\n", acc[0], acc[1], acc[2]);
//		return 'x';
//	}else if (acc[1]>=9 && (acc[0]<=2 && acc[0]>=-2) && (acc[2]<=2 && acc[2]>=-2)){
//		//printf("accelerometer: %d, %d, %d\n", acc[0], acc[1], acc[2]);
//		return 'Y';
//	} else if (acc[1]<=-9 && (acc[0]<=2 || acc[0]>=-2) && (acc[2]<=2 || acc[2]>=-2)){
//		//printf("accelerometer: %d, %d, %d\n", acc[0], acc[1], acc[2]);
//		return 'y';
//	}else  if (acc[2]>=9 && (acc[1]<=2 && acc[1]>=-2) && (acc[0]<=2 && acc[0]>=-2)){
//		//printf("accelerometer: %d, %d, %d\n", acc[0], acc[1], acc[2]);
//		return 'Z';
//	} else if (acc[2]<=-9 && (acc[1]<=2 || acc[1]>=-2) && (acc[0]<=2 || acc[0]>=-2)){
//		//printf("accelerometer: %d, %d, %d\n", acc[0], acc[1], acc[2]);
//		return 'z';
//	}
//	return -1;

}


		
int answer_handler(int pos){
        printf("handler fired \n");

		if (old_pos != pos){
			switch (pos){
				case 'X': 
					puts("Face X\n");
					pub_answer_event(0);
					led_on(GREEN);
					break;

				case 'x': 
					puts("Face x\n");
					pub_answer_event(2);
					led_on(RED);
					break;

				case 'Y': 
					puts("Face Y\n");
					pub_answer_event(1);
					led_on(VIOLET);
					break;

				case 'y':
					puts("Face y\n");
					pub_answer_event(3);
					led_on(BLUE);
					break;

				case 'Z':
				    puts("Face Z, NmFC\n");
				    led_on(WHITE);
				    break;

				case 'z': 
					puts("Face z, NFC/button\n");
					led_on(0);
					break;

				default: 
					break;
			}
			old_pos = pos;
			
		}
		return 0;
				
}

int cmd_mpu_init(int argc, char **argv){


    mpu_init();
    //prova_pid = thread_create(prova_thread_stack, sizeof(prova_thread_stack), THREAD_PRIORITY_MAIN - 1,
    //THREAD_CREATE_STACKTEST, prova , NULL, "prova_thread");

    return 0;
}
int cmd_mpu_start(int argc, char **argv){


    mpu_start();
    //prova_pid = thread_create(prova_thread_stack, sizeof(prova_thread_stack), THREAD_PRIORITY_MAIN - 1,
    //THREAD_CREATE_STACKTEST, prova , NULL, "prova_thread");

    return 0;
}
int cmd_mpu_stop(int argc, char **argv){


    mpu_stop();
    //prova_pid = thread_create(prova_thread_stack, sizeof(prova_thread_stack), THREAD_PRIORITY_MAIN - 1,
    //THREAD_CREATE_STACKTEST, prova , NULL, "prova_thread");

    return 0;
}
