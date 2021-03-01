#include "shell.h"
#include "commands.h"
#include "mqtt.h"
#include "log.h"
#include <net/netif.h>
#include <net/gnrc/netif.h>
#include <net/gnrc/netapi.h>
#include <net/ipv6/addr.h>
enum states{ STARTING, CONNECTING, IDLE, GROUP_CREATION, ANSWERING } current_state;

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

void shell_init(void) {
    char shell_buf[SHELL_DEFAULT_BUFSIZE];

    shell_run(shell_commands, shell_buf, SHELL_DEFAULT_BUFSIZE);
}

void networking_init(void) {
    ipv6_addr_t addr, *addr_ptr;
    addr_ptr = &addr;

    addr_ptr = ipv6_addr_from_str(addr_ptr,"fd83:e1e3:9898:dedf::200");
    gnrc_netif_t* netif = (gnrc_netif_t*) netif_get_by_id(6);
    
    gnrc_netapi_set(netif->pid, NETOPT_IPV6_ADDR,
                        ((64 << 8U) | 0), addr_ptr,
                        sizeof(addr));
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
    networking_init();
    mqtt_init();
    shell_init();
    
    current_state = CONNECTING;
    


    return 0;
}

