#include "paho.h"



int topic_cnt = 0;
MQTTClient client;
Network network;
char _topic_to_subscribe[MAX_TOPICS][MAX_LEN_TOPIC];
int status;

unsigned char buf[BUF_SIZE];
unsigned char readbuf[BUF_SIZE];


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
    status=0;
    return res;
}

int _cmd_con(int argc, char **argv)
{
    //        typedef struct
//        {
//            /** The eyecatcher for this structure.  must be MQTC. */
//            char struct_id[4];
//            /** The version number of this structure.  Must be 0 */
//            int struct_version;
//            /** Version of MQTT to be used.  3 = 3.1 4 = 3.1.1
//              */
//            unsigned char MQTTVersion;
//            MQTTString clientID;
//            unsigned short keepAliveInterval;
//            unsigned char cleansession;
//            unsigned char willFlag;
//            MQTTPacket_willOptions will;
//            MQTTString username;
//            MQTTString password;
//        } MQTTPacket_connectData;

//    if (argc < 1) {
//        printf(
//                "usage: %s [brokerip addr] [port] [clientID] [user] [password] [keepalivetime]\n",
//                argv[0]);
//        return 1;
//    }
    /* ensure client isn't connected in case of a new connection */
    if (client.isconnected) {
        printf("mqtt_example: client already connected, disconnecting it\n");
        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }


    char *remote_ip = (char*)&BROKER_HOST;
    int port = BROKER_PORT;
    int ret = -1;


    MQTTPacket_willOptions lwt_data = MQTTPacket_willOptions_initializer;
    lwt_data.topicName.cstring= PUB_TOPIC;
    lwt_data.message.cstring = LWT_MSG;

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.willFlag = 1;
    connect_data.will = lwt_data;
    connect_data.MQTTVersion = MQTT_VERSION_v311;
    connect_data.clientID.cstring = DEFAULT_MQTT_CLIENT_ID;
    connect_data.cleansession = IS_CLEAN_SESSION;


    if (argc > 1) {
        remote_ip = argv[1];
    }

    if (argc > 2) {
        port = atoi(argv[2]);
    }

    if (argc > 3) {
        connect_data.clientID.cstring = argv[3];
    }

    connect_data.username.cstring = DEFAULT_MQTT_USER;
    if (argc > 4) {
        connect_data.username.cstring = argv[4];
    }

    connect_data.password.cstring = DEFAULT_MQTT_PWD;
    if (argc > 5) {
        connect_data.password.cstring = argv[5];
    }

    connect_data.keepAliveInterval = DEFAULT_KEEPALIVE_SEC;
    if (argc > 6) {
        connect_data.keepAliveInterval = atoi(argv[6]);
    }

    printf("mqtt_example: Connecting to MQTT Broker from %s %d\n",
           remote_ip, port);
    printf("mqtt_example: Trying to connect to %s , port: %d\n",
           remote_ip, port);
    ret = NetworkConnect(&network, remote_ip, port);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect\n");
        return ret;
    }

    printf("user:%s clientId:%s password:%s\n", connect_data.username.cstring,
           connect_data.clientID.cstring, connect_data.password.cstring);
    ret = MQTTConnect(&client, &connect_data);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect client %d\n", ret);
        _cmd_discon(0, NULL);
        return ret;
    }
    else {
        printf("mqtt_example: Connection successfully\n");
    }
    status = (ret > 0) ? 0 : 1;
    return (ret > 0) ? 0 : 1;
}

int _cmd_pub(int argc, char **argv)
{
    enum QoS qos = QOS0;
    char* topic = (char*)&PUB_TOPIC;
    printf("cmd_pub");
    if (argc < 2) {
        printf("usage: %s <string msg> [topic name] [QoS level]\n",
               argv[0]);
        return 1;
    }

    if (argc > 2) {
        topic = argv[2];
    }

    if (argc > 3) {
        qos = get_qos(argv[3]);
    }

    MQTTMessage message;
    message.qos = qos;
    message.retained = IS_RETAINED_MSG;
    message.payload = argv[1];
    message.payloadlen = strlen(message.payload);

    int rc;
    if ((rc = MQTTPublish(&client, topic, &message)) < 0) {
        printf("mqtt_example: Unable to publish (%d)\n", rc);
    }
    else {
        printf("mqtt_example: Message (%s) has been published to topic %s"
               "with QOS %d\n",
               (char *)message.payload, topic, (int)message.qos);
    }

    return rc;
}

int _cmd_sub(int argc, char **argv)
{
    enum QoS qos = QOS0;
    char* topic = (char*)&SUB_TOPIC;

    if (argc < 1) {
        printf("usage: %s [topic name] [QoS level]\n", argv[0]);
        return 1;
    }
    if (argc > 1) {
        topic = argv[1];
    }

    if (argc > 2) {
        qos = get_qos(argv[2]);
    }

    if (topic_cnt > MAX_TOPICS) {
        printf("mqtt_example: Already subscribed to max %d topics,"
               "call 'unsub' command\n", topic_cnt);
        return -1;
    }

    if (strlen(topic) > MAX_LEN_TOPIC) {
        printf("mqtt_example: Not subscribing, topic too long %s\n", argv[1]);
        return -1;
    }
    strncpy(_topic_to_subscribe[topic_cnt], topic, strlen(topic));

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
               topic, (int) qos);
        topic_cnt++;
    }
    return ret;
}

int _cmd_unsub(int argc, char **argv)
{
    char* topic = (char*)&SUB_TOPIC;

    if (argc < 1) {
        printf("usage %s [topic name]\n", argv[0]);
        return 1;
    }
    if (argc > 1) {
        topic = argv[1];
    }

    int ret = MQTTUnsubscribe(&client, topic);

    if (ret < 0) {
        printf("mqtt_example: Unable to unsubscribe from topic: %s\n", topic);
        _cmd_discon(0, NULL);
    }
    else {
        printf("mqtt_example: Unsubscribed from topic:%s\n", topic);
        topic_cnt--;
    }
    return ret;
}
