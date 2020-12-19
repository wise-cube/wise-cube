#include "stdio.h"
#include "cube_messaging.h"


int cmd_discon(int argc, char **argv){
    (void)argc;
    (void)argv;

    int err = discon();
    printf("cmd discon %d", err);
    return err;
}
int cmd_con(int argc, char **argv){
    (void)argc;
    (void)argv;
    return con();
}
int cmd_send(int argc, char **argv){
    (void)argc;
    (void)argv;

    if (argc <= 1) {
        printf("usage: send <string msg> [topic name] \n");
        return -1;
    }
    char* msg = argv[1];

    return send(msg);
}
