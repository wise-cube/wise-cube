'use strict'

const express = require('express');
const http = require('http');
const WebSocket = require('ws');
var mqtt = require('mqtt')
var client = mqtt.connect() // you add a ws:// url here e.g. 'mqtt://test.mosquitto.org'

var clientId = 'mqttjs_' + Math.random().toString(16).substr(2, 8)
var reconnectTimeout = 2000;
var host = "127.0.0.1"; // mosquitto MQTT broker is set on localhost
var port = 1883; // default port we are using for websockets connection

function onConnect() {
    console.log("Connected");
    //client.subscribe("/cube");
    //client.subscribe("/group");

    client.on('connect', function () {
      client.subscribe(['/cube', '/group'], function (err) {
        if (!err) {
          //define a new event handler to trigger actions:
          // TODO
          client.publish('/cube', 'message')
        }
      })
    })

    message = new Paho.MQTT.Message("example");
    message.destinationName = "/cube";
    message.destinationName = "/group";

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
   
    client.on('message', function (topic, message, packet) {
      console.log('Received Message: ' + message.toString() + '\n On topic: ' + topic)
    })
  }
  
  
function MQTTconnect() {
    console.log("Connected to " + host + " " + port);
    mqtt = new Paho.MQTT.Client(host, port, clientID);
    
    var options = {
    clientId: clientId,
    protocolId: 'MQTT',
    wsOptions: {},
    keepalive: 60,
    clean: true,
    reconnectPeriod: 1000,
    connectTimeout: 30 * 1000,
   
    will: {
    topic: 'WillMsg',
    payload: 'Connection Closed abnormally..!',
    qos: 0,
    retain: false
    },
    
    timeout: 3,
    onSuccess: onConnect,
    }; // End of options

    mqtt.connect(options);
}

client.on('close', function () {
  console.log(clientId + ' disconnected')
})
