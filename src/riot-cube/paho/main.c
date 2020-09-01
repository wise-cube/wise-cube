#include "paho.h"

extern MQTTClient client;
extern Network network;

extern char _topic_to_subscribe[MAX_TOPICS][MAX_LEN_TOPIC];

extern unsigned char buf[BUF_SIZE];
extern unsigned char readbuf[BUF_SIZE];

const shell_command_t shell_commands[] =
        {
                { "con",    "connect to MQTT broker",             _cmd_con    },
                { "discon", "disconnect from the current broker", _cmd_discon },
                { "pub",    "publish something",                  _cmd_pub    },
                { "sub",    "subscribe topic",                    _cmd_sub    },
                { "unsub",  "unsubscribe from topic",             _cmd_unsub  },
                { NULL,     NULL,                                 NULL        }
        };




int main(void)
{
#ifdef MODULE_LWIP
    /* let LWIP initialize */
    xtimer_sleep(1);
#endif

    NetworkInit(&network);

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE,
                   readbuf,
                   BUF_SIZE);
    printf("Running mqtt paho example. Type help for commands info\n");

    MQTTStartTask(&client);

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
