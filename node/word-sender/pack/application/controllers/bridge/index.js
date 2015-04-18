// ============================================================
// BRIDGE
var controllerInstance;
var Bridge;

module.exports = Bridge = (function() {
  var config = require('config');
  var events = require('events');


  // --------------------------------------------------------------
  function Bridge() {
    console.log('[Controller] Bridge -> Constructor');

    this.NAMESPACE = 'bridge';
    this.eve = new events.EventEmitter();
    this.socketIO = null;
    // this.socket = null;
    this.joinID = '';
  }


  // --------------------------------------------------------------
  Bridge.prototype = new events.EventEmitter();


  // --------------------------------------------------------------
  Bridge.prototype.setup = function() {
    console.log('[Controller] Bridge -> Setup');

    var app = module.parent.exports;
      
    var controllers = app.get('controllers');
    var eve = controllers.viewer.eve;

    this.socketIO = app.get('socketIO').of(controllers.bridge.NAMESPACE);
    _socket = null;
    // socket = this.socket

    this.socketIO.on('connection', function(socket) {
      console.log('[Controller] Bridge -> socketIO -> connection');

      _socket = socket;
    });

    /*
     * eventEmitter
     */
    eve.on('join', function(roomId, callback){
      console.log('[Controller] Bridge -> socketIO -> connection -> join');

  
    });

    // this.socketIO.on('connection', function(socket) {
    //   console.log('[Controller] Bridge -> socketIO -> connection');
    // });

  //     socket.on('join', function(data, callback) {
  //       console.log('[Controller] Bridge -> socketIO -> connection -> join');

  //       eve.emit('join', data.id, function(err, id) {
  // //         if(err){
  // //           callback(err);
  // //           return;
  // //         }
  // //         socket.join(id);
  // //         callback(err, id);
  //       });
  //     });
  //   });

  };


  // // --------------------------------------------------------------
  // Viewer.prototype.post = function(image, data){
  //   console.log('[Controller] Viewer -> post');

  //   data.id = image.id;
  //   this.socketIO.to(image.roomId).emit('post', data);
  // };


  // // --------------------------------------------------------------
  // Viewer.prototype.trigger = function(roomId, data){
  //   console.log('[Controller] Viewer -> trigger');

  //   this.socketIO.to(roomId).emit('trigger', data);
  // };

  return Bridge;
})();

module.exports = controllerInstance = new Bridge();