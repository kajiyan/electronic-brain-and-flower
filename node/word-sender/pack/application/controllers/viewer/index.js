// ============================================================
// VIEWER
var controllerInstance;
var Viewer;

module.exports = Viewer = (function() {
  var config = require('config');
  var events = require('events');


  // --------------------------------------------------------------
  function Viewer() {
    console.log('[Controller] Viewer -> Constructor');

    this.NAMESPACE = 'viewer';
    this.eve = new events.EventEmitter();
    this.socketIO = null;
  }


  // --------------------------------------------------------------
  Viewer.prototype = new events.EventEmitter();


  // --------------------------------------------------------------
  Viewer.prototype.setup = function() {
    console.log('[Controller] Viewer -> Setup');

    var app = module.parent.exports;
      
    var controllers = app.get('controllers');
    var eve = controllers.viewer.eve;

    this.socketIO = app.get('socketIO').of(controllers.viewer.NAMESPACE);

    this.socketIO.on('connection', function(socket) {
      console.log('[Controller] Viewer -> socketIO -> connection');

      socket.on('join', function(data, callback) {
        console.log('[Controller] Viewer -> socketIO -> connection -> join');

        eve.emit('join', data.id, function(err, id) {
          if(err){
            callback(err);
            return;
          }
          socket.join(id);
          callback(err, id);
        });

      });
    });

    /*
     * eventEmitter
     */
    eve.on('join', function(roomId, callback){
      console.log('[Controller] Viewer -> eventEmitter -> join');
      controllers.rooms.create(roomId, function(err, rId){
        // console.log(rId);
        callback(err, rId);
      });
    });
  };


  // --------------------------------------------------------------
  Viewer.prototype.post = function(image, data){
    console.log('[Controller] Viewer -> post');

    data.id = image.id;
    this.socketIO.to(image.roomId).emit('post', data);
  };


  // --------------------------------------------------------------
  Viewer.prototype.trigger = function(roomId, data){
    console.log('[Controller] Viewer -> trigger');

    this.socketIO.to(roomId).emit('trigger', data);
  };

  return Viewer;
})();

module.exports = controllerInstance = new Viewer();