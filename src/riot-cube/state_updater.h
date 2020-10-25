#ifndef STATE_UPDATER
#define STATE_UPDATER

int state_updater_init(void);
void connect(void);
void state_update(void);
void* state_updater_thread_handler(void* data);


#endif