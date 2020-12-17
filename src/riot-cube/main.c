#include "shell.h"
#include "mqtt_wrapper.h"
#include "utils.h"
#include "buttons.h"
#include "mpu.h"
#include "state_updater.h"
#include "nfc_reader.h"
#include "cube_led/cube_led.h"
extern state_t current_state;


void shell_init(void) {
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    const shell_command_t shell_commands[] =
        {
                { "con",    "connect to MQTT broker",              cmd_con    },
                { "discon", "disconnect from the current broker",  cmd_discon },
                { "pub",    "publish something",                   cmd_pub    },
                { "sub",    "subscribe topic",                     cmd_sub    },
                { "unsub",  "unsubscribe from topic",              cmd_unsub  },
                { "led_burst", "flashes all the color",            cmd_led_burst},
                { "pub_shake", "publish the shake event message",  cmd_pub_shake_event},
                { "pub_answer", "publish answer event message",    cmd_pub_answer_event},
                { "long", 	"button long press",                   cmd_pub_long_press_event},
                { "short", 	"button short press",                  cmd_pub_short_press_event},
                { "mpu_stop", 	"mpu stop",                        cmd_mpu_stop},
                { "mpu_start", 	"mpu start"	,                      cmd_mpu_start},
                { "shake", 	"detect and send shake",               cmd_shake_toggle},
                { "face", 	"detect and send facechange"	,      cmd_face_toggle},
                { NULL,     NULL,                                 NULL        }

        };

    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
}




void init(void){

    int err = 0;
    err |= led_init();

    led_set_color(RED);
    err |= mqtt_init();
    // err |= mpu_init();
    err |= nfc_init();
    // err |= buttons_init();

    led_set_color(YELLOW);
    // con();
    
    // err |= state_updater_init();

     
    // state_update();
    shell_init();
}


int main(void)
{
    current_state = STATE_UNINITIALIZED;
    init();
    return 0;
}

