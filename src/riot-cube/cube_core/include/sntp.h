#ifndef CUBE_SNTP_H
#define CUBE_SNTP_H
#include "net/sntp.h"
#include "net/sock/udp.h"
#include "net/sock/dns.h"
#include "net/ipv6/addr.h"

#include "globals.h"
#define SNTP_STACKSIZE 2048+1024 
#define STATE_UPDATER_PRIORITY 6
int last_ntp_update;
sock_udp_ep_t sntp_server = { .port = NTP_PORT, .family = AF_INET6 };
char sntp_thread_stack[SNTP_STACKSIZE];


static void* sntp_thread(void* data) {
    (void)data;
    int err = 0;
    
    while(true){

        err = !sntp_sync(&sntp_server, 5 * US_PER_SEC ); 
        unsigned int time = xtimer_now_usec64();
        if (err){
            LOG_WARNING("sntp synch failed, time: %u\n", time);
        }  else {      
            LOG_DEBUG("sntp synch ok, time: %u\n", time);
        }
        
        xtimer_sleep(60);
    }


    return NULL;
}
        


int sntp_init(void){
    ipv6_addr_t *addr = (ipv6_addr_t *)&sntp_server.addr;
    ipv6_addr_from_str(addr, BROKER_HOST);
    unsigned int start_time = xtimer_now_usec64();
    LOG_DEBUG("time before sntp: %u\n", start_time);

    pid_t sntp_pid = thread_create( sntp_thread_stack,
        SNTP_STACKSIZE ,
        STATE_UPDATER_PRIORITY,
        0,
        sntp_thread ,
        NULL, "cube_sntp");

    int err = (int)sntp_pid < 0;
    
    
    if (!err) {
        LOG_INFO("Sntp init successful with pid: %d!\n", sntp_pid);
    } else {
        LOG_ERROR("Sntp init failed with err %d\n", sntp_pid);
        event_handler(EVENT_ERROR, 0);
    }

    return err;
}
#endif