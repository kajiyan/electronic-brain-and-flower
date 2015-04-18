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
  }


  // --------------------------------------------------------------
  Client.prototype = new events.EventEmitter();


  // --------------------------------------------------------------
  Client.prototype.setup = (function(_this) {
    return function(data){
      console.log('[Controller] Client -> Setup');

      var app = module.parent.exports;

      var controllers = app.get('controllers');
      var socketIO = app.get('socketIO').of(controllers.client.NAMESPACE);
      var eve = controllers.client.eve;
      

      socketIO.on('connection', function(socket){
        console.log('[Controller] Client -> socketIO -> connection');

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

        socket.on('trigger', function(data){
          console.log('[Controller] Client -> socketIO -> trigger');
          console.log(socket.roomId,data);

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
  return Client;
})();

module.exports = controllerInstance = new Client();