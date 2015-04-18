var express = require('express');
var router = express.Router();

var config = require('config');
var _ = require('lodash');


// ------------------------------------------------------------
router.get('/', function(req, res, next) {
  console.log('Router -> Client[GET] -> Index');

  var keyData = {
    'roomid': ''
  };

  res.render('client/index.swig.html', keyData);
});


// ------------------------------------------------------------
router.get('/:roomid', function(req, res, next) {
  console.log('Router -> Client[GET] -> Index:roomid');

  var app = module.parent.exports;
  var controllers = app.get('controllers');
  var models = app.get('models');

  // GET 値
  var GET = req.params;
  // console.log(GET);

  var key = 'roomid';
  var valid = true;
  var roomid = '';

  var keyData = {
    'roomid': roomid
  };

  // オブジェクトのキーを要素にもった配列を作り、
  // 配列に指定した値が含まれているかどうかを確認する
  // GET の値に roomId が存在するか
  if(_.contains(_.keys(GET), key)) {
    // roomId があればビューに反映する
    var keyData = {
      'roomid': GET[key]
    };

    res.render('client/index.swig.html', keyData);
  } else {
    console.log("NOT ROOM ID");

    // エラー時の処理
  }
  
});

module.exports = router;