#include "paho.h"
#include "utils.h"
#include "globals.h"

int topic_cnt = 0;
MQTTClient client;
Network network;
char _topic_to_subscribe[MAX_TOPICS][MAX_LEN_TOPIC];

unsigned char buf[BUF_SIZE];
unsigned char readbuf[BUF_SIZE];

extern char* group_id;
extern char* player_id;
extern char* token;

/*
// Paho Functions
*/
unsigned get_qos(const char *str)
{
    int qos = atoi(str);

    switch (qos) {
        case 1:     return QOS1;
        case 2:     return QOS2;
        default:    return QOS0;
    }
}

void _on_msg_received(MessageData *data)
{
    printf("paho_mqtt_example: message received on topic"
           " %.*s: %.*s\n",
           (int)data->topicName->lenstring.len,
           data->topicName->lenstring.data, (int)data->message->payloadlen,
           (char *)data->message->payload);
}

int _cmd_discon(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    topic_cnt = 0;
    int res = MQTTDisconnect(&client);
    if (res < 0) {
        printf("mqtt_example: Unable to disconnect\n");
    }
    else {
        printf("mqtt_example: Disconnect successful\n");
    }

    NetworkDisconnect(&network);
    return res;
}

int _cmd_con(int argc, char **argv)
{
    if (argc < 1) {
        printf(
                "usage: %s [brokerip addr] [port] [clientID] [user] [password] "
                "[keepalivetime]\n",
                argv[0]);
        return 1;
    }

    char *remote_ip = (char*)&BROKER_HOST;

    if (argc > 1) {
        remote_ip = argv[1];
    }

    int ret = -1;

    /* ensure client isn't connected in case of a new connection */
    if (client.isconnected) {
        printf("mqtt_example: client already connected, disconnecting it\n");
        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }

    int port = BROKER_PORT;
    if (argc > 2) {
        port = atoi(argv[2]);
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = MQTT_VERSION_v311;

    data.clientID.cstring = DEFAULT_MQTT_CLIENT_ID;
    if (argc > 3) {
        data.clientID.cstring = argv[3];
    }

    data.username.cstring = DEFAULT_MQTT_USER;
    if (argc > 4) {
        data.username.cstring = argv[4];
    }

    data.password.cstring = DEFAULT_MQTT_PWD;
    if (argc > 5) {
        data.password.cstring = argv[5];
    }

    data.keepAliveInterval = DEFAULT_KEEPALIVE_SEC;
    if (argc > 6) {
        data.keepAliveInterval = atoi(argv[6]);
    }

    data.cleansession = IS_CLEAN_SESSION;
    data.willFlag = 0;

    printf("mqtt_example: Connecting to MQTT Broker from %s %d\n",
           remote_ip, port);
    printf("mqtt_example: Trying to connect to %s , port: %d\n",
           remote_ip, port);
    ret = NetworkConnect(&network, remote_ip, port);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect\n");
        return ret;
    }

    printf("user:%s clientId:%s password:%s\n", data.username.cstring,
           data.clientID.cstring, data.password.cstring);
    ret = MQTTConnect(&client, &data);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect client %d\n", ret);
        _cmd_discon(0, NULL);
        return ret;
    }
    else {
        printf("mqtt_example: Connection successfully\n");
    }

    return (ret > 0) ? 0 : 1;
}

int _cmd_pub(int argc, char **argv)
{
    enum QoS qos = QOS0;

    if (argc < 3) {
        printf("usage: %s <topic name> <string msg> [QoS level]\n",
               argv[0]);
        return 1;
    }
    if (argc == 4) {
        qos = get_qos(argv[3]);
    }
    MQTTMessage message;
    message.qos = qos;
    message.retained = IS_RETAINED_MSG;
    message.payload = argv[2];
    message.payloadlen = strlen(message.payload);

    int rc;
    if ((rc = MQTTPublish(&client, argv[1], &message)) < 0) {
        printf("mqtt_example: Unable to publish (%d)\n", rc);
    }
    else {
        printf("mqtt_example: Message (%s) has been published to topic %s"
               "with QOS %d\n",
               (char *)message.payload, argv[1], (int)message.qos);
    }

    return rc;
}

