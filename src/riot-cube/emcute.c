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
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
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
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    char *topic = PUB_TOPIC;
    char *message = "{\"msg_type\":\"status_update\", \"connected\":false}";
    size_t len = strlen(message);


    /* parse address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_HOST) == NULL) {
        printf("error parsing IPv6 address %s\n", BROKER_HOST);
        return 1;
    }
    printf("[LOG]: connecting to [%s]:%i\n", BROKER_HOST, (int)gw.port);
    int err = emcute_con(&gw, true, topic, message, len, 0);
    if (err == EMCUTE_DUP) {
        printf("[LOG] Already connected");
    } else if (err!= EMCUTE_OK) {
    // if (err!= EMCUTE_OK) {
        printf("[ERR] unable to connect to [%s]:%i (%d) \n ", BROKER_HOST, (int)gw.port, err);
        return 1;
    }
    printf("Successfully connected to gateway at [%s]:%i\n",
           BROKER_HOST, (int)gw.port);

    set_state(STATE_CONNECTED);
    return 0;
    
}

int discon(void)
{
    int res = emcute_discon();
    if (res == EMCUTE_NOGW) {
        puts("[ERR] not connected to any broker");
        return 1;
    }
    else if (res != EMCUTE_OK) {
        puts("[ERR] unable to disconnect");
        return 1;
    }
    puts("Disconnect successful");
    return 0;

    set_state(STATE_INITIALIZED);
}

int pub(char* topic, char* payload)
{

    (void)topic;

    unsigned flags = EMCUTE_QOS_0;
    

    printf("[MQTT] Published new message \n\t[TOPIC] %s\n\t[MSG] %s \n ", topic, payload);

    // if (emcute_reg(&pub_topic) != EMCUTE_OK) {
    //     printf("[ERR] unable to obtain topic %s ID", PUB_TOPIC);
    //     return 1;
    // }
    /* step 2: publish data */
    if (emcute_pub(&pub_topic, payload, strlen(payload), flags) != EMCUTE_OK) {
        printf("[ERR] unable to publish data to topic '%s [%i]'\n",
                pub_topic.name, (int)pub_topic.id);
        return 1;
    }

    printf("Published %i bytes to topic '%s [%i]'\n",
            (int)strlen(payload), pub_topic.name, pub_topic.id);

    return 0;
}

int sub(  char* topic)
{
    unsigned flags = EMCUTE_QOS_0;

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

    printf("[LOG] Trying to register to %s ", pub_topic.name);  
    if (emcute_reg(&pub_topic) != EMCUTE_OK) {
        printf("[ERR] unable to obtain ID for topic %s ", PUB_TOPIC);
        return 1;
    }
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