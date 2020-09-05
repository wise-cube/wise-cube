#include "cube_functions.h"
#include "led.h"
#include "buttons.h"

char line_buf[SHELL_DEFAULT_BUFSIZE];
const shell_command_t shell_commands[] =
        {
                { "con",    "connect to MQTT broker",             _cmd_con    },
                { "discon", "disconnect from the current broker", _cmd_discon },
                { "pub",    "publish something",                  _cmd_pub    },
                { "sub",    "subscribe topic",                    _cmd_sub    },
                { "unsub",  "unsubscribe from topic",             _cmd_unsub  },
                { "led_burst", "flashes all the color",          cmd_led_burst},
                { "get_status",  "show status ",                cmd_get_status },
                { "pub_shake", "publish the shake event message", cmd_pub_shake_event},
                { "pub_button_ok", "publish the button Ok event message", cmd_pub_button_ok_event},
                { "pub_button_ko", "publish the button Ko event message", cmd_pub_button_ko_event},
                { NULL,     NULL,                                 NULL        }
        };

extern int status;

int main(void)
{
    cube_init();
    led_init();
    buttons_init();
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);


    return 0;
}
