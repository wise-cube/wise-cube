#include "utils.h"

#include "paho_mqtt.h"
#include "MQTTClient.h"

void wlog_res (char* action, int res){

    if (res == 0) {
        printf("[LOG] : %s ok\n", action);
    } else {
        printf("[ERR] : %s failed (%d)\n", action, res);
    }
}


void wlog(char* msg){
    printf("[LOG] : %s ok\n", msg);
}

unsigned get_qos(const char *str)
{
    int qos = atoi(str);

    switch (qos) {
        case 1:     return QOS1;
        case 2:     return QOS2;
        default:    return QOS0;
    }
}


