var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.end('api');
});

router.get('/socket', function(req, res, next) {
  res.end('api');
});


module.exports = router;
