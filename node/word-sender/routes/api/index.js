var express = require('express');
var router = express.Router();

var config = require('config');
var ntwitter = require('ntwitter');

/* GET home page. */
router.get('/', function(req, res, next) {
  var app = module.parent.exports;

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

  res.render('api/index.swig.html', { title: 'Express' });
});

module.exports = router;
