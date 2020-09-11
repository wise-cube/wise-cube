import paho.mqtt.client as mqtt


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("game/", qos=2)  # set correct topic name
    # client.subscribe([("my/topic", 0), ("another/topic", 2)]) #use an array if you need to sub to many topics
    client.publish("group/", payload=None, qos=2, retain=False)  # set correct topic name


def on_disconnect(client, userdata, rc):
    if rc != 0:
        print("Unexpected disconnection.")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print("Received message '" + str(msg.payload) + "' on topic '" + msg.topic + "' with QoS " + str(msg.qos))


client = mqtt.Client(client_id="", clean_session=True, userdata=None, transport="websockets")
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect

# option functions
client.max_inflight_messages_set(self, inflight)
client.max_queued_messages_set(self, queue_size)
client.message_retry_set(retry)
client.ws_set_options(self, path="/mqtt", headers=None)
client.will_set("sample/will_msg", payload=None, qos=2, retain=False)  # specify right topic for will_msg
client.reconnect_delay_set(min_delay=1, max_delay=120)

"""
client.tls_set(ca_certs=None, certfile=None, keyfile=None, cert_reqs=ssl.CERT_REQUIRED,
    tls_version=ssl.PROTOCOL_TLS, ciphers=None)
"""

client.connect(host="localhost", port=1885, keepalive=60) #set correct port

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
