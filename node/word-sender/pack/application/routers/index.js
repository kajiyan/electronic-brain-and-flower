var express = require('express');
var router = express.Router();

var config = require('config');
var _ = require('lodash');
// var http = require('http');

/* GET home page. */
router.get('/', function(req, res, next) {
  var app = module.parent.exports;

  var controllers = app.get('controllers');
  var models = app.get('models');

  // GET 値
  var GET = req.params;

  var key = 'roomid';
  var valid = true;
  var roomId = '';

  // オブジェクトのキーを要素にもった配列を作り、
  // 配列に指定した値が含まれているかどうかを確認する
  // GET の値に roomId が存在するか
  if(_.contains(_.keys(GET), key)) {
    var _roomId = GET[key];
    console.log("TEST");
    
  } else {
    console.log("NOT ROOM ID");

    var keyData = {
      'roomId': roomId
    };
    res.render('index.swig.html', keyData);
  }
});

module.exports = router;
