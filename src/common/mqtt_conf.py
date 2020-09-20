import paho.mqtt.client as mqtt
from .handlers import *
import json
from re import finditer
import os
import traceback, sys
from sys import stdout



BROKER_HOST='localhost'
if os.environ.get('IS_DOCKER'):
    BROKER_HOST = 'mqtt-broker'

from_cube_topic = "/from_cube/+"

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(from_cube_topic, qos=0)
    print(f"Connected with result code {rc}")


def on_disconnect(client, userdata, rc):
    print(f"Unexpected disconnection. {rc}")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, msg_handler, message):
    print(f"Received message")
    stdout.flush()
    msg = json.loads(message.payload)
    msg_type = msg["msg_type"]

    print(f"Loaded json message")
    stdout.flush()

    try:
    # if 'cube' in msg.topic:
        print(f"Entered try")
        cube_id = int(finditer("/from_cube/([0-9]+)", message.topic).__next__().group(1))
        print(cube_id)
        c = ScopedSession.query(Cube).filter(Cube.id == cube_id).first()
        print(c)
        g = c.group


        print(f"Received message: {msg} from client {cube_id}")

        msg_handler(client, msg, msg_type, c, g)
        print(f"Handled message: {msg} from client {cube_id}")

    except Exception as e:

        print(f"Received invalid message: {msg.payload} on topic {msg.topic}")


        traceback.print_tb(e.__traceback__)
        einfo = sys.exc_info()
        traceback.print_exception(*einfo)
    finally:
        stdout.flush()


def client_init(msg_handler):
    print("Start Init")
    stdout.flush()

    client = mqtt.Client(client_id="mqtt-sample", clean_session=True, userdata=msg_handler)
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_disconnect = on_disconnect
    client.reconnect_delay_set(min_delay=1, max_delay=120)

    err = client.connect(host=BROKER_HOST, port=1884, keepalive=60)
    print(f'Connection ok? {not err}')
    # client.publish('/to_group/1', 'holaaaaaaaaaaaaaa')

    stdout.flush()
    client.loop_forever()
