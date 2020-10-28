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
  client.subscribe("/to_group/1");
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
  var msg = JSON.parse(message.payloadString)
  trigger_pub(msg)
}


function trigger_pub(msg){
	var event = document.createEvent("MouseEvent");
	event.initMouseEvent("click", true, true, window, 0, 0, 0, 0, 0, false, false, false, false, 0, null);

	switch(msg.msg_type) {

	case "long_press_event":
		var elem = document.getElementById("progress_button")	;

		break;
	case "shake_event":
		var elem = document.getElementById("random_button");
		break;
	case "short_press_event":
		var elem = document.getElementById("end_button")	;
		break;
	case "sel_ans":
		on_sel_ans(msg.num);
		break;
	default:
		console.log("Invalid msg_type")
		break;
	}

	if (typeof(elem) != 'undefined' && elem != null) {
		console.log("in if")
		elem.dispatchEvent(event);
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