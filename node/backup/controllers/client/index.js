// ============================================================
// CLIENT
var Client;
var controllerInstance;

module.exports = Client = (function() {
  var config = require('config');
  var events = require('events');

  var dgram = require('dgram');
  var osc = require('osc-min');

  // --------------------------------------------------------------
  function Client() {
    console.log('Client | controller -> Constructor');

    this.NAMESPACE = 'client';
    this.eve = new events.EventEmitter();
    this.udp = dgram.createSocket("udp4");
  }


  // --------------------------------------------------------------
  Client.prototype = new events.EventEmitter();


  // --------------------------------------------------------------
  Client.prototype.setup = (function(_this) {
    return function(data) {
      console.log('Client | controller -> Setup');

      var app = module.parent.exports;

      var controllers = app.get('controllers');
      var socketIO = app.get('socketIO').of(controllers.client.NAMESPACE);

      var eve = controllers.client.eve;
      var udp = controllers.client.udp;

      // socket.io の処理
      socketIO.on('connection', function(socket) {
        console.log('Client | controller  -> socketIO -> connection');

        var buffer;
        buffer = osc.toBuffer({
          address: "/word",
          args: [{
            type: "string",
            value: "HELLO WORLD"
          }]
        });
        udp.send(buffer, 0, buffer.length, 12002, "localhost");
      });
    };
  })(this);


  // --------------------------------------------------------------
  return Client;
})();

module.exports = controllerInstance = new Client();


// // ============================================================
// // CLIENT
// var controllerInstance;
// var Client;
// module.exports = Client = (function() {
//   var config = require('config').get('Config');
//   var events = require('events');

//   // --------------------------------------------------------------
//   function Client() {
//     console.log('Client -> Constructor');

//     this.NAMESPACE = 'client';
//     this.eve = new events.EventEmitter();
//   }

//   // --------------------------------------------------------------
//   Client.prototype = new events.EventEmitter();

//   // --------------------------------------------------------------
//   Client.prototype.setup = (function(_this) {
//     return function(data){
//       console.log('Client -> Setup');

//       var app = module.parent.exports;
//       // _this.eve = new events.EventEmitter();

//       var controllers = app.get('controllers');
//       var socketIO = app.get('socketIO').of(controllers.client.NAMESPACE);
//       var eve = controllers.client.eve;
      

//       socketIO.on('connection', function(socket){
//         console.log('Client -> socketIO -> connection');

//         socket.on('join', function(data, callback){
//           console.log('Client -> socketIO -> join');
//           var id = data.id;

//           console.log(data);

//           eve.emit('join', id, function(err, room){
//             if(err){
//               console.log(err);
//               callback(err);
//               return;
//             }
//             socket.roomId = id;
//             socket.join(id);
//             callback(undefined, id);
//           });
//         });

//         socket.on('post', function(data, callback){
//           console.log('Client -> socketIO -> post');
          
//           eve.emit('post', socket.roomId, data, function(err, imageId){
//             if(err){
//               callback(err);
//               return;
//             }
//             callback(err, imageId);
//           });
//         });

//         socket.on('trigger', function(data){
//           console.log('Client -> socketIO -> trigger');
//           console.log(data);

//           eve.emit('trigger', socket.roomId, data, function(err, imageId){
//             if(err){
//               callback(err);
//               return;
//             }
//             callback(err, imageId);
//           });
//         });

//       });


//       /*
//        * eventEmitter
//        */
//       eve.on('join', function(roomId, callback){
//         console.log('Client -> eventEmitter -> join');

//         controllers.rooms.show(roomId, function(err, room){
//           if(err){
//             callback(err);
//             return;
//           }
//           if(!room){
//             callback(new Error("Not Found | Client -> eventEmitter -> join"));
//             return;
//           }
//           callback(undefined, room);
//         });
//       });

//       eve.on('post', function(roomId, data, callback){
//         console.log('Client -> eventEmitter -> post');

//         controllers.rooms.images.update(roomId, data, function(err, image){
//           if(err){
//             callback(err);
//             return;
//           }
//           controllers.viewer.post(image, data);
//           callback(undefined, image.id);
//         });
//       });

//       eve.on('trigger', function(roomId, data){
//         console.log('Client -> eventEmitter -> trigger');

//         controllers.viewer.trigger(roomId, data);
//       });


//       // /* ---------- */
//       // _this.socketIO.on('join', function(roomId, callback){
//       //   controllers.rooms.show(roomId, function(err, room){
//       //     if(err){
//       //       callback(err);
//       //       return;
//       //     }
//       //     if(!room){
//       //       callback(new Error("Not Found"));
//       //       return;
//       //     }
//       //     callback(undefined, room);
//       //   });
//       // });

//       // /* ---------- */
//       // _this.socketIO.on('post', function(roomId, data, callback){
//       //   controllers.rooms.images.update(roomId, data, function(err, image){
//       //     if(err){
//       //       callback(err);
//       //       return;
//       //     }
//       //     controllers.viewer.post(image, data);
//       //     callback(undefined, image.id);
//       //   });
//       // });

//       // /* ---------- */
//       // _this.socketIO.on('trigger', function(roomId, data){
//       //   _this.controllers.viewer.trigger(roomId, data);
//       // });
//     };
//   })(this);

//   // --------------------------------------------------------------
//   // Client.prototype = new events.EventEmitter();

//   return Client;
// })();

// module.exports = controllerInstance = new Client();
