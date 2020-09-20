from common.handlers import CubeHandler, GroupHandler
from sys import stdout
from common.mqtt_conf import client_init
import json

def message_handler(client, msg, msg_type, c, g):
    print(type(msg))

    if msg_type == 'button_ok_event':
        client.publish(GroupHandler(g).to_group_topic(), 'shake')
    elif  msg_type == 'shake_event':
        client.publish(GroupHandler(g).to_group_topic(), 'shake')
    elif msg_type == 'button_ko_event':
        client.publish(GroupHandler(g).to_group_topic(), 'shake')
    else:
        print('msg_type not recognized')
    stdout.flush()


def message_handler(client, msg, msg_type, c, g):
    print(type(msg))

    if msg_type in ['button_ok_event','shake_event','button_ko_event'] :
        client.publish('/to_group/1', json.dumps(msg))
    elif msg_type == 'answer_event':
        client.publish('/to_group/1', json.dumps({'msg_type':'sel_ans','num':msg['num']}))

    stdout.flush()

client_init(message_handler)


