var express = require('express');
var router = express.Router();

var config = require('config');
var _ = require('lodash');


// ------------------------------------------------------------
router.get('/', function(req, res, next) {
  console.log('Router -> Bridge[GET] -> Index');

  res.render('bridge/index.swig.html');
});

module.exports = router;