#include "mqtt_function.h"

int topic_cnt= 0;
char _topic_to_subscribe[MAX_TOPICS][MAX_LEN_TOPIC];

unsigned get_qos(const char *str) {
    int qos = atoi(str);

    switch (qos) {
    case 1:     return QOS1;
    case 2:     return QOS2;
    default:    return QOS0;
    }
}


int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

int json_conv(char* JSON_STRING) {
  int i;
  int r;
  jsmn_parser p;
  jsmntok_t t[128]; /* We expect no more than 128 tokens */

  jsmn_init(&p);
  r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t,
                 sizeof(t) / sizeof(t[0]));
  if (r < 0) {
    printf("Failed to parse JSON: %d\n", r);
    return 1;
  }

  /* Assume the top-level element is an object */
  if (r < 1 || t[0].type != JSMN_OBJECT) {
    printf("Object expected\n");
    return 1;
  }

  /* Loop over all keys of the root object */
  for (i = 1; i < r; i++) {
	unsigned int length = t[i + 1].end - t[i + 1].start;
	char val[length + 1];  
	char g[strlen(val)+1];   
	char p[strlen(val)+1];   
	memcpy(val, &JSON_STRING[t[i+1].start], length);
	val[length] = '\0';
	
    if (jsoneq(JSON_STRING, &t[i], "type") == 0) {
		printf("Type: %s\n", val);     
      i++;
    } else if (jsoneq(JSON_STRING, &t[i], "group_id") == 0) {
      /* We may additionally check if the value is either "true" or "false" */
      printf("Group: %s\n", val);
         
	  memcpy(g, val , strlen(val));
	  g[strlen(val)] = '\0';
      group_id = g;
      
      printf("Group_var: %s\n", group_id);
      
      i++;
    } else if (jsoneq(JSON_STRING, &t[i], "player_id") == 0) {
      /* We may want to do strtol() here to get numeric value */
      printf("Player: %s\n", val);
           
      memcpy(p, val , strlen(val));
	  p[strlen(val)] = '\0';
      player_id = p;
      
      printf("Player_var: %s\n", player_id);
      i++;
    } else if (jsoneq(JSON_STRING, &t[i], "group_token") == 0) {
      /* We may want to do strtol() here to get numeric value */
      printf("Token: %s\n", val);
      i++;
    } else if (jsoneq(JSON_STRING, &t[i], "status") == 0) {
      /* We may want to do strtol() here to get numeric value */
       printf("gr: %s", group_id);
      printf("Status: %s\n", val);
     
      i++;
    }
  }
  return EXIT_SUCCESS;
}

void _on_msg_received(MessageData *data) {
	
	//printf("on_message arrived triggered\n");
	char* msg= (char *)data->message->payload;
    printf("----------------->message received on topic %.*s: %.*s\n",
           (int)data->topicName->lenstring.len,
           data->topicName->lenstring.data, (int)data->message->payloadlen,
           msg);
	json_conv(msg);
}

int con(void) {

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
        discon();
        return ret;
    }
    else {
        printf("mqtt_example: Connection successfully\n");
    }

    return (ret > 0) ? 0 : 1;
}

