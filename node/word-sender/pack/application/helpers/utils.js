var Utils = (function() {
  var events = require('events'); // イベントドリブンを使用する
  var _ = require('lodash');

  function Utils() {
    // constructor
    console.log('Utils -> Constructor');

    this.eve = new events.EventEmitter();
  }

  // --------------------------------------------------------------
  Utils.prototype.generateID = function() {
    console.log('Utils -> generateID');

    var src = '0123456789'.split('');
    var dst = _.sample(src, 6);
    return dst.join('');
  };

  // --------------------------------------------------------------
  Utils.prototype.getExpirationDate = function(period) {
    console.log('Utils -> getExpirationDate');

    var date = new Date();
    date.setTime(date.getTime() - period);
    return date;
  };

  // --------------------------------------------------------------
  Utils.prototype.parseDataURL = function(string) {
    console.log('Utils -> parseDataURL');
    
    if(/^data:.+\/(.+);base64,(.*)$/.test(string)) {
      return {
        ext: RegExp.$1,
        data: new Buffer(RegExp.$2, 'base64')
      };
    }
    return;
  };

  return Utils;
})();

module.exports = new Utils();