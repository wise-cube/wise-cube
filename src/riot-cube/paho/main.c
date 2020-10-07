#include "shell.h"

#include "mqtt_wrapper.h"
#include "led.h"
#include "buttons.h"
#include "mpu.h"


const shell_command_t shell_commands[] =
        {
                { "con",    "connect to MQTT broker",              cmd_con    },
                { "discon", "disconnect from the current broker",  cmd_discon },
                { "pub",    "publish something",                   cmd_pub    },
                { "sub",    "subscribe topic",                     cmd_sub    },
                { "unsub",  "unsubscribe from topic",              cmd_unsub  },
                { "led_burst", "flashes all the color",            cmd_led_burst},
                { "led_on", "set led to onecolor",                 cmd_led_on},
                { "pub_shake", "publish the shake event message",  cmd_pub_shake_event},
                { "pub_answer", "publish answer event message",    cmd_pub_answer_event},
                { "long", 	"button long press",                   cmd_pub_long_press_event},
                { "short", 	"button short press",                  cmd_pub_short_press_event},
                { "mpu_stop", 	"mpu stop",                        cmd_mpu_stop},
                { "mpu_start", 	"mpu start"	,                      cmd_mpu_start},
                { NULL,     NULL,                                 NULL        }
        };


int main(void)
{
    #ifdef MODULE_LWIP
    xtimer_sleep(1);
    #endif

    led_init();
    buttons_init();
    mpu_init();
    mqtt_init();

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);


    return 0;
}
