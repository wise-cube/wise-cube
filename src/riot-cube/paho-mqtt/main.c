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

MQTTClient client;
Network network;

char* group_id= "0";
char* player_id= "0";
char* token= "null";

static  char buf[BUF_SIZE];
static  char readbuf[BUF_SIZE];

int main(void)
{
#ifdef MODULE_LWIP
    /* let LWIP initialize */
    xtimer_sleep(2);
#endif

    NetworkInit(&network);

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE,
                   readbuf,
                   BUF_SIZE);

    MQTTStartTask(&client);
    
    con();
    
    new_group_req();
    
    //ok new_player_req("3");
    
    //ok new_player_accept_event("1", "1");
    
	//ok resume_group_req("token");
    
   //ok new_game("2");
    
   //ok player_req("1");
    
   //ok new_question("3", "4");
    
   //ok new_answer("5", "a");
    
    return 0;
}
