#include "shell.h"
#include "commands.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "log.h"

enum states{ STARTING, CONNECTING, IDLE, GROUP_CREATION, ANSWERING } current_state;


void shell_init(void) {
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    // const shell_command_t shell_commands[] =
    //     {
    //             { "con",    "connect to MQTT broker",              cmd_con    },
    //             { "discon", "disconnect from the current broker",  cmd_discon },
    //             { "pub",    "publish something",                   cmd_pub    },
    //             { "sub",    "subscribe topic",                     cmd_sub    },
    //             { "unsub",  "unsubscribe from topic",              cmd_unsub  },
    //             { "led_burst", "flashes all the color",            cmd_led_burst},
    //             { "pub_shake", "publish the shake event message",  cmd_pub_shake_event},
    //             { "pub_answer", "publish answer event message",    cmd_pub_answer_event},
    //             { "long", 	"button long press",                   cmd_pub_long_press_event},
    //             { "short", 	"button short press",                  cmd_pub_short_press_event},
    //             { "mpu_stop", 	"mpu stop",                        cmd_mpu_stop},
    //             { "mpu_start", 	"mpu start"	,                      cmd_mpu_start},
    //             { "shake", 	"detect and send shake",               cmd_shake_toggle},
    //             { "face", 	"detect and send facechange"	,      cmd_face_toggle},
    //     };
    const shell_command_t shell_commands[SHELL_DEFAULT_BUFSIZE] =  { 
            { "con",    "connect to MQTT broker",              cmd_con    },
            { "discon", "disconnect from the current broker",  cmd_discon },
            { "netif", "Interface manager",              lwip_netif_config},
            { NULL, NULL, NULL}
        
         };
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
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
    shell_init();
    current_state = CONNECTING;
    mqtt_init();
    return 0;
}

