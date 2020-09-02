#ifndef RIOT_CUBE_GLOBALS_H
#define RIOT_CUBE_GLOBALS_H

#endif //RIOT_CUBE_GLOBALS_H

char* group_id;
char* player_id;
char* token;

#define BROKER_HOST "fe80::1ac0:4dff:fe27:2698"
#define BROKER_PORT 1884

#define SUB_TOPIC "/to_cube/1"
#define PUB_TOPIC "/from_cubes"

#define LWT_MSG "{msg_type:'disconnected'}"
#define DEFAULT_MQTT_CLIENT_ID "cube_1"
