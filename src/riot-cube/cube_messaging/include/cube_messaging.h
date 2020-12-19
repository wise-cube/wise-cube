#ifndef CUBE_MESSAGING_H
#define CUBE_MESSAGING_H

#define MSG_BUF_SIZE 128

int messaging_init(void);
int con(void);
int discon(void);
int is_con(void);
int send(char* msg);
// int on_msg(char* msg);


int cmd_discon(int argc, char **argv);
int cmd_con(int argc, char **argv);
int cmd_send(int argc, char **argv);

#endif //RIOT_CUBE_CUBE_FUNCTIONS_H
