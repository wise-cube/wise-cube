#include "shell.h"
#include "commands.h"
#include "mqtt.h"
#include "log.h"

enum states{ STARTING, CONNECTING, IDLE, GROUP_CREATION, ANSWERING } current_state;


void shell_init(void) {
    char shell_buf[SHELL_DEFAULT_BUFSIZE];

    const shell_command_t shell_commands[SHELL_DEFAULT_BUFSIZE] =  { 
            { "con",    "connect to MQTT broker",              cmd_con    },
            { "discon", "disconnect from the current broker",  cmd_discon },
            { "connect", "connect to MQTT-SN gateway", cmd_connect },
            { "disconnect", "disconnect from MQTT-SN gateway", cmd_disconnect },
            { "reg", "register a given topic", cmd_reg },
            { "unreg", "remove a topic registration [locally]", cmd_unreg },
            { "pub", "publish data", cmd_pub },
            { "sub", "subscribe to topic", cmd_sub },
            { "unsub", "unsubscribe from topic", cmd_unsub },
            { "info", "print state information", cmd_info },
            { NULL, NULL, NULL}
        
         };
    shell_run(shell_commands, shell_buf, SHELL_DEFAULT_BUFSIZE);
}

void print_info(void){

    LOG_INFO("BOARD: %s\n", BOARD);

    #ifdef I2C_NUMOF
    LOG_INFO("I2Cs: %d\n", I2C_NUMOF);
    #else
    LOG_INFO("NO I2C AVAILABLE\n");
    #define I2C_NUMOF 0
    #endif

    #ifdef ADC_NUMOF
    LOG_INFO("ADCs: %d\n", ADC_NUMOF);
    #else
    LOG_INFO("NO ADC AVAILABLE\n");
    #define ADC_NUMOF 0
    #endif

    #ifdef DAC_NUMOF
    LOG_INFO("DACs: %d\n", DAC_NUMOF);
    #else
    LOG_INFO("NO DAC AVAILABLE\n");
    #define DAC_NUMOF 0
    #endif
    
    LOG_INFO("BROKER:    [%s]:%d\n", BROKER_HOST, BROKER_PORT);
    LOG_INFO("PUB TOPIC: %s\n", PUB_TOPIC);
    LOG_INFO("SUB TOPIC: %s\n", SUB_TOPIC);
}

int main(void)
{
    current_state = STARTING;
    print_info();
    mqtt_init();
    shell_init();
    current_state = CONNECTING;
    
    return 0;
}

