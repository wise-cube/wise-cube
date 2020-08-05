/*
 * Copyright (C) 2019 Javier FILEIV <javier.fileiv@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file        main.c
 * @brief       Example using MQTT Paho package from RIOT
 *
 * @author      Javier FILEIV <javier.fileiv@gmail.com>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "xtimer.h"
#include "shell.h"
#include "thread.h"
#include "mutex.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#include "mqtt_function.h"


static unsigned char buf[BUF_SIZE];
static unsigned char readbuf[BUF_SIZE];

int main(void){
	#ifdef MODULE_LWIP
		/* let LWIP initialize */
		xtimer_sleep(1);
	#endif

    NetworkInit(&network);

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE, readbuf, BUF_SIZE);

    MQTTStartTask(&client);

    connect();
    
    new_game("3");

	new_answere("5");
	
	resume_group_req("abcde");
    
    return 0;
}