int _cmd_sub(int argc, char **argv)
{
    enum QoS qos = QOS0;

    if (argc < 2) {
        printf("usage: %s <topic name> [QoS level]\n", argv[0]);
        return 1;
    }

    if (argc >= 3) {
        qos = get_qos(argv[2]);
    }

    if (topic_cnt > MAX_TOPICS) {
        printf("mqtt_example: Already subscribed to max %d topics,"
               "call 'unsub' command\n", topic_cnt);
        return -1;
    }

    if (strlen(argv[1]) > MAX_LEN_TOPIC) {
        printf("mqtt_example: Not subscribing, topic too long %s\n", argv[1]);
        return -1;
    }
    strncpy(_topic_to_subscribe[topic_cnt], argv[1], strlen(argv[1]));

    printf("mqtt_example: Subscribing to %s\n", _topic_to_subscribe[topic_cnt]);
    int ret = MQTTSubscribe(&client,
                            _topic_to_subscribe[topic_cnt], qos, _on_msg_received);
    if (ret < 0) {
        printf("mqtt_example: Unable to subscribe to %s (%d)\n",
               _topic_to_subscribe[topic_cnt], ret);
        _cmd_discon(0, NULL);
    }
    else {
        printf("mqtt_example: Now subscribed to %s, QOS %d\n",
               argv[1], (int) qos);
        topic_cnt++;
    }
    return ret;
}

int _cmd_unsub(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage %s <topic name>\n", argv[0]);
        return 1;
    }

    int ret = MQTTUnsubscribe(&client, argv[1]);

    if (ret < 0) {
        printf("mqtt_example: Unable to unsubscribe from topic: %s\n", argv[1]);
        _cmd_discon(0, NULL);
    }
    else {
        printf("mqtt_example: Unsubscribed from topic:%s\n", argv[1]);
        topic_cnt--;
    }
    return ret;
}

/*
// Json Functions
*/

int _json_parse(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

//int json_parse(char* json_string) {
//    int i;
//    int r;
//    jsmn_parser p;
//    jsmntok_t t[128]; /* We expect no more than 128 tokens */
//
//    jsmn_init(&p);
//    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t,
//                   sizeof(t) / sizeof(t[0]));
//    if (r < 0) {
//        printf("Failed to parse JSON: %d\n", r);
//        return 1;
//    }
//
//    /* Assume the top-level element is an object */
//    if (r < 1 || t[0].type != JSMN_OBJECT) {
//        printf("Object expected\n");
//        return 1;
//    }
//
//    /* Loop over all keys of the root object */
//    for (i = 1; i < r; i++) {
//        unsigned int length = t[i + 1].end - t[i + 1].start;
//        char val[length + 1];
//        char g[strlen(val)+1];
//        char p[strlen(val)+1];
//        memcpy(val, &JSON_STRING[t[i+1].start], length);
//        val[length] = '\0';
//
//        if (jsoneq(JSON_STRING, &t[i], "type") == 0) {
//            printf("Type: %s\n", val);
//            i++;
//        } else if (jsoneq(JSON_STRING, &t[i], "group_id") == 0) {
//            /* We may additionally check if the value is either "true" or "false" */
//            printf("Group: %s\n", val);
//
//            memcpy(g, val , strlen(val));
//            g[strlen(val)] = '\0';
//            group_id = g;
//
//            printf("Group_var: %s\n", group_id);
//
//            i++;
//        } else if (jsoneq(JSON_STRING, &t[i], "player_id") == 0) {
//            /* We may want to do strtol() here to get numeric value */
//            printf("Player: %s\n", val);
//
//            memcpy(p, val , strlen(val));
//            p[strlen(val)] = '\0';
//            player_id = p;
//
//            printf("Player_var: %s\n", player_id);
//            i++;
//        } else if (jsoneq(JSON_STRING, &t[i], "group_token") == 0) {
//            /* We may want to do strtol() here to get numeric value */
//            printf("Token: %s\n", val);
//            i++;
//        } else if (jsoneq(JSON_STRING, &t[i], "status") == 0) {
//            /* We may want to do strtol() here to get numeric value */
//            printf("gr: %s", group_id);
//            printf("Status: %s\n", val);
//
//            i++;
//        }
//    }
//    return EXIT_SUCCESS;
//}

