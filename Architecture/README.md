# Architecture

The system is composed of 5 main parts

- The wise cube
- NFC Tags
- A webserver
- An MQTT broker
- Database

That should communicate in this fashion

![Sketch](architecture.jpg)

The cube will handle four player for each screen, and will display images received on the player's MQTT channel on the corresponding screen.

In particular when it is placed over a certain NFC tag it shows the player a QR code  with the link to play a game with other people on the same cube

In order to make the game logic we are going to use the following entities.


<img src="entities.png" height="" >


### The magic cube

The cube will mostly work as a proxy between the server and the phone/player.

The hardware is based on a NUCLEO_ST board, preferably a small form factor one like the [STM32](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-mpu-eval-tools/stm32-mcu-mpu-eval-tools/stm32-nucleo-boards/nucleo-f042k6.html). 

It is connected via WIFI (or possibly any other wireless communication method, as it only interacts with the server) and talks to an MQTT broker.

It aims to have four 96x96 [SSD1327  lcd screens](https://www.reichelt.com/it/it/arduino-display-1-12-display-grove-oled-ssd1327-grv-oled-1-12-p191247.html?r=1) on  4 sides, that will mainly display the QR code to join a game and display some information, like the correctness of an answer or a scoreboard.

One of the remaining two faces will have an NFC reader like the [RC522](https://lastminuteengineers.com/how-rfid-works-rc522-arduino-tutorial/) used to trigger the interactions. This will usually be the bottom face.

Therefore, the last face may be flat or contain some buttons.

In order to interact with 4 screens plus an NFC reader we will use multiple slaves over SPI, but anyway, as our overall bandwidth over SPI for reading and writing data is  limited we will costrain the usage of the screen and rather delegate it to the mobile phone.

The cube will post and receive some messages over **MQTT** and in fact the cube will show a QR code that redirects the user to a predefined webpage, along with a token that authenticates him contained in the URL!

This way the user doesn't have to register himself and, as  most phones have now  integrated the feature of opening a qr code from the camera, it doesn't require any additional step!

###### Start a new Group

This event is triggered with an NFC tag containing  the constant `group_req` 
 When triggered it will do the following:

- unsubscribe from all channels

- subscribe on `cube_id` 
- post a  group_req message over `cube_id` and wait for the server to publish the new group_id and the  4 players_id
- subscribe to `cube_id/_group_id/player_n_id/+` for each player 
- Assign a face to each player_id

###### Start a new Game

This event is triggered with an NFC tag containing the constant `game_req` 

 When triggered it will do the following:

- post a `game_req <game_id>`  message over `cube_id/group_id`

- when a qr_message is received on `cube_id/new_group_id/player_n_id` it will show the qr on one of the 4 correponding faces
- when a status_message is received on `cube_id/new_group_id/player_n_id/status` it will update the face of the corresponding player with a certain information that may depend on the game.

### NFC Tags

An NFC tag is a simple integrated circuit with a memory storage, a radio antenna and some logic. They are passive devices, meaning that they need an external source of power to work, in this case it is 'drawn' from the device that read it through induction. 

We actually need one of them for each game we want to make available, each of them should store the following values:

- GROUP_REQ 
- JOIN_GROUP* <group_id>

- GAME_REQ <game_id>

The GROUP_REQ will be used by the staff member to reset the magic cube, whereas the game_req are spread across the museum and will activate different games.

*The JOIN_GROUP tag is an update that should be implemented in a second time.
The idea is that the cube may have another NFC tag to allow another cube to join his group but has to be deeper investigated. 

### Game Server 

The Game Server  should implement both the http server and all the game logic.
It will subscribe to the MQTT broker and listen for incoming messages from the cube.
On a new publish it will update the database in a coherent way, publish a response over MQTT and notifying the web clients via a rest-API or a web socket.

The philosophy is the following: there are 2 communication MQTT for the cubes and HTTP(S) for the phone clients.

MQTT port 8883

HTTPS port 443

HTTP port 80 ( yes, it has  )

MQTT and HTTP rest API follow the same scheme: messages are encoded as a JSON object with one mandatory field: "type" that acts as a selector for the event to be triggered. The remaining fields are type-specific and may contain additional arguments.

A post or a topic will 'identify' the requestor  in fact both the cube and the phone are actually authenticated, the cube because it has a secret key and a signed certificate, the client beacuse it has a random token generated by the server, therefore we will encode all this information in the url like this.

```
http://gameserver/cube_id/group_id/player_id/game_id/access_token
mqtt://gameserver/cube_id/group_id
mqtt://gameserver/cube_id/group_id/player_id
```

the topic in MQTT corresponds to the URL in http, therefore we can share the code, treating a new message as an event, and dispatching the responses on the proper channel.

When a `game_req` request arrives on the MQTT channel `cube_id/group_id` it will have a structure like 

```
{
type:"game_req"
game_id: 0xAF00 
timestamp: 1587848908.49727
}
```

The server will check the group to which it belongs and republish on all the cubes belonging to the same group, in order to let them update their internal state.
Afterwards the server will publish the qr code for the players on the MQTT and start the HTTP listener

```
{
type:"qr_code"
payload: < encoded qr_code > 
}
```

the qr code will point to an URL like 

```
http://zubb-gameserver/cube_id/group_id/game_id/user_id/access_token
```

Where the access_token it's just a light but effective countermeasure that removes the chance of game jamming by simply changing `user_id`  in the url, you have to take a photo of the qr_code in order to join 😉

If the token is correct, it will answer with a web page displaying the game, and then users will answer a list of answers and  client/server and will communicate about the status of the game with messages like

SERVER -> CLIENT

````
{
"type":"question",
"number": 0x01
"title":"How did Julius Caesar die ?".
"ans": {
			 "A":"Slained by a tiger"
			 "B":"Killed by a fan"
			 "C":"Overdose"
			 "D":"He's still alive"
			}
}
````

CLIENT -> SERVER 

```
{
"type":"answer",
"ans": "A"
}
```

### MQTT broker

A simple mqtt broker that may be implemented with any technology.
It is only required to bridge the communication between the broker and the Game Server

It should only accept secure TLS connections.

### Database

It is used  by the Game Server to keep historical data and add a persistency layer for future statistics and insights.

