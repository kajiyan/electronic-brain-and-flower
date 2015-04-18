var express = require('express');
var router = express.Router();

var config = require('config');

var dgram = require('dgram');

var twitter = require('twitter');
var osc = require('osc-min');

/* GET home page. */
router.get('/', function(req, res, next) {
  var app = module.parent.exports;

  // UDP
  var udp = dgram.createSocket("udp4");

  // twitter のインスタンスを作る
  var twitterInstance = new twitter({
    consumer_key: config.ntwitter.consumerKey,
    consumer_secret: config.ntwitter.consumerSecret,
    access_token_key: config.ntwitter.accessTokenKey,
    access_token_secret: config.ntwitter.accessTokenSecret
  });

  // twitter stream API
  twitterInstance.stream('statuses/filter', {track: 'javascript'}, function(stream) {
    stream.on('data', function(data) {
      console.log(data.text);

      var buffer;
      buffer = osc.toBuffer({
        address: "/word",
        args: [{
          type: "string",
          value: data.text
        }]
      });
      return udp.send(buffer, 0, buffer.length, 12345, "localhost");
    });

    stream.on('error', function(error) {
      throw error;
    });
  });

  // var sendHeartbeat = function() {
  //   var buffer;
  //   buffer = osc.toBuffer({
  //     address: "/heartbeat",
  //     args: [
  //       12,
  //       "sttttring",
  //       new Buffer("beat"),
  //       {
  //         type: "integer",
  //         value: 7
  //       }
  //     ]
  //   });
  //   console.log("SEND!!");
  //   return udp.send(buffer, 0, buffer.length, 12345, "localhost");
  // };
 
  // setInterval(sendHeartbeat, 10000);

  res.render('api/index.swig.html', { title: 'Express' });
});

module.exports = router;
