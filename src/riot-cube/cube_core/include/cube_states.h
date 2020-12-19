
#ifndef CUBE_STATES
#define CUBE_STATES


enum enum_state_t {
    STATE_ERROR,
    STATE_UNINITIALIZED,
    STATE_DISCONNECTED,
    STATE_CONNECTED,
    STATE_SUBSCRIBED,
    STATE_PAIR_REQ,
    STATE_PAIR_ACK,
    STATE_PAIRED,
    STATE_GAME,
    STATE_FACE
};
typedef enum enum_state_t cube_state_t;

int state_notifier_init(void);

void set_state(cube_state_t new_state);
cube_state_t get_state(void);

int cmd_get_state(int argc, char** argv );

#endif