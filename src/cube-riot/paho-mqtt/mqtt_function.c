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

int con(){

    char *remote_ip = "fe80::fa8a:3207:eaae:d9ad";

    int ret = -1;

    /* ensure client isn't connected in case of a new connection */
    if (client.isconnected) {
        printf("mqtt_example: client already connected, disconnecting it\n");
        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }

    int port = 1884;
   

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = MQTT_VERSION_v311;

    data.clientID.cstring = DEFAULT_MQTT_CLIENT_ID;
    

    data.username.cstring = DEFAULT_MQTT_USER;
   
    data.password.cstring = DEFAULT_MQTT_PWD;
    

    data.keepAliveInterval = DEFAULT_KEEPALIVE_SEC;
    
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
        discon(0, NULL);
        return ret;
    }
    else {
        printf("mqtt_example: Connection successfully\n");
    }

    return (ret > 0) ? 0 : 1;
}

int discon(){
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
        discon(0, NULL);
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

void new_group_req(){
	char* topic="group";
	char* payload='{'type': 'new_group_req'}';
	
	pub(topic, payload);
	sub(topic);
}

void new_player_req(char* group_id){
	char* topic="group/player";
	char payload[50]= "{'type': 'new_player_req', 'group_id': ";
	char b[]= "}\0";
	
	strcat(payload, group_id);
	strcat(payload, b);
	printf(payload);
	printf("\n");
	pub(topic, payload);
	sub(topic);
}

void new_player_accept_event(char* group_id, char* player_id){
	char* topic="group/player";
	char payload[60]= "{'type': 'new_player_acc', 'group_id': ";
	char a[]=", player_id: ";
	char b[]= "}\0";
	
	strcat(payload, group_id);
	strcat(payload, a);
	strcat(payload, player_id);
	strcat(payload, b);
	printf(payload);
	printf("\n");
	pub(topic, payload);
}

void resume_group_req(char* token){
	char* topic="group/req";
	char payload[50]= "{'type': 'resume_group_req', 'group_token': ";
	char b[]= "}\0";
	
	strcat(payload, token);
	strcat(payload, b);
	printf(payload);
	printf("\n");
	
	pub(topic, payload);
	
	sub(topic);
}

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

void player_req(char* group_id){
	char* topic="group/player";
	char payload[40]= "{'type': 'player_req', 'group_id': ";
	char b[]= "}\0";
	
	strcat(payload, group_id);
	strcat(payload, b);
	printf(payload);
	printf("\n");
	pub(topic, payload);
	sub(topic);
}

void new_question(char* game_id, char* point){
	char* topic="game/question";
	char payload[50]= "{'type': 'new_question', 'game_id': ";
	char a[]= ", point: ";
	char b[]= "}\0";
	
	strcat(payload, game_id);
	strcat(payload, a);
	strcat(payload, point);
	strcat(payload, b);
	printf(payload);
	printf("\n");
	pub(topic, payload);
}

void new_answer(char* answer_id, char* answer_val){
	char* topic="game/answer";
	char payload[60]= "{'type': 'new_group_event', 'answer_id': ";
	char a[]=", 'answer_val: '";
	char b[]= "}\0";
	
	strcat(payload, answer_id);
	strcat(payload, a);
	strcat(payload, answer_val);
	strcat(payload, b);
	printf(payload);
	printf("\n");
	pub(topic, payload);
}