int discon(void) {
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

int pub(char* topic, char* payload) {
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

int sub(char* topic) {
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

    //printf("mqtt_example: Subscribing to %s\n", _topic_to_subscribe[topic_cnt]);
    int ret = MQTTSubscribe(&client, _topic_to_subscribe[topic_cnt], qos, _on_msg_received);
    if (ret < 0) {
        printf("mqtt_example: Unable to subscribe to %s (%d)\n",
               _topic_to_subscribe[topic_cnt], ret);
        discon();
    }
    else {
        printf("mqtt_example: Now subscribed to %s, QOS %d\n", topic, (int) qos);
        topic_cnt++;
    }
    printf("ret = %d\n", ret);
    return ret;
}

int _cmd_unsub(int argc, char **argv) {
    if (argc < 2) {
        printf("usage %s <topic name>\n", argv[0]);
        return 1;
    }

    int ret = MQTTUnsubscribe(&client, argv[1]);

    if (ret < 0) {
        printf("mqtt_example: Unable to unsubscribe from topic: %s\n", argv[1]);
        discon();
    }
    else {
        printf("mqtt_example: Unsubscribed from topic:%s\n", argv[1]);
        topic_cnt--;
    }
    return ret;
}

//Topic: game, answere

void new_group_req(void) {
	if (! client.isconnected){
		con();
	}
	char* topic_pub="group/req";
	char* topic_sub="group/resp";
	char* payload="{ type : new_group_req }\0";
	
	pub(topic_pub, payload);
	printf("after sub %d\n", sub(topic_sub) );
	return;
}

void new_player_req(char* group_id) {
	if (! client.isconnected){
		con();
	}
	char* topic_pub="group/req/player";
	char* topic_sub="group/resp/player";
	char payload[60]= "{ type : new_player_req , group_id : ";
	char b[]= " }\0";
	
	strcat(payload, group_id);
	strcat(payload, b);
	//printf(payload);
	//printf("\n");ƒ
	pub(topic_pub, payload);
	//sub(topic_sub);
	printf("after sub %d\n", sub(topic_sub) );
}

void new_player_accept_event(char* group_id, char* player_id) {
	if (! client.isconnected){
		con();
	}
	char* topic="group/player";
	char payload[60]= "{ type : new_player_acc , group_id : ";
	char a[]=" , player_id : ";
	char b[]= " }\0";
	
	strcat(payload, group_id);
	strcat(payload, a);
	strcat(payload, player_id);
	strcat(payload, b);
	//printf(payload);
	//printf("\n");
	pub(topic, payload);
}

void resume_group_req(char* token) {
	if (! client.isconnected){
		con();
	}
	char* topic_pub="group/req";
	char* topic_sub="group/resp";
	char payload[50]= "{ type : resume_group_req , group_token : ";
	char b[]= " }\0";
	
	strcat(payload, token);
	strcat(payload, b);
//	printf(payload);
	printf("\n");
	
	pub(topic_pub, payload);
	
	sub(topic_sub);
}

void new_game(char* game_id) {
	if (! client.isconnected){
		con();
	}
	
	char* topic="game";
	char payload[40]= "{ type : new_game_event , game_id : ";
	char b[]= " }\0";
	
	strcat(payload, game_id);
	strcat(payload, b);
//	printf(payload);
	printf("\n");
	pub(topic, payload);
}

void player_req(char* group_id) {
	if (! client.isconnected){
		con();
	}
	char* topic_pub="group/req/player";
	char* topic_sub="group/resp/player";
	char payload[40]= "{ type : player_req , group_id : ";
	char b[]= " }\0";
	
	strcat(payload, group_id);
	strcat(payload, b);
//	printf(payload);
	printf("\n");
	pub(topic_pub, payload);
	sub(topic_sub);
}

void new_question(char* game_id, char* point) {
	if (! client.isconnected){
		con();
	}
	char* topic="game/question";
	char payload[50]= "{ type : new_question_event , game_id : ";
	char a[]= " , point : ";
	char b[]= " }\0";
	
	strcat(payload, game_id);
	strcat(payload, a);
	strcat(payload, point);
	strcat(payload, b);
//	printf(payload);
	printf("\n");
	pub(topic, payload);
}

void new_answer(char* answer_id, char* answer_val) {
	if (! client.isconnected){
		con();
	}
	char* topic="game/answer";
	char payload[60]= "{ type : new_answer_event , answer_id : ";
	char a[]=" , answer_val : ";
	char b[]= " }\0";
	
	strcat(payload, answer_id);
	strcat(payload, a);
	strcat(payload, answer_val);
	strcat(payload, b);
//	printf(payload);
	printf("\n");
	pub(topic, payload);
}

