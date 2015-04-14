var express = require('express');
var router = express.Router();

var config = require('config');

var dgram = require('dgram');

var ntwitter = require('ntwitter');
var osc = require('osc-min');

/* GET home page. */
router.get('/', function(req, res, next) {
  var app = module.parent.exports;

  // UDP
  var udp = dgram.createSocket("udp4")

  // ntwitter のインスタンスを作る
  var twitter = new ntwitter({
    consumer_key: config.ntwitter.consumerKey,
    consumer_secret: config.ntwitter.consumerSecret,
    access_token_key: config.ntwitter.accessTokenKey,
    access_token_secret: config.ntwitter.accessTokenSecret
  });

  twitter.stream('statuses/filter', {'track':'test'}, function(stream) {
    stream.on('data', function (data) {
      console.log(data);
    });

    setTimeout(stream.destroy, 5000);
  });


  var sendHeartbeat = function() {
    var buf;
    buf = osc.toBuffer({
      address: "/heartbeat",
      args: [
        12,
        "sttttring",
        new Buffer("beat"),
        {
          type: "integer",
          value: 7
        }
      ]
    });
    console.log("SEND!!");
    return udp.send(buf, 0, buf.length, 12345, "localhost");
  };
 
  setInterval(sendHeartbeat, 10000);

  res.render('api/index.swig.html', { title: 'Express' });
});

module.exports = router;
