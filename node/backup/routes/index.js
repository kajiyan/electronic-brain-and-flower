var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.end('api');
});

router.get('/client', function(req, res, next) {
  console.log('Router | client -> GET:Index');

  var keyData = {
    'roomid': ''
  };

  res.render('client/index.swig.html', keyData);
});


module.exports = router;
