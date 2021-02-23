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

#include "log.h"
#include "mqtt.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"


#define BUF_SIZE                        1024
#define MQTT_VERSION_v311               4       /* MQTT v3.1.1 version is 4 */
#define COMMAND_TIMEOUT_MS              4000
#define DEFAULT_MQTT_USER               ""
#define DEFAULT_MQTT_PWD                ""
#define DEFAULT_KEEPALIVE_SEC           10
#define MAX_LEN_TOPIC                   128
#define MAX_TOPICS                      2
#define IS_CLEAN_SESSION                1
#define IS_RETAINED_MSG                 0
#define LWT_MSG "{'cmd':'disconnected' }"

MQTTClient mqtt_client;
Network mqtt_network;
unsigned char mqtt_buffer[BUF_SIZE];
unsigned char mqtt_readbuffer[BUF_SIZE];

void on_msg_received(MessageData *data) {

        int len = (int)data->message->payloadlen;
        char *paylaod = (char *)data->message->payload;

        LOG_INFO("Received %d bytes: %s", len, paylaod);
}
int mqtt_init(void){

    int ret;
    NetworkInit(&mqtt_network);
    MQTTClientInit(&mqtt_client, &mqtt_network, COMMAND_TIMEOUT_MS, mqtt_buffer, BUF_SIZE, mqtt_readbuffer, BUF_SIZE);
    ret = MQTTStartTask(&mqtt_client) < 1;  

    if (ret < 0){
        LOG_INFO("Mqtt init successful");
    } else {
        LOG_INFO("Mqtt init successful");
    }
    con();

    return ret;
}



int con(void){
    printf("Connecting\n");
    if ( is_con() ) {
        printf("Client already connected\n");
        return 0;
    }

    int ret = 0;

    // Create mqtt last will object ( used to signal disconnection )
    MQTTPacket_willOptions lwt_data = MQTTPacket_willOptions_initializer;
    lwt_data.topicName.cstring = PUB_TOPIC;
    lwt_data.message.cstring = LWT_MSG;
    
    printf("lwt_data: %s\n",lwt_data.topicName.cstring);
    printf("lwt_msg: %s\n",lwt_data.message.cstring);
    // Create mqtt client ( used to signal disconnection )
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.willFlag = 1;
    connect_data.will = lwt_data;
    connect_data.MQTTVersion = MQTT_VERSION_v311;
    connect_data.clientID.cstring = CLIENT_ID;
    connect_data.cleansession = IS_CLEAN_SESSION;
    //    connect_data.username.cstring
    //    connect_data.password
    //    connect_data.keepAliveInterval

    LOG_INFO("Trying to connect to %s:%d \n",BROKER_HOST,BROKER_PORT);
    // Connecting to network
    ret = NetworkConnect(&mqtt_network, BROKER_HOST, BROKER_PORT);
    if (ret ) {
        LOG_ERROR("Unable to connect to reach broker (%d)\n", ret);
        return ret;
    }

     // Connecting to broker
    ret = MQTTConnect(&mqtt_client, &connect_data);
    if (ret < 0) {
        printf("Unable to connect to broker (%d)\n", ret);
    } else {
        printf("Connection Successful\n");
    }
    return ret;
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
int is_con(void){
    return mqtt_client.isconnected;
}