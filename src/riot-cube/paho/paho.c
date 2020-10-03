#include "paho.h"

#include "utils.h"
#include "led.h"




MQTTClient mqtt_client;
Network mqtt_network;

void _on_msg_received(MessageData *data) {
    printf("paho_mqtt_example: message received on topic"
           " %.*s: %.*s\n",
           (int)data->topicName->lenstring.len,
           data->topicName->lenstring.data, (int)data->message->payloadlen,
           (char *)data->message->payload);
}

int is_con(void) {
    return mqtt_client.isconnected;
}
int discon(void) {

    int res = MQTTDisconnect(&mqtt_client);
    if (res < 0) {
        printf("Unable to disconnect from mqtt broker\n");
    }
    else {
        printf("Disconnect successful from mqtt broker \n");
    }

    NetworkDisconnect(&mqtt_network);

    return res;
}
int con(char* ip_addr, int port){
    if ( is_con() ) {
        printf("mqtt_example: client already connected\n");
        return 0;
    }

    // Retrieve broker ip and port
    char *broker_ip = (char*)&BROKER_HOST;
    int broker_port = BROKER_PORT;
    if (ip_addr) broker_ip = ip_addr;
    if (port) broker_port = port;
    int err = 0;

    // Create mqtt last will object ( used to signal disconnection )
    MQTTPacket_willOptions lwt_data = MQTTPacket_willOptions_initializer;
    lwt_data.topicName.cstring = PUB_TOPIC;
    lwt_data.message.cstring = LWT_MSG;

    // Create mqtt client ( used to signal disconnection )
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.willFlag = 1;
    connect_data.will = lwt_data;
    connect_data.MQTTVersion = MQTT_VERSION_v311;
    connect_data.clientID.cstring = DEFAULT_MQTT_CLIENT_ID;
    connect_data.cleansession = IS_CLEAN_SESSION;
    //    connect_data.username.cstring
    //    connect_data.password
    //    connect_data.keepAliveInterval

    printf("Trying to connect to network\n");
    // Connecting to network
    err = NetworkConnect(&mqtt_network, broker_ip, broker_port);
    if (err ) {
        printf("Unable to connect to network (%d)\n", err);
        return err;
    }

    printf("Trying to connect to %s, port: %d\n",
       ip_addr, port);

     // Connecting to broker
    err = MQTTConnect(&mqtt_client, &connect_data);
    if (err < 0) {
        printf("Unable to connect to broker (%d)\n", err);
        discon();

    } else {
         printf("Connection Successful");
    }
    return err;
}
int pub(char* payload, char* topic){

    if ( !payload) {
        printf("Cannot publish, no message specified \n");
        return -1;
    }
    if ( !is_con() ) {
        printf("Cannot publish, mqtt client disconnected \n");
        return -1 ;
//        con();
    }

    enum QoS msg_qos = QOS0;
    char* msg_payload = (payload);
    char* msg_topic = (topic) ? topic :(char*)&PUB_TOPIC;

    int err;

    MQTTMessage message;
    message.qos = msg_qos;
    message.retained = IS_RETAINED_MSG;
    message.payload = msg_payload;
    message.payloadlen = strlen(message.payload);

    err = MQTTPublish(&mqtt_client, msg_topic, &message);
    if (!err){
        printf("Message (%s) has been published to topic %s"
                   "with QOS %d\n",
                   (char *)message.payload, topic, (int)message.qos);
   } else {
        printf("Unable to publish, error: %d\n", err);
   }
   return err;
}

void* mqtt_thread_handler(void* data){
    return NULL;
}



//unsigned char buf[BUF_SIZE];
//unsigned char readbuf[BUF_SIZE];
//
int mqtt_init(void){

    int err = 0;
    unsigned char* buf = malloc(BUF_SIZE);
    unsigned char* readbuf = malloc(BUF_SIZE);
//    client = malloc(sizeof(MQTTClient))
//    network = malloc(sizeof(Network))

    if (buf == NULL || readbuf == NULL){
        err = -64;
    }



    NetworkInit(&mqtt_network);
    xtimer_sleep(3);
    MQTTClientInit(&mqtt_client, &mqtt_network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE, readbuf, BUF_SIZE);

    err |= MQTTStartTask(&mqtt_client);
//    con(NULL,0);
    wlog_res("Mqtt init", err);
    return err;

}

int _cmd_discon(int argc, char **argv){
    (void)argc;
    (void)argv;

    int err = discon();
    wlog_res("cmd discon", err);
    return err;
}

int _cmd_con(int argc, char **argv){
//    printf("usage: con <string msg> [broker ip ] [broker port]\n")
//    char * ip_addr = (argc > 1) ? argv[1] : NULL;
//    int  port = (argc > 2) ? atoi(argv[2]): 0;
//    char * ip_addr = (argc > 1) ? argv[1] : NULL;
//    int  port = (argc > 2) ? atoi(argv[2]): 0;

    int err = con(NULL, 0);
    wlog_res("cmd con", err);
    return err;
}
int _cmd_pub(int argc, char **argv){
//    printf("usage: pub <string msg> [topic name] [QoS level]\n")
    if (argc <= 1) {
        return -1;
    }
    char* msg = argv[1];
    char* topic = (argc > 2) ? argv[2] : NULL;
//    enum Qos qos = (argc > 3) ? get_qos(atoi(argv[3])) : NULL;

    int err = pub(topic, msg);
    wlog_res("cmd con", err);
    return err;

}
int _cmd_sub(int argc, char **argv){
//    printf("usage: sub [topic name] [QoS level]\n", argv[0])
    char* topic = (argc > 1) ? (char*)&SUB_TOPIC : NULL;
    enum QoS qos = (argc > 2) ? QOS0 : get_qos(argv[2]);

    if (strlen(topic) > MAX_LEN_TOPIC) {
        printf("Not subscribing, topic too long %s\n", argv[1]);
        return -1;
    }

    printf("Subscribing to %s\n", topic);

    int err = MQTTSubscribe(&mqtt_client, topic, qos, _on_msg_received);
    if (err) {
        printf("Unable to subscribe to %s (%d)\n", topic, err  );
    } else {
        printf("Now subscribed to %s, QOS %d\n", topic, (int) qos);
    }
    return err;
}
int _cmd_unsub(int argc, char **argv){
    char* topic = (char*)&SUB_TOPIC;

    if (argc < 1) {
        printf("usage %s [topic name]\n", argv[0]);
        return 1;
    }
    if (argc > 1) {
        topic = argv[1];
    }

    int ret = MQTTUnsubscribe(&mqtt_client, topic);

    if (ret < 0) {
        printf("mqtt_example: Unable to unsubscribe from topic: %s\n", topic);
        _cmd_discon(0, NULL);
    }
    else {
        printf("mqtt_example: Unsubscribed from topic:%s\n", topic);
    }
    return ret;
}

int cmd_mqtt_init(int argc, char **argv){
    return mqtt_init();
}
