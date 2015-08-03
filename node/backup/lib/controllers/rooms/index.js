// ============================================================
// ROOMS
var controllerInstance;
var Rooms;

Rooms = (function() {
  var config = require('config');
  var events = require('events');
  var _ = require('lodash');
  var images = require('./images');
  var imageFile = require('./imageFile');
  var utils = require(config.helpers + 'utils');

  // this を固定する関数
  // 定義された場所の this のプロパティを参照できるようにする
  var __bind = function(fn, me){
    return function(){
      return fn.apply(me, arguments);
    };
  };

  // --------------------------------------------------------------
  function Rooms() {
    console.log('[Controller] Rooms -> constructor');
  }

  // --------------------------------------------------------------
  Rooms.prototype = new events.EventEmitter();

  // --------------------------------------------------------------
  Rooms.prototype.setup = function() {
    console.log('[Controller] Rooms -> setup');
    
    this.app = module.parent.exports;
    this.eve = new events.EventEmitter();
    this.controllers = this.app.get('controllers');
    this.models = this.app.get('models');

    this.images = this.images();

    // _this._create = __bind(_this._create, _this);
    // _this.create = __bind(_this.create, _this);
  };

  // --------------------------------------------------------------
  Rooms.prototype.index = function(offset, limit, callback, options) {
    console.log('[Controller] Rooms -> index');

    // model の取得
    var models = this.app.get('models');
    var roomModel = models.rooms.getModels().Room;

    options = _.extend({
      images: false
    }, options);

    roomModel.count({}, function(err, count){
      if(err){
        callback(err);
        return;
      }

      roomModel
        .find({})
        .sort('-lastModified')
        .skip(offset)
        .limit(limit)
        .select(options.images ? '' : '-images')
        .exec(function(err, rooms){
          if(err){
            callback(err);
            return;
          }
          callback(undefined, rooms, count);
        });
    });
  };

  // --------------------------------------------------------------
  Rooms.prototype._create = function(callback){
    console.log('[Controller] Rooms -> _create');

    // model の取得
    var RoomModel = this.models.rooms.getModels().Room;

    var room = new RoomModel({
      _id: utils.generateID(),
      lastModified: new Date()
    });

    room.save(function(err){
      if(err){
        callback(err);
        return;
      }
      
      imageFile.mkdir(room.id, function(err){
        if(err){
          callback(err);
          return;
        }
        callback(undefined, room.id);
      });
    });
  };

  // --------------------------------------------------------------
  Rooms.prototype.show = function(id, callback, options) {
    console.log('[Controller] Rooms -> show');

    options = _.extend({
      expired: false,
      images: false
    }, options);

    // model の取得
    var models = this.app.get('models');
    var roomModel = models.rooms.getModels().Room;

    roomModel
      .findById(id)
      .where('lastModified').gt(options.expired ? 0 : utils.getExpirationDate(config.AVAILABLE_PERIOD))
      .select(options.images?'':'-images')
      .exec(callback);
  };

  // --------------------------------------------------------------
  Rooms.prototype.update = function(id, callback, options) {
    console.log('[Controller] Rooms -> update');

    options= _.extend({
      images: false
    }, options);
    
    this.show(id, function(err, room){
      if(err){
        callback(err);
        return;
      }
      if(!room){
        callback(new Error("Not Found"));
        return;
      }
      
      room.lastModified= new Date();
      
      room.save(function(err){
        if(err){
          callback(err);
          return;
        }
        callback(undefined, room);
      });
    }, {
      expired: true,
      images: options.images
    });
  };

  // --------------------------------------------------------------
  Rooms.prototype.destroy = function(id, callback) {
    console.log('[Controller] Rooms -> destroy');

    // model の取得
    var models = this.app.get('models');
    var roomModel = models.rooms.getModels().Room;

    roomModel
      .findByIdAndRemove(id)
      .exec(function(err, room){
        if(err){
          callback(err);
          return;
        }
        callback();
        /*
        var ids= _.map(rooms, function(room){
          return room;
        });
        */
        imageFile.rmdir([room.id]);
      });
  };

  // this 固定　--------------------------------------------------------------
  Rooms.prototype.create = function(id, callback) {
    console.log('[Controller] Rooms -> create');

    if(id){
      this.show(id, function(err, room){
        if(err){
          callback(err);
          return;
        }
        if(!room){
          this._create(callback);
          return;
        }
        callback(undefined, id);
      });
    } else {
      this._create(callback);
    }
  };


  // --------------------------------------------------------------
  // IMAGES
  Rooms.prototype.images = function() {
    var _this = this;

    return {
      show: function(id, callback){
        console.log('[Controller] Rooms -> Images -> show');

        var models = _this.app.get('models');
        var roomModel = models.rooms.getModels().Room;

        roomModel
          .findById(id)
          .where('lastModified')
          .gt(utils.getExpirationDate(config.AVAILABLE_PERIOD))
          .exec(function(err, room){
            console.log(room);

            if(err){
              callback(err);
              return;
            }
            callback(undefined, room.images);
          });
      },
      update: function(roomId, data, callback){
        console.log('[Controller] Rooms -> Images -> update');
        // console.log(roomId, data, callback);
        // console.log("---------");

        // model の取得
        _this.show(roomId, function(err, room){
          if(err){
            callback(err);
            return;
          }
          if(!room){
            callback(new Error("Not Found"));
            return;
          }

          console.log("---------");
          console.log(room);
          console.log("---------");

          var parsedData = utils.parseDataURL(data.data);
          var image = room.images.create({
            width: data.width,
            height: data.height,
            roomId: roomId,
            userInfo: data.userInfo
          });
            
          image.name = image.id + '.' + parsedData.ext;
          room.images.push(image);
          room.numOfImages++;
          room.lastModified = new Date();
          
          room.save(function(err){
            if(err){
              callback(err);
              return;
            }
            callback(undefined, image);

            imageFile.save(roomId, image.name, parsedData.data);
          });
        }, {
          images: true
        });
      },
      destroy: function(id, callback) {
        console.log('[Controller] Rooms -> Images -> destroy');

        _this.show(id, function(err, room){
          if(err){
            callback(err);
            return;
          }
          if(!room){
            callback(new Error("Not Found"));
            return;
          }
          
          room.images= [];
          room.numOfImages= 0;
          
          room.save(function(err){
            if(err){
              callback(err);
              return;
            }
            
            imageFile.remove(id, function(err){
              if(err){
                callback(err);
              }
              callback();
            });
          });
        }, {
          images: true
        });
      }
    };
  };

  return Rooms;
})();

module.exports = controllerInstance = new Rooms();