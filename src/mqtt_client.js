'use strict'

const express = require('express');
const http = require('http');
const WebSocket = require('ws');
var mqtt = require('mqtt')
var client = mqtt.connect() // you add a ws:// url here

var reconnectTimeout = 2000;
var host = "127.0.0.1"; // mosquitto is set on localhost
var port = 1883; // default port we are using for websockets connection

function onConnect() {
    console.log("Connected");
    client.subscribe("/cube");
 // client.subscribe("/group");

    message = new Paho.MQTT.Message("example");
    message.destinationName = "/cube";
 // message.destinationName = "/group";

    mqtt.send(message);
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
  }
  

function MQTTconnect() {
    console.log("Connected to " + host + " " + port);
    mqtt = new Paho.MQTT.Client(host, port, "wise-cube_clientID");
    var options = {
    timeout: 3,
    onSuccess: onConnect,
    };
    mqtt.connect(options);
}