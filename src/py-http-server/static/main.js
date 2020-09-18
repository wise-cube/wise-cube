const express = require('express');
const http = require('http');
const WebSocket = require('ws');
var mqtt = require('mqtt');


var host = "localhost";
var port = 1885;
var clientid = "group_1";





//var clientid = 'mqttjs_' + Math.random().toString(16).substr(2, 8);




// Create a client instance
client = new Paho.MQTT.Client(host, port, clientid);

// set callback handlers
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

// connect the client
client.connect({onSuccess:onConnect});


// called when the client connects
function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("onConnect");
  client.subscribe("group/req");
  //client.subscribe("group/req/player");
  client.subscribe("group/player");
  client.subscribe("game");
  client.subscribe("game/");

  client.subscribe("game/avatar");
  client.subscribe("game/answer");
  client.subscribe("game/question");

}

function pub(topic, msg){
	console.log("in pub")
	message = new Paho.MQTT.Message(msg);
	message.destinationName = topic;
	client.send(message);
	console.log("message pub")
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived(message) {
  console.log("onMessageArrived:"+message.payloadString);
  var msg = message.payloadString.replace(/\s/g, '"' )
  trigger_pub(msg)
}


function trigger_pub(msg){
	var obj = JSON.parse(msg);
	console.log(obj);
	switch(obj.type) {

	case "ok_event":
		on_ok();
		break;
	case "shake_event":
		on_shake()
		break;
	case "ko_event":
		on_ko();
		break;

	default:
		console.log("Invalid msg_type")
		break;
	}
}


function new_group_resp(id, token, status){
	var payload= ' {"type":"new_group_resp","group_id":' + id + ', "group_token ": ' + token + ', "status" :' + status+ '}';
	console.log("in new resp:   "+ payload)
	pub("group/resp", payload)
}


function new_player_resp(g_id, p_id){
	var payload= ' {"type":"new_player_resp","group_id":' + g_id + ', "player_id" : ' + p_id + '}';
	console.log("in new p_resp:   "+ payload)
	pub("group/resp/player", payload)
}


function player_resp(g_id, p_id){
	var payload= ' {"type":player_resp","group_id":' + g_id + ', "player_id" : ' + p_id + '}';
	console.log("inp_resp:   "+ payload)
	pub("group/resp/player", payload)
}

function send_answer(ans_num){
    if ( !answer) {
        window.location="/new_answer?num="+ans_num
    }

}