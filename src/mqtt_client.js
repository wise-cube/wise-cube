/*'use strict'

const express = require('express');
const http = require('http');
const WebSocket = require('ws');
var mqtt = require('mqtt');*/
console.log("after require");
var host = "localhost";
var port = 1885;
var clientid = "cubee";

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
  client.subscribe("group/req/player");
  client.subscribe("group/req");
  client.subscribe("game");
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
	case "new_group_req":
		console.log("Trigger new_group_resp");
		new_group_resp();
		break;
	case "new_player_req":
		console.log("Trigger new_player_resp");
		new_player_resp();
		break;
	case "new_player_acc":
		break;
	case "resume_group_req":
		break;
	case "new_game_event":
		break;
	case "player_req":
		break;
	case "new_question":
		break;
	case "new_group_event":
		break;
	default:
		console.log("NO CASE!!!!")
		break;
	}		
}


function new_group_resp(){
	var id = "2";
	var token ="abc"
	var status= "200"
	var payload= ' {"type":"new_group_resp","group_id":' + id + ', "group_token ": ' + token + ', "status" :' + status+ '}';
	console.log("in new resp:   "+ payload)
	pub("group/resp", payload)
}


function new_player_resp(){
	var g_id = "2";
	var p_id ="1a"
	var payload= ' {"type":"new_player_resp","group_id":' + g_id + ', "player_id" : ' + p_id + '}';
	console.log("in new p_resp:   "+ payload)
	pub("group/resp/player", payload)
}


