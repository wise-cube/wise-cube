//
// Created by di3go on 2020-09-04.
//

#ifndef RIOT_CUBE_BUTTONS_H
#define RIOT_CUBE_BUTTONS_H

int buttons_init(void);

void long_press_event(void);
void short_press_event(void);

void button_interrupt_handler(void* data);
void* button_thread_handler(void* data);
void* timer_thread_handler(void* data);

#endif //RIOT_CUBE_BUTTONS_H
