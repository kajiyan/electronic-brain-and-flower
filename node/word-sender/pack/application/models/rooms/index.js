// ============================================================
// ROOMS MODEL
var modelInstance;
var RoomsModel;

RoomsModel = (function() {
  var config = require('config');
  var mongoose = require('mongoose');
  var utils = require(config.helpers + 'utils');

  
  // --------------------------------------------------------------
  function RoomsModel() {
    console.log('[Model] Rooms -> constructor');

    /*
     * DataBase
     */
    mongoose.connect('mongodb://' + config.host + '/addressbook');

    /*
     * DataBase Schemas
     */
    var ImageSchema = new mongoose.Schema({
      width: Number,
      height: Number,
      roomId: String,
      userInfo: String,
      name: String
    }, {
      toJSON: {
        virtuals: true
      }
    });

    ImageSchema.virtual('url').get(function(){
      return '//api.' + config.host + '/images/' + this.roomId + '/' + this.name;
    });

    var RoomSchema = new mongoose.Schema({
      _id: String,
      images: [ImageSchema],
      numOfImages: {
        type: Number,
        default: 0
      },
      lastModified: {
        type: Date,
        default: new Date()
      }
    }, {
      _id: false,
      toJSON: {
        virtuals: true
      }
    });

    RoomSchema.virtual('expired').get(function(){
      return this.lastModified < utils.getExpirationDate(config.AVAILABLE_PERIOD);
    });

    mongoose.model('Room', RoomSchema);

    this.Room = mongoose.model('Room');
  }

  // --------------------------------------------------------------
  RoomsModel.prototype.getModels = function() {
    console.log('[Model] Rooms -> getModels');

    var result = {
      'Room': this.Room
    };

    return result;
  };

  return RoomsModel;
})();


module.exports = modelInstance = new RoomsModel();