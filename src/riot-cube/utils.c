/* utils.c
            __   .__ .__
     __ __ _/  |_ |__||  |    ______
    |  |  \\   __\|  ||  |   /  ___/
    |  |  / |  |  |  ||  |__ \___ \
    |____/  |__|  |__||____   ____  >
                                \/
    This file contains various utils
*/
#include "utils.h"

state_t current_state = STATE_UNINITIALIZED;

void wlog_res (char* action, int res){

    if (res == 0) {
        printf("[LOG] : %s ok\n", action);
    } else {
        printf("[ERR] : %s failed (%d)\n", action, res);
    }
}


void wlog(char* msg){
    printf("[LOG] : %s ok\n", msg);
}

