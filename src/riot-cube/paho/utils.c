#include "utils.h"


extern int status;
int is_online(void){
    int cnt=0;
//    netif_t * netif_ptr = NULL;
//    for(netif_ptr = netif_iter(NULL); netif_ptr!=NULL ;netif_ptr = netif_iter(netif_ptr)){
//        cnt ++;
//    }
    return cnt;
}

int cmd_get_status(int argc, char **argv){
    return status;
}

