#ifndef CUBE_EVENTS_H
#define CUBE_EVENTS_H


typedef enum event_id_t_enum             /* Defines an enumeration type    */
{
    EVENT_LPRESS,
    EVENT_SPRESS,
    EVENT_FACE,
    EVENT_NFC,
    EVENT_SHAKE,
    EVENT_FACE_ON,
    EVENT_FACE_OFF,
    EVENT_GAME_ON,
    EVENT_GAME_OFF,
    EVENT_PAIR_REQ,
    EVENT_PAIR_OK,
    EVENT_INIT,
    EVENT_CON,
    EVENT_DISCON,
    EVENT_SUB,
    EVENT_ERROR
} event_id_t;

void event_handler(event_id_t event_id, char event_arg );
// char event_names[][24] =  { "LPRESS_EVENT","SPRESS_EVENT","FACE_EVENT","SHAKE_EVENT","FACE_ON","FACE_OFF", "GAME_ON", "GAME_OFF", "PAIR_REQ", "PAIR_OK" };

int cmd_shake_event(int argc, char **argv);
int cmd_short_press_event(int argc, char **argv);
int cmd_long_press_event(int argc, char **argv);
int cmd_answer_event(int argc, char **argv);


#endif