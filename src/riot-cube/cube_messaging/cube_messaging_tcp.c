#include <stdio.h>
#include <log.h>

#include "net/sock/udp.h"
#include "cube_messaging.h"
uint8_t buf[128];
int messaging_init(void)
{
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_t sock;
    local.port = 12345;
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP sock");
        return 1;
    }
    while (1) {
        sock_udp_ep_t remote;
        ssize_t res;
        if ((res = sock_udp_recv(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT,
                                 &remote)) >= 0) {
            puts("Received a message");
            if (sock_udp_send(&sock, buf, res, &remote) < 0) {
                puts("Error sending reply");
            }
        }
        if ((res = sock_udp_recv(&sock, buf, sizeof(buf), 3 * 1000000,
                                 &remote)) >= 0) {
            puts("Received a message");
            if (sock_udp_send(&sock, buf, res, &remote) < 0) {
                puts("Error sending reply");
            }
        }
    }
    return 0;
}