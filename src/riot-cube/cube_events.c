#include "cube_events.h"

#ifndef LPRESS_EVENT
void long_press_event(void){
    return;
}
#endif

#ifndef SPRESS_EVENT
void short_press_event(void){
    return;
}
#endif

#ifndef FACE_EVENT
void face_change_event(int facenum){
    return;
}
#endif

#ifndef S_EVENT
void shake_event(void){
    return;
}
#endif

#ifndef NFC_EVENT
void nfc_card_event(int card_num){
    return;
}
#endif


int pub_msg(char* message){
    return 0;
}