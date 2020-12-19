#include <log.h>


#ifndef VIRTUAL

#include "cube_led/cube_led.h"
#include "cube_button/cube_button.h"
#include "cube_mpu/cube_mpu.h"
#include "cube_nfc/cube_nfc.h"
#endif

#include "cube_messaging.h"
#include "cube_states.h"
#include "cube_events.h"

#include "sntp.h"
#include "cube_shell.h"



void init(void){

    int err = 0;
    // err |= led_init();
    // err |= mqtt_init();
    // err |= mpu_init();
    // err |= nfc_init();
    // err |= buttons_init();
    err |= state_notifier_init();
    err |= shell_init();
    err |= sntp_init();
    event_handler(EVENT_INIT,0);
    err |= messaging_init();
     
    
    if (!err) {
        LOG_INFO("Init successful !\n");
    } else {
        LOG_ERROR("Init failed \n");
        event_handler(EVENT_ERROR, 0);
    }
    
}


int main(void)
{
    init();
    return 0;
}

