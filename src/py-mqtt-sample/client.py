import json
import paho.mqtt.client as mqtt
from common.handlers import CubeHandler, GroupHandler
print('Starting')
from time import sleep
from sys import stdout
from common.conf import MqttClient

topicSub = "/from_cubes"
topicPub = "/to_cube/1"
STATES = ["START", "GROUP_CREATION", "AVATAR_CREATION", "GROUP_READY"]
cubeState = STATES[0]


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(topicSub, qos=0)
    print("topic subscribe")
    client.publish(topicPub, "prova", qos=0, retain=False)

    # client.subscribe([("my/topic", 0), ("another/topic", 2)]) #use an array if you need to sub to many topics


def on_disconnect(client, userdata, rc):
    if rc != 0:
        print("Unexpected disconnection.")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
	print("Received message: " + str(msg.payload) + " -  on topic '" + msg.topic + "' with QoS " + str(msg.qos))
	type_rec(msg)
	stdout.flush()


def type_rec(msg):
	global cubeState
	json_data = json.loads(msg.payload)
	msg_type = json_data["msg_type"]

	#Shake
	if msg_type == "shake_event":
		print("shake event: " + msg_type)
		print("state: " + cubeState)
		if cubeState == STATES[1]:
			cubeState = STATES[2]
			print("Message: visualize avatar")
		elif cubeState == STATES[2]:
			print("Message: visualize different avatar")
		else:
			print("No actions")

	#button ok
	elif msg_type == "button_ok_event":
		print("button_ok_event: " + msg_type + "\n")
		print("state: " + cubeState)

		if cubeState == STATES[0]:
			print("Begin group creation...")
			cubeState = STATES[1]

		elif cubeState == STATES[1]:
			print("Finished group creation!")
			cubeState = STATES[3]

		elif cubeState == STATES[2]:
			print("Message: save avatar")
			cubeState = STATES[1]

		else:
			print("No state change")

	#button ko
	elif msg_type == "button_ko_event":
		print ("button_ko_event: " + msg_type)

	else:
		print("No match")


client = mqtt.Client(client_id="mqtt-sample", clean_session=True, userdata=None)

client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect
client.reconnect_delay_set(min_delay=1, max_delay=120)

err = client.connect(host="mqtt-broker", port=1884, keepalive=60)

print(f'Connection ok? {not err}')
stdout.flush()

MqttClient = client
# # Blocking call that processes network traffic, dispatches callbacks and
# # handles reconnecting.
# # Other loop*() functions are available that give a threaded interface and a
# # manual interface.
client.loop_forever()
