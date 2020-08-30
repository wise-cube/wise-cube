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
  client.subscribe("group");
  client.subscribe("group/player");
  client.subscribe("group/req");
  client.subscribe("game");
  client.subscribe("game/answer");
  client.subscribe("game/question");
  //message = new Paho.MQTT.Message("Hello");
  //message.destinationName = "World";
  //client.send(message);
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
  check_type_msg(message.toString());  
}


function check_type_msg(msg){
	
	var p=JSON.parse(msg.type);	
    
    
}
