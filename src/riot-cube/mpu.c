/* mpu.c
         _____  ______   __ __
        /     \ \____ \ |  |  \
       |  Y Y  \|  |_> >|  |  /
       |__|_|  /|   __/ |____/
            \/ |__|
       This file contains the accelerometer code that handles the
       face selection and shake detection
*/
#include "mpu.h"
#include "utils.h"
#include "board.h"
#include "mqtt_wrapper.h"

#define SHAKE_TRESHOLD_USEC 1500 * 1000

kernel_pid_t mpu_pid;
mpu9x50_t mpu_dev;
mpu9x50_results_t acc_buf;


bool mpu_running;
bool mpu_detect_shake_running;
bool mpu_detect_face_running;
uint32_t last_shake_time;
int current_face;

int mpu_start(void){
    printf("[LOG] Mpu thread status : %d\n",thread_getstatus(mpu_pid));
    mpu_running = 1;
    return thread_wakeup(mpu_pid);
}

int mpu_init(void){

    int err;

    char* mpu_thread_stack = malloc(THREAD_STACKSIZE_MAIN);
    err = !mpu_thread_stack;
    wlog_res("Mpu thread stack allocation", err);
    mpu_pid = thread_create( mpu_thread_stack,
                    THREAD_STACKSIZE_MAIN  ,
                    12,
                    THREAD_CREATE_STACKTEST,
                    mpu_thread_handler ,
                    NULL, "mpu_thread");
    err = mpu_pid < 1;
    last_shake_time = 0 ;
    err |=  mpu_start();
    wlog_res("Mpu start",err);
    return err;
}

void  mpu_stop(void){ mpu_running = 0;}
void* mpu_thread_handler(void* data){
    mpu_running = 0;
    current_face = 'N';
    int  err;


    mpu9x50_status_t conf = {0x01,0x01,0x01,0x03,0x03,1000,100,0x00,0x00,0x00};
    mpu9x50_params_t params = {I2C_INTERFACE,0x68,0x0C,1000};
    mpu9x50_t tmp_mpu_dev = {params,conf};
    memcpy(&mpu_dev, &tmp_mpu_dev, sizeof(mpu9x50_t));

    /* Initialise the I2C serial interface as master */
    i2c_init(I2C_INTERFACE);
    err = mpu9x50_init(&mpu_dev, &params);


    if (err == -1) {
        puts("[Error] The given i2c is not enabled");
        return (void*) -1;
    }
    else if (err == -2) {
        puts("[Error] The compass did not answer correctly on the given address");
        err =0 ;
    }


    mpu9x50_set_sample_rate(&mpu_dev, 200);
    if (mpu_dev.conf.sample_rate != 200) {
        puts("[Error] The sample rate was not set correctly");
        return (void*) -1 ;
    }


    float acc[3] = {0};
    mpu9x50_results_t acc_buf = {0};
    mpu9x50_results_t res_buf = {0};


    while (1){
//        printf("[LOG] Mpu thread status : %d\n",thread_getstatus(mpu_pid));
        if (!mpu_running){
		    thread_sleep();
		    continue;
        }

        memset(&acc_buf, 0, sizeof(acc_buf));

		for (int i = 0 ; i < 10 ; i++){

//			err =  mpu9x50_read_gyro(&mpu_dev, &res_buf);
//			if(err == -2){
//				printf("Gyro full-scale range is configured wrong\n");
//				return NULL;
//			} else {
//				gyr_buf.x_axis += res_buf.x_axis;
//				gyr_buf.y_axis += res_buf.y_axis;
//				gyr_buf.z_axis += res_buf.z_axis;
//			}

        err =  mpu9x50_read_accel(&mpu_dev, &res_buf);
        if(err){
            printf("Error while mesauring accel\n");
            continue;
        } else {
            acc_buf.x_axis += res_buf.x_axis;
            acc_buf.y_axis += res_buf.y_axis;
            acc_buf.z_axis += res_buf.z_axis;
            }
		}



		acc[0] = ((float)acc_buf.x_axis)/1000;
		acc[1] = (float)acc_buf.y_axis/1000;
		acc[2] = (float)acc_buf.z_axis/1000;
//        printf("[LOG]: mpu acc: %f, %f, %f\n", acc[0], acc[1], acc[2]);

//		gyro[0] = gyr_buf.x_axis/10;
//		gyro[1] = gyr_buf.y_axis/10;
//		gyro[2] = gyr_buf.z_axis/10;
		
		float acc_sum = acc[0]*acc[0] + acc[1]*acc[1] + acc[2]*acc[2];
//		printf("x: %f, y: %f, z:%f, s: %f\n", acc[0], acc[1], acc[2], acc_sum);
		//puts("----> val: %d", s);
		if (mpu_detect_shake_running)
		    detect_shake(acc_sum);
		if (mpu_detect_face_running)
		    detect_face_change(acc);
		xtimer_usleep(100000);
	}

	printf("mpu handler exited\n");
}

void detect_shake(float acc_sum_squared){
        // printf("Acc sum %f", acc_sum_squared);
		if (acc_sum_squared > 300){

            int current_time = xtimer_now_usec();
            if (current_time - last_shake_time > SHAKE_TRESHOLD_USEC){

			puts("[EVENT] Cube shake\n");
			pub_shake_event();

            last_shake_time = current_time;
		}
        
    }
}
int detect_face_change(float *acc){
    int axis = 0 ;
    char choices[] = "xXyYzZ";
    float max_acc = acc[0];

    if (acc[1]*acc[1] > max_acc * max_acc) {
        max_acc = acc[1];
        axis = 1;
    }

    if (acc[2]*acc[2] > max_acc * max_acc) {
        max_acc = acc[2];
        axis = 2;
    }

    int c = 2*axis + (int)(max_acc > 0);

    char old_face = current_face;
    current_face = choices[c];
    if (old_face != current_face){
        handle_face_change(current_face);
        return 1;
    }
    return 0;

}

void handle_face_change(char face){
    switch (face){
        case 'X':
            puts("Face X\n");
            pub_answer_event(0);
            led_set_color(GREEN);
            break;

        case 'x':
            puts("Face x\n");
            pub_answer_event(2);
            led_set_color(RED);
            break;

        case 'Y':
            puts("Face Y\n");
            pub_answer_event(1);
            led_set_color(VIOLET);
            break;

        case 'y':
            puts("Face y\n");
            pub_answer_event(3);
            led_set_color(BLUE);
            break;

        case 'Z':
            puts("Face Z, NmFC\n");
            led_set_color(WHITE);
            break;

        case 'z':
            puts("Face z, NFC/button\n");
            led_set_color(0);
            break;

        default:
            break;

}

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
int cmd_shake_toggle(int argc, char **argv){

    mpu_detect_shake_running^=1;
    //prova_pid = thread_create(prova_thread_stack, sizeof(prova_thread_stack), THREAD_PRIORITY_MAIN - 1,
    //THREAD_CREATE_STACKTEST, prova , NULL, "prova_thread");

    return 0;
}
int cmd_face_toggle(int argc, char **argv){

    mpu_detect_face_running^=1;
    //prova_pid = thread_create(prova_thread_stack, sizeof(prova_thread_stack), THREAD_PRIORITY_MAIN - 1,
    //THREAD_CREATE_STACKTEST, prova , NULL, "prova_thread");

    return 0;
}