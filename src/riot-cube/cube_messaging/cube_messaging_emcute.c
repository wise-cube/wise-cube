
#ifdef EMCUTE

#include <log.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#include "cube_events.h"
#include "cube_states.h"


#define SUB_TOPIC "/to_cube/1"
#define PUB_TOPIC "/from_cube/1"
#define LWT_MSG "{ \"msg_type\" : \"disconnected\" }"
// #define DEFAULT_MQTT_CLIENT_ID "cube_1"
#define EMCUTE_ID "cube_1"

#define EMCUTE_PORT         (1886U)
#define EMCUTE_PRIO         (12)

#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];

emcute_topic_t sub_topic = { .name = SUB_TOPIC};
emcute_topic_t pub_topic = { .name = PUB_TOPIC};
emcute_sub_t subscription;

pid_t mqtt_thread_pid;

static void *emcute_thread(void *arg)
{
    char emcute_id_buf[16] = {0};
    snprintf(emcute_id_buf, 16, "%s_%d", EMCUTE_ID, rand() & 0xffffffff);
    (void)arg;
    emcute_run(EMCUTE_PORT, emcute_id_buf);
    return NULL;    /* should never be reached */
}

static void on_msg(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    char event_id_buf[4];
    char event_arg_buf[4]; 
    strncpy(event_id_buf, in, 4);
    strncpy(event_arg_buf, in+6, 4);
    event_id_t event_id = (event_id_t)atoi(event_id_buf);
    char event_arg = (char)atoi(event_arg_buf);
        printf(" Received new message  %s %s\n ",
           topic->name, in);
    for (size_t i = 0; i < len; i++) {
        printf("%c", in[i]);
    }
    puts("");
    event_handler( event_id,  event_arg );
}

static int sub(void)
{
    // if (get_state() < STATE_CONNECTED){
    //     LOG_ERROR("can't sub, disconnected\n");
    //     return -1;
    // }

    if (emcute_sub(&subscription, EMCUTE_QOS_1) != EMCUTE_OK) {
        LOG_ERROR("unable to subscribe to %s\n", SUB_TOPIC);
        return 1;
    }

    LOG_INFO("Successfully subscribed to %s\n", SUB_TOPIC);
    event_handler(EVENT_SUB,0);
    return 0;
    
}
static int unsub(void)
{
    if (get_state() < STATE_SUBSCRIBED ){
        return 1;
    }

    if (emcute_unsub(&subscription) != EMCUTE_OK ){
        LOG_ERROR("Unable to unsub from %s\n", SUB_TOPIC);
        return 2;
    }

    LOG_INFO("Unsubscribed from %s\n", SUB_TOPIC);

    return 0;
}

int con(void)
{
    if (get_state() > STATE_DISCONNECTED){        
        LOG_WARNING("Mqtt already connected\n");
        return 0;
    }

    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    char *topic = (char*)&PUB_TOPIC;
    char *message = "{\"msg_type\":\"status_update\", \"connected\":false}";
    size_t len = strlen(message);


    /* Parsing Address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR) == NULL) {
        printf("[ERR] error parsing IPv6 address %s\n", SERVER_ADDR);
        event_handler(EVENT_ERROR, 0);
        exit(1);
    }
    LOG_INFO("[LOG]: connecting to [%s]:%i\n", SERVER_ADDR, (int)gw.port);

    /* Connecting to broker */
    int err = emcute_con(&gw, 0, topic, message, len,  EMCUTE_QOS_0);
    
    if (err == EMCUTE_DUP) {
        LOG_WARNING("[WARN] Already connected\n");
        return 0;
    } else if (err!= EMCUTE_OK) {
        LOG_ERROR("unable to connect to [%s]:%i (%d) \n ", SERVER_ADDR, (int)gw.port, err);
        return 1;
    }
    LOG_INFO("Successfully connected to gateway at [%s]:%i\n",
           SERVER_ADDR, (int)gw.port);

    /* Registering Topics */
    LOG_DEBUG("[LOG] Trying to register to %s\n ", pub_topic.name);  
    if (emcute_reg(&pub_topic) != EMCUTE_OK) {
        LOG_ERROR("[ERR] unable to obtain ID for topic %s \n", PUB_TOPIC);
        return 1;
    }
    LOG_INFO("Successfully registered to topic %s \n", PUB_TOPIC);
    if (emcute_reg(&sub_topic) != EMCUTE_OK) {
        LOG_ERROR("[ERR] unable to obtain ID for topic %s ", SUB_TOPIC);
        return 1;
    }
    LOG_INFO("Successfully registered to topic %s \n", SUB_TOPIC);
    

    /* Subscribing to SUB_TOPIC */
    memcpy(&(subscription.topic), &sub_topic, sizeof(sub_topic));
    subscription.cb = on_msg;
    err = sub();
    if (err ) {
        LOG_WARNING("Subscribe to %s failed with err %d\n", SUB_TOPIC, err);
        return 1;
    } else{
        LOG_ERROR("Successfully subscribed to %s\n", SUB_TOPIC);
        
    }

    event_handler(EVENT_CON,0);
    return 0;

}

int discon(void)
{   
    int err = 0;
    err = unsub();
    if (err){
        LOG_ERROR("unable to unsub\n");

    }

    err = emcute_discon();
    if (err == EMCUTE_NOGW) {
        LOG_WARNING("not connected to any broker\n");
    } else if (err) {
        LOG_ERROR("unable to disconnect\n");
    } else {
        LOG_INFO("Disconnect successful\n");
    }
    
    event_handler(EVENT_DISCON,0);
    return err;
}

int send( char* payload)
{   int err;
    unsigned flags = EMCUTE_QOS_1;
    

    LOG_DEBUG("Trying to publish %s on topic %s\n",payload, PUB_TOPIC );
    if (get_state() < STATE_CONNECTED){
        LOG_WARNING("Cannot publish, disconnected\n");
        return -1;
    }

    err = emcute_pub(&pub_topic, payload, strlen(payload), flags);
    if (err != EMCUTE_OK) {
        LOG_ERROR("Unable to publish, err %d\n", err);
        return 1;
    }

    printf("Published %i bytes\n",
            (int)strlen(payload));

    return 0;
}


static msg_t mqtt_ping_queue[16];

int mqtt_init(void)
{
    LOG_DEBUG("Mqtt init started\n");
    /* the main thread needs a msg queue to be able to run `ping6`*/
    msg_init_queue(mqtt_ping_queue, ARRAY_SIZE(mqtt_ping_queue));

    /* start the emcute thread */
    mqtt_thread_pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    sub_topic.name = SUB_TOPIC;
    pub_topic.name = PUB_TOPIC;
    
    int err  = mqtt_thread_pid < 0;
    if (!err) {
        LOG_INFO("Mqtt init successful with pid: %d!\n", mqtt_thread_pid);
    } else {
        LOG_ERROR("Mqtt init failed with err %d\n", mqtt_thread_pid);
        event_handler(EVENT_ERROR, 0);
        return 1;
    }
    
    return 0;
  

}
int messaging_init(void){
    return mqtt_init();
}
#else
int EMCUTE_UNUSED=1;
#endif