#include "mqtt_function.h"

unsigned get_qos(const char *str){
    int qos = atoi(str);

    switch (qos) {
    case 1:     return QOS1;
    case 2:     return QOS2;
    default:    return QOS0;
    }
}

void _on_msg_received(MessageData *data){
    printf("paho_mqtt_example: message received on topic"
           " %.*s: %.*s\n",
           (int)data->topicName->lenstring.len,
           data->topicName->lenstring.data, (int)data->message->payloadlen,
           (char *)data->message->payload);
}

int disconnect(int argc, char **argv){
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

int connect(){
    char *remote_ip = "2001:db8::2";
	int port = 1883;
	
    int ret = -1;

    /* ensure client isn't connected in case of a new connection */
    if (client.isconnected) {
        printf("mqtt_example: client already connected, disconnecting it\n");
        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }
       
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = MQTT_VERSION_v311;

    data.cleansession = IS_CLEAN_SESSION;
    data.willFlag = 0;

    printf("mqtt_example: Connecting to MQTT Broker from %s %d\n", remote_ip, port);
    printf("mqtt_example: Trying to connect to %s , port: %d\n", remote_ip, port);
    
    ret = NetworkConnect(&network, remote_ip, port);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect\n");
        return ret;
    }

    ret = MQTTConnect(&client, &data);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect client %d\n", ret);
        disconnect(0, NULL);
        return ret;
    }
    else {
        printf("mqtt_example: Connection successfully\n");
    }

    return (ret > 0) ? 0 : 1;
}

int pub(char* topic, char* payload){
    enum QoS qos = QOS0;

    MQTTMessage message;
    message.qos = qos;
    message.retained = IS_RETAINED_MSG;
    message.payload = payload;
    message.payloadlen = strlen(message.payload);

    int rc;
    if ((rc = MQTTPublish(&client, topic, &message)) < 0) {
        printf("mqtt_example: Unable to publish (%d)\n", rc);
    }
    else {
        printf("mqtt_example: Message (%s) has been published to topic %s"
               "with QOS %d\n",
               (char *)message.payload, topic , (int)message.qos);
    }

    return rc;
}

int sub(char* topic){
    enum QoS qos = QOS0;

    if (topic_cnt > MAX_TOPICS) {
        printf("mqtt_example: Already subscribed to max %d topics,"
                "call 'unsub' command\n", topic_cnt);
        return -1;
    }

    if (strlen(topic) > MAX_LEN_TOPIC) {
        printf("mqtt_example: Not subscribing, topic too long %s\n", topic);
        return -1;
    }
    strncpy(_topic_to_subscribe[topic_cnt], topic, strlen(topic));

    printf("mqtt_example: Subscribing to %s\n", _topic_to_subscribe[topic_cnt]);
    int ret = MQTTSubscribe(&client, _topic_to_subscribe[topic_cnt], qos, _on_msg_received);
    if (ret < 0) {
        printf("mqtt_example: Unable to subscribe to %s (%d)\n",
               _topic_to_subscribe[topic_cnt], ret);
        disconnect(0, NULL);
    }
    else {
        printf("mqtt_example: Now subscribed to %s, QOS %d\n", topic, (int) qos);
        topic_cnt++;
    }
    return ret;
}

int _cmd_unsub(int argc, char **argv){
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

//Topic: game, answere
 
void new_game(char* game_id){
	
	char* topic="game";
	char payload[40]= "{'type': 'new_game_event', 'game_id': ";
	char b[]= "}\0";
	
	strcat(payload, game_id);
	strcat(payload, b);
	printf(payload);
	printf("\n");
	pub(topic, payload);
}

void new_answer(char* answer_id){

	char* topic="answer";
	char payload[45]= "{'type': 'new_group_event', 'answer_id': ";
	char b[]= "}\0";
	
	strcat(payload, answer_id);
	strcat(payload, b);
	printf(payload);
	printf("\n");
	pub(topic, payload);
}

//Topic: new_group

void new_group_req(){
	char* topic_pub="group/req/new";
	char* topic_sub="group/resp/new";
	char* payload="{'type': 'new_group_req'}";
	
	pub(topic_pub, payload);
	
	sub(topic_sub);
}

void resume_group_req(char* token){
	char* topic_pub="group/req";
	char* topic_sub="group/resp";
	
	char payload[50]= "{'type': 'resume_group_req', 'group_token': ";
	char b[]= "}\0";
	
	strcat(payload, token);
	strcat(payload, b);
	printf(payload);
	printf("\n");
	
	
	pub(topic_pub, payload);
	
	sub(topic_sub);
}
