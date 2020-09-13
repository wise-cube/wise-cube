import paho.mqtt.client as mqtt
import json

topicSub: str = "/from_cubes"
topicPub: str = "/to_cube/1"
STATES=["START", "GROUP_CREATION", "AVATAR_CREATION", "GROUP_READY"]
#State : group_creation, avatar_creation, 
cubeState = STATES[0]

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(topicSub, qos=0)  
    print ("topic subscribe")
    client.publish(topicPub, "prova", qos=0, retain=False)  
    
    # client.subscribe([("my/topic", 0), ("another/topic", 2)]) #use an array if you need to sub to many topics


def on_disconnect(client, userdata, rc):
    if rc != 0:
        print("Unexpected disconnection.")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
	print("Received message : " + str(msg.payload) + " -  on topic '" + msg.topic + "' with QoS " + str(msg.qos))
	type_rec(msg)
	
	
def type_rec(msg):
	global cubeState
	json_data=json.loads(msg.payload)
	msg_type = json_data["msg_type"]

	#Shake 
	if msg_type == "shake_event":
		print ("shake event : " + msg_type)
		print ("state: " + cubeState)
		if cubeState == STATES[1]:
			cubeState = STATES[2]
			print(" messaggio visualizza avatar")
		elif cubeState == STATES[2]:
			print ("messaggio visualizza avatar diverso")
		else:
			print("no actions")
			
	#button ok 	
	elif msg_type == "button_ok_event":
		print ("button_ok_event : " + msg_type + "\n")
		print ("state: " + cubeState)
		
		if cubeState == STATES[0]:
			print("inizio creazione gruppo")
			cubeState = STATES[1]
			
		elif cubeState == STATES[1]:
			print("fine creazione gruppo")
			cubeState = STATES[3]
			
		elif cubeState == STATES[2]:
			print("messaggio salva avatar")
			cubeState = STATES[1]
			
		else :
			print("no state change")
	
	#button ko	
	elif msg_type == "button_ko_event":
		print ("button_ko_event : " + msg_type)
			
	else: 
		print ("no match ")
	
	
	
	

client = mqtt.Client(client_id="", clean_session=True, userdata=None, transport="websockets")
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect

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
