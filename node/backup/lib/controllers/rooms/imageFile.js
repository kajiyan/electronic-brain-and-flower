var ImageFile = (function() {
  var events = require('events');
  var fs = require('fs');
  var path = require('path');
  var childProcess = require('child_process');
  var _ = require('lodash');

  var BASE_PATH = path.join(__dirname, '../../public/images/api');

  // --------------------------------------------------------------
  function ImageFile() {
    // constructor
    console.log('ImageFile -> Constructor');

    this.eve = new events.EventEmitter();
  }

  // --------------------------------------------------------------
  ImageFile.prototype.mkdir = function(roomId, callback) {
    console.log('ImageFile -> mkdir');

    fs.mkdir(path.join(BASE_PATH, roomId), function(err){
      if(err){
        callback(err);
        return;
      }
      callback();
    });
  };

  // --------------------------------------------------------------
  ImageFile.prototype.save = function(roomId, fileName, data) {
    console.log('ImageFile -> save');
    console.log(roomId, fileName, data);
    console.log(path.join(BASE_PATH, roomId.toString(), fileName));
    console.log("---------");

    fs.writeFile(path.join(BASE_PATH, roomId.toString(), fileName), data, function(err){
    });
  };

  // --------------------------------------------------------------
  ImageFile.prototype.remove = function(roomId, callback) {
    console.log('ImageFile -> remove');

    childProcess.exec('rm ' + path.join(BASE_PATH, roomId, '*'), function(error, stdout, stderr){
      callback(error);
    });
  };

  // --------------------------------------------------------------
  ImageFile.prototype.rmdir = function(roomId) {
    console.log('ImageFile -> rmdir');

    var paths = _.map(roomIds, function(roomId){
      return path.join(BASE_PATH, roomId);
    });

    childProcess.exec('rm -rf ' + paths.join(' '), function(error, stdout, stderr){
    });
  };

  return ImageFile;
})();

module.exports = new ImageFile();