import json

from common.handlers import CubeHandler, GroupHandler
from common.conf import ScopedSession
from common.models import *
from time import sleep
from sys import stdout
from common.conf import MqttClient
from re import finditer
import os
import traceback, sys

BROKER_HOST='localhost'
if os.environ.get('IS_DOCKER'):
    BROKER_HOST = 'mqtt-broker'

from_cube_topic = "/from_cube/+"
to_cube_topic = "/to_cube/1"
to_group_topic = "/to_cube/1"

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(from_cube_topic, qos=0)
    print(f"Connected with result code {rc}")


def on_disconnect(client, userdata, rc):
    print(f"Unexpected disconnection. {rc}")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):

    try:
    # if 'cube' in msg.topic:
        cube_id = int(finditer("/from_cube/([0-9]+)", msg.topic).__next__().group(1))
        print(cube_id)
        # ch = CubeHandler.by_id(cube_id)
        # c = ScopedSession.query(Cube).filter(Cube.id == cube_id).first()
        c = ScopedSession.query(Cube).first()
        g = ScopedSession.query(Group).filter(Group.id == c.group_id).first()
       """
        print(g)
        ch = CubeHandler(c)
        gh = None
        if g:
            gh = GroupHandler(g)
        """
        # topic = msg.topic
        msg = json.loads(msg.payload)
        msg_type = msg["msg_type"]


        print(f"Received message: {msg} from client {cube_id}")
        stdout.flush()
        message_handler(msg, msg_type, ch, gh)

    except Exception as e:

        print(f"Received invalid message: {msg.payload} on topic {msg.topic}")


        traceback.print_tb(e.__traceback__)
        einfo = sys.exc_info()
        traceback.print_exception(*einfo)
    finally:
        stdout.flush()

def message_handler(msg, type, ch, gh):
    if type == 'button_ok_event':
        gh.button_ok()
    elif  type == 'shake_event':
        gh.shake()
    elif type == 'button_ko_event':
        gh.button_ko()
    else:
        print('msg_type not recognized')
    stdout.flush()



# def type_rec(msg):
#     global cubeState
#
#
#     #Shake
#     if msg_type == "shake_event":
#         print("shake event: " + msg_type)
#         print("state: " + cubeState)
#         if cubeState == STATES[1]:
#             cubeState = STATES[2]
#             print("Message: visualize avatar")
#         elif cubeState == STATES[2]:
#             print("Message: visualize different avatar")
#         else:
#             print("No actions")
#
#     #button ok
#     elif msg_type == "button_ok_event":
#         print("button_ok_event: " + msg_type + "\n")
#         print("state: " + cubeState)
#
#         if cubeState == STATES[0]:
#             print("Begin group creation...")
#             cubeState = STATES[1]
#
#         elif cubeState == STATES[1]:
#             print("Finished group creation!")
#             cubeState = STATES[3]
#
#         elif cubeState == STATES[2]:
#             print("Message: save avatar")
#             cubeState = STATES[1]
#
#         else:
#             print("No state change")
#
#     #button ko
#     elif msg_type == "button_ko_event":
#         print ("button_ko_event: " + msg_type)
#
#     else:
#         print("No match")

def client_init():
    print("Start Init")
    stdout.flush()
    client = MqttClient

    client.on_connect = on_connect
    client.on_message = on_message
    client.on_disconnect = on_disconnect
    client.reconnect_delay_set(min_delay=1, max_delay=120)

    err = client.connect(host=BROKER_HOST, port=1884, keepalive=60)
    print(f'Connection ok? {not err}')
    stdout.flush()
    client.loop_start()
    client.publish('/to_group/1','holaaaaaaaaaaaaaa')


client_init()
sleep(2<<10)
# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
# print("Start Loop")
# stdout.flush()

