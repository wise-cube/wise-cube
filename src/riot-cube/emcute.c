#ifndef USE_MQTT

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "utils.h"
#include "mqtt.h"
#ifndef EMCUTE_ID
#define EMCUTE_ID           ("gertrud")
#endif
#define EMCUTE_PORT         (1886U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

emcute_topic_t sub_topic,pub_topic;
extern state_t current_state;

int mqtt_thread_pid; 
static void *emcute_thread(void *arg)
{
    char emcute_id_buf[16] = {0};
    snprintf(emcute_id_buf, 16, "%s_%d", EMCUTE_ID, rand() & 0xffffffff);
    (void)arg;
    emcute_run(EMCUTE_PORT, emcute_id_buf);
    return NULL;    /* should never be reached */
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;

        printf("[MQTT] Received new message \n\t[TOPIC] %s\n\t[MSG] %s\n ",
           topic->name, in);
    for (size_t i = 0; i < len; i++) {
        printf("%c", in[i]);
    }
    puts("");
}


int con(void)
{
    if (current_state > STATE_DISCONNECTED){        
        puts("[LOG] Already connected");
        return 0;
    }

    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    char *topic = (char*)&PUB_TOPIC;
    char *message = "{\"msg_type\":\"status_update\", \"connected\":false}";
    size_t len = strlen(message);


    /* parse address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_HOST) == NULL) {
        printf("[ERR] error parsing IPv6 address %s\n", BROKER_HOST);
        set_state(STATE_DISCONNECTED);
        exit(1);
    }

    printf("[LOG]: connecting to [%s]:%i\n", BROKER_HOST, (int)gw.port);

    int err = emcute_con(&gw, true, topic, message, len,  EMCUTE_QOS_1);
    printf("after connection\n");
    if (err == EMCUTE_DUP) {
        printf("[LOG] Already connected");
    } else if (err!= EMCUTE_OK) {
    // if (err!= EMCUTE_OK) {
        printf("[ERR] unable to connect to [%s]:%i (%d) \n ", BROKER_HOST, (int)gw.port, err);
        set_state(STATE_DISCONNECTED);
        return 1;
    }
    printf("Successfully connected to gateway at [%s]:%i\n",
           BROKER_HOST, (int)gw.port);

    printf("[LOG] Trying to register to %s ", pub_topic.name);  
    if (emcute_reg(&pub_topic) != EMCUTE_OK) {
        printf("[ERR] unable to obtain ID for topic %s ", PUB_TOPIC);
        return 1;
    }

    set_state(STATE_CONNECTED);
    return 0;
    
}

int discon(void)
{
    int err = emcute_discon();
    if (err == EMCUTE_NOGW) {
        puts("[ERR] not connected to any broker");

    }
    else if (err) {
        puts("[ERR] unable to disconnect");

    } else {
        puts("Disconnect successful");
    }
    
    set_state(STATE_DISCONNECTED);
    return err;

    
}

int pub(char* topic, char* payload)
{   int err;


    (void)topic;

    unsigned flags = EMCUTE_QOS_1;
    

    printf("[MQTT] Publishing new message \n\t[TOPIC] %s\n\t[MSG] %s \n ", topic, payload);
    if (current_state < STATE_CONNECTED){
        puts("[ERR] disconnected");
        return -1;
    }
    // if (emcute_reg(&pub_topic) != EMCUTE_OK) {
    //     printf("[ERR] unable to obtain topic %s ID", PUB_TOPIC);
    //     return 1;
    // }
    /* step 2: publish data */
    err = emcute_pub(&pub_topic, payload, strlen(payload), flags);
    if (err != EMCUTE_OK) {
        printf("[ERR] unable to publish data to topic '%s [%i]' err %d\n",
                pub_topic.name, (int)pub_topic.id, err);
        // set_state(STATE_DISCONNECTED);
        return 1;
    }

    printf("Published %i bytes to topic '%s [%i]'\n",
            (int)strlen(payload), pub_topic.name, pub_topic.id);

    return 0;
}

int sub(  char* topic)
{
    if (current_state != STATE_CONNECTED){
        return -1;
    }
    unsigned flags = EMCUTE_QOS_1;

    /* find empty subscription slot */
    unsigned i = 0;
    for (; (i < NUMOFSUBS) && (subscriptions[i].topic.id != 0); i++) {}
    if (i == NUMOFSUBS) {
        puts("[ERR] no memory to store new subscriptions");
        return 1;
    }

    subscriptions[i].cb = on_pub;
    strcpy(topics[i], topic);
    subscriptions[i].topic.name = topics[i];
    if (emcute_sub(&subscriptions[i], flags) != EMCUTE_OK) {
        printf("[ERR] unable to subscribe to %s\n", topic);
        return 1;
    }

    printf("Now subscribed to %s\n", topic);
    return 0;
}

int unsub(char* topic)
{
    if (current_state != STATE_CONNECTED){
        return -1;
    }
    /* find subscriptions entry */
    for (unsigned i = 0; i < NUMOFSUBS; i++) {
        if (subscriptions[i].topic.name &&
            (strcmp(subscriptions[i].topic.name, topic) == 0)) {
            if (emcute_unsub(&subscriptions[i]) == EMCUTE_OK) {
                memset(&subscriptions[i], 0, sizeof(emcute_sub_t));
                printf("Unsubscribed from '%s'\n", topic);
            }
            else {
                printf("Unsubscription form '%s' failed\n", topic);
            }
            return 0;
        }
    }

    printf("[ERR] no subscription for topic '%s' found\n", topic);
    return 1;
}



int mqtt_init(void)
{
    printf("in emcuteeeeeeeeeeeeeeeeee\n");
    /* the main thread needs a msg queue to be able to run `ping6`*/
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* step 1: get topic id */

    /* initialize our subscription buffers */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    memset(&pub_topic, 0, ( sizeof(pub_topic)));
    memset(&sub_topic, 0, ( sizeof(sub_topic)));

    /* start the emcute thread */
    mqtt_thread_pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    sub_topic.name = SUB_TOPIC;
    pub_topic.name = PUB_TOPIC;
    
    con();

  
    // if (emcute_reg(&pub_topic) != EMCUTE_OK) {
    //     puts("[ERR] unable to obtain topic ID");
    //     return 1;
    // }



    // while (thread_getstatus(mqtt_thread_pid) != STATUS_PENDING)
    // {
    //     xtimer_sleep(1);
    // }
    /* should be never reached */
    return 0;
}
#endif
