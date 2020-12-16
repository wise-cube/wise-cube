#ifdef USE_MQTT

/* paho.c
                 .__
  ______ _____   |  |__    ____
   \____ \\__  \  |  |  \  /  _ \
   |  |_> >/ __ \_|   Y  \(  <_> )
   |   __/(____  /|___|  / \____/
   |__|        \/      \/
    paho is the package used to enable mqtt support in riot-os
    available since release 2020.7
*/
#include "mqtt.h"
#include "utils.h"
#include "led.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define BROKER_PORT 1884

MQTTClient mqtt_client;
Network mqtt_network;

void on_msg_received(MessageData *data) {
    printf("paho_mqtt_example: message received on topic"
           " %.*s: %.*s\n",
           (int)data->topicName->lenstring.len,
           data->topicName->lenstring.data, (int)data->message->payloadlen,
           (char *)data->message->payload);
}
int mqtt_init(void){

    int err = 0;
    unsigned char* buf = malloc(BUF_SIZE);
    unsigned char* readbuf = malloc(BUF_SIZE);
//    client = malloc(sizeof(MQTTClient))
//    network = malloc(sizeof(Network))

    NetworkInit(&mqtt_network);
    MQTTClientInit(&mqtt_client, &mqtt_network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE, readbuf, BUF_SIZE);

    err = MQTTStartTask(&mqtt_client) < 1;  
    

//    con(NULL,0);
    wlog_res("Mqtt init", err);
    return err;
}



int con(void){
    if ( is_con() ) {
        printf("mqtt_example: client already connected\n");
        return 0;
    }

    // Retrieve broker ip and port
    char broker_ip[] = BROKER_HOST;
    int broker_port = BROKER_PORT;
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
    printf("Trying to connect to %s:%d \n",broker_ip,broker_port);
    // Connecting to network
    err = NetworkConnect(&mqtt_network, broker_ip, broker_port);
    if (err) {
        printf("Unable to connect to network (%d)\n", err);
        return err;
    }

     // Connecting to broker
    err = MQTTConnect(&mqtt_client, &connect_data);
    if (err < 0) {
        printf("Unable to connect to broker (%d)\n", err);
        // discon();
    } else {
         printf("LOG]Connection Successful\n");
    }
    return err;
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
int pub(char* payload, char* topic){

    if ( !payload) {
        printf("Cannot publish, no message payload specified \n");
        return -1;
    }
    if ( !is_con() ) {
        printf("Cannot publish, mqtt client disconnected \n");
        return -1 ;
        // int res = con();
        // if (res){
        //     printf("Cannot publish, mqtt client disconnected \n");
        //     return -1;
        //     }
    }
    char* msg_payload = (payload);
    char* msg_topic = (topic) ? topic :(char*)&PUB_TOPIC;

    int err;

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = IS_RETAINED_MSG;
    message.payload = msg_payload;
    message.payloadlen = strlen(message.payload);

    err = MQTTPublish(&mqtt_client, msg_topic, &message);
    if (!err){
        printf("Message \"%s\" has been published to topic %s "
                   "with QOS %d\n",
                   (char *)message.payload, msg_topic, (int)message.qos);
   } else {
        printf("Unable to publish, error: %d\n", err);
   }
   return err;
}
int sub(char* topic){
    if ( !is_con() ) {
        printf("Cannot publish, mqtt client disconnected \n");
        return -1;
    }
    char* sub_topic = (topic) ? topic :(char*)&SUB_TOPIC;

    printf("Subscribing to %s\n", sub_topic);

    if (strlen(sub_topic) > MAX_LEN_TOPIC) {
        printf("Not subscribing, topic too long %s\n", sub_topic);
        return -1;
    }
    int err = MQTTSubscribe(&mqtt_client, sub_topic, QOS0, on_msg_received);
    if (err) {
        printf("Unable to subscribe to %s (%d)\n", sub_topic, err  );
    } else {
        printf("Now subscribed to %s, QOS0\n", sub_topic);
    }
    return err;
}
int unsub(char* topic){
    if ( !is_con() ) {
        printf("Cannot publish, mqtt client disconnected \n");
        return -1;
    }
    char* sub_topic = (topic) ? topic :(char*)&SUB_TOPIC;

    printf("Unsubscribing from %s\n", topic);

    if (strlen(sub_topic) > MAX_LEN_TOPIC) {
        printf("Not unsubscribing, topic too long %s\n", sub_topic);
        return -1;
    }
    int err = MQTTUnsubscribe(&mqtt_client, sub_topic);
    if (err) {
        printf("Unable to unsubscribe to %s (%d)\n", topic, err  );
    } else {
        printf("Now unsubscribed from %s\n", topic);
    }
    return err;
}
int is_con(void){return mqtt_client.isconnected;}
#endif