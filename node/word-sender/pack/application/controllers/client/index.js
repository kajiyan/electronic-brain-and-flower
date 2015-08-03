// ============================================================
// CLIENT
var controllerInstance;
var Client;

module.exports = Client = (function() {
  var config = require('config');
  var events = require('events');

  // --------------------------------------------------------------
  function Client() {
    console.log('[Controller] Client -> Constructor');

    this.NAMESPACE = 'client';
    this.eve = new events.EventEmitter();
    this.socketIO = null;
  }


  // --------------------------------------------------------------
  Client.prototype = new events.EventEmitter();


  // --------------------------------------------------------------
  Client.prototype.setup = (function(_this) {
    return function(data){
      console.log('[Controller] Client -> Setup');

      var app = module.parent.exports;

      var controllers = app.get('controllers');
      var eve = controllers.client.eve;

      this.socketIO = app.get('socketIO').of(controllers.client.NAMESPACE);

      this.socketIO.on('connection', function(socket){
        console.log('[Controller] Client -> socketIO -> connection');

        // From Local Node.js
        socket.on('readText', function(data, callback){
          console.log('[Controller] Client -> socketIO -> readText');

          // dataには読み上げ用のテキスト（text）とRoomIdが入っている
          controllers.client.sendReadText( data );
        });

        // From Local Node.js
        socket.on('readLightValue', function(data, callback){
          console.log('[Controller] Client -> socketIO -> readLightValue');

          controllers.client.sendLightValue( data );
        });

        socket.on('join', function(data, callback){
          console.log('[Controller] Client -> socketIO -> join');
          var id = data.id;

          console.log(data);

          eve.emit('join', id, function(err, room){
            if(err){
              console.log(err);
              callback(err);
              return;
            }
            socket.roomId = id;
            socket.join(id);
            callback(undefined, id);
          });
        });

        socket.on('post', function(data, callback){
          console.log('[Controller] Client -> socketIO -> post');
          
          eve.emit('post', socket.roomId, data, function(err, imageId){
            if(err){
              callback(err);
              return;
            }
            callback(err, imageId);
          });
        });

        // 画像が送信された時に発火する
        socket.on('trigger', function(data){
          console.log('[Controller] Client -> socketIO -> trigger');
          console.log(socket.roomId, data);

          // controllers.bridge.socketIO.to(roomId).emit('trigger', data);

          eve.emit('trigger', socket.roomId, data, function(err, imageId){
            if(err){
              callback(err);
              return;
            }
            callback(err, imageId);
          });
        });

      });


      /*
       * eventEmitter
       */
      eve.on('test', function(roomId, callback){
        console.log('[Controller] Client -> eventEmitter -> test');

        console.log(controllers.client);
        // socketIO.to(roomId).emit('fromBridgeAppReadText', '');
      });

      eve.on('join', function(roomId, callback){
        console.log('[Controller] Client -> eventEmitter -> join');

        controllers.rooms.show(roomId, function(err, room){
          if(err){
            callback(err);
            return;
          }
          if(!room){
            callback(new Error("Not Found | [Controller] Client -> eventEmitter -> join"));
            return;
          }
          callback(undefined, room);
        });
      });

      eve.on('post', function(roomId, data, callback){
        console.log('[Controller] Client -> eventEmitter -> post');

        controllers.rooms.images.update(roomId, data, function(err, image){
          if(err){
            callback(err);
            return;
          }
          controllers.viewer.post(image, data);
          callback(undefined, image.id);
        });
      });

      eve.on('trigger', function(roomId, data){
        console.log('[Controller] Client -> eventEmitter -> trigger');

        controllers.viewer.trigger(roomId, data);
      });
    };
  })(this);
  
  // --------------------------------------------------------------
  Client.prototype.sendReadText = function( data ){
    console.log('---------');
    console.log('[Controller] Client -> sendReadText');

    this.socketIO.to(data.roomId).emit('readText', data);

    console.log('---------');
  };

  // --------------------------------------------------------------
  Client.prototype.sendLightValue = function( data ){
    console.log('---------');
    console.log('[Controller] Client -> sendLightState');

    this.socketIO.to(data.roomId).emit('readLightValue', data);

    console.log('---------');
  };

  return Client;
})();

module.exports = controllerInstance = new Client();