
#ifndef CUBE_SHELL_H
#define CUBE_SHELL_H

#include "shell.h"
#include "thread.h"

#include "cube_states.h"

#define SHELL_PRIORITY 14 
#define SHELL_STACKSIZE 4096
char line_buf[SHELL_DEFAULT_BUFSIZE];
char shell_stack[SHELL_STACKSIZE] = {0};

const shell_command_t shell_commands[] = {
    { "state",    "prints current cube state",              cmd_get_state    },
    #ifdef CUBE_MESSAGING
    { "con",    "connects to server ",              cmd_con    },
    { "discon",    "disconnects from server ",              cmd_discon    },
    #endif
    { NULL,     NULL,                                 NULL        }

};
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
    //             { NULL,     NULL,                                 NULL        }

    //     };


void* shell_thread(void* data){
    (void) data;
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return NULL;
}




int shell_init(void) {
    

    int shell_pid = thread_create( shell_stack,
            SHELL_STACKSIZE,
            SHELL_PRIORITY   ,
            THREAD_CREATE_STACKTEST,
            shell_thread ,
            NULL, "cube_shell");

    int err = (int)shell_pid < 0;

    if (!err) {
        LOG_INFO("Shell init successful with pid: %d!\n", shell_pid);
    } else {
        LOG_ERROR("Shell init failed with err %d\n", shell_pid);
        event_handler(EVENT_ERROR, 0);
        return 1;
    }
    
    return 0;
}
#endif