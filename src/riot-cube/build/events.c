#include <stdio.h>

void shake_event(void){
    printf("shake_event");
}
void answer_event(void){
    printf("answer_event");
}
void connected_event(void){
    printf("connected_event");
}
void disconnected_event(void){
    printf("disconnected_event");
}

void incoming_msg_event(char* payload, int len){
    printf("incoming_msg_event");
    printf("\tlen    : %d", len);
    printf("\tpayload: %s", payload);
}