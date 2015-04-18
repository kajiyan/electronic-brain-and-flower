var express = require('express');
var router = express.Router();


// ------------------------------------------------------------
router.get('/', function(req, res, next) {
  console.log('Router -> Rooms[GET] -> Index');

  var app = module.parent.exports;
  var controllers = app.get('controllers');

  var offset = req.query.offset || 0;
  var limit = req.query.limit || 100;

  controllers.rooms.index(offset, limit, function(err, rooms, count){
    console.log(err, rooms, count);

    if(err){
      res.json(400, err);
      return;
    }
    if(!rooms){
      res.json(404, {});
      return;
    }
    res.json({
      'total': count,
      'results': rooms
    });
  }, {
    images: !!req.query.images || false
  });
});


// ------------------------------------------------------------
router.get('/:id', function(req, res, next) {
  console.log('Router -> Rooms[GET] -> Index:id');

  var app = module.parent.exports;
  var controllers = app.get('controllers');

  controllers.rooms.show(req.params.id, function(err, room){
    if(err){
      res.json(400, err);
      return;
    }
    if(!room){
      res.json(404, {});
      return;
    }
    res.json(room);
  }, {
    images: !!req.query.images || false
  });
});


// ------------------------------------------------------------
router.put('/:id', function(req, res, next) {
  console.log('Router -> Rooms[PUT] -> Index:id');

  var app = module.parent.exports;
  var controllers = app.get('controllers');

  controllers.rooms.update(req.params.id, function(err, room){
    if(err){
      res.json(400, err);
      return;
    }
    if(!room){
      res.json(404, {});
      return;
    }
    res.json(room);
  }, {
    images: !!req.query.images || false
  });
});


// ------------------------------------------------------------
router.delete('/:id', function(req, res, next) {
  console.log('Router -> Rooms[DELETE] -> Index:id');

  var app = module.parent.exports;
  var controllers = app.get('controllers');

  controllers.rooms.destroy(req.params.id, function(err){
    if(err){
      res.json(400, err);
      return;
    }
    res.json({});
  });
});


// ------------------------------------------------------------
router.get('/:id/images', function(req, res){
  console.log('Router -> Rooms[GET] -> Images:id');

  var app = module.parent.exports;
  var controllers = app.get('controllers');

  controllers.rooms.images.show(req.params.id, function(err, images){
    if(err){
      res.json(400, err);
      return;
    }
    if(!images){
      res.json(404, {});
      return;
    }
    res.json({
      'results': images
    });
  });
});


// ------------------------------------------------------------
router.delete('/:id/images', function(req, res){
  console.log('Router -> Rooms[DELETE] -> Images:id');
});


module.exports = router;
