var config = require('config');
var dgram = require('dgram');
var twitter = require('twitter');
var osc = require('osc-min');


var udp = dgram.createSocket("udp4");

var twitterClient = new twitter({
  consumer_key: config.twitter.consumerKey,
  consumer_secret: config.twitter.consumerSecret,
  access_token_key: config.twitter.accessTokenKey,
  access_token_secret: config.twitter.accessTokenSecret
});

var buf;

twitterClient.stream('statuses/filter', {track: 'javascript'}, function(stream) {
  stream.on('data', function(data) {
    buf = osc.toBuffer({
      address: "/word",
      args: [
        {
          type: "string",
          value: data.text
        }
      ]
    });
    console.log(data.text);
    udp.send(buf, 0, buf.length, 12002, "localhost");
  });
 
  stream.on('error', function(error) {
    throw error;
  });
});