var io = require('socket.io-client');
var dgram = require('dgram');
var osc = require('osc-min');


var viewerSocket = io.connect('http://localhost:3030/viewer');
var udp = dgram.createSocket("udp4");


viewerSocket.on("post", function(data){
  console.log("Bridge App | Socket -> _postHandler");

  var buffer;

  buffer = osc.toBuffer({
    address: "/addImage",
    args: [
      {
        type: "string",
        value: data.url
      }
    ]
  });
  udp.send(buffer, 0, buffer.length, 12002, "localhost");
});

viewerSocket.on('connect', function(socket){
  console.log('Bridge App | Socket -> Connect');

  viewerSocket.emit('join', {id: '983621'}, function(err, roomID){
    if(err){
      console.log(err);
      return;
    }
  });
});

// var socket = io.connect('http://ec2-54-65-140-79.ap-northeast-1.compute.amazonaws.com:3030/client');
 
// socket.on('news', function (data) {
//     console.log(data);
//     socket.emit('my other evenr', {my: 'data'});
// });


// var webSocket = require('websocket');
// var WebSocketClient = webSocket.client;

// var webSocketClient = new WebSocketClient();

// webSocketClient.on('connectFailed', function(error) {
//   console.log('Connect Error: ' + error.toString());
// });

// webSocketClient.on('connect', function(connection) {
//   console.log('WebSocket webSocketClient Connected');

//   connection.on('error', function(error) {
//     console.log("Connection Error: " + error.toString());
//   });

//   connection.on('close', function() {
//     console.log('echo-protocol Connection Closed');
//   });

//   connection.on('message', function(message) {
//     if (message.type === 'utf8') {
//       console.log("Received: '" + message.utf8Data + "'");
//     }
//   });

//   function sendNumber() {
//     if (connection.connected) {
//       var number = Math.round(Math.random() * 0xFFFFFF);
//       connection.sendUTF(number.toString());
//       setTimeout(sendNumber, 1000);
//     }
//   }
//   sendNumber();
// });
  
// webSocketClient.connect('ws://localhost:3030', 'bridge-protocol');