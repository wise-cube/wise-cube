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

int group_id= 0;
int player_id= 0;


static unsigned char buf[BUF_SIZE];
static unsigned char readbuf[BUF_SIZE];

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
    printf("Running mqtt paho example. Type help for commands info\n");

    MQTTStartTask(&client);
    
    con();
    
    new_group_req();
    
    //new_player_req(group_id);
    
    //new_player_accept_event("1", "1");
    
	//resume_group_req("token");
    
    //new_game("2");
    
    //player_req("1");
    
    //new_question("3", "4");
    
    //new_answer("5", "a");
    
    return 0;
}
