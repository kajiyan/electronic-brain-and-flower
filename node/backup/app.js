var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');

var config = require('config');
var swig = require('swig');

var app = express();


// view engine setup
app.engine('html', swig.renderFile);
app.set('view cache', true);
swig.setDefaults({ cache: false });
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'swig');

// uncomment after placing your favicon in /public
app.use(favicon(__dirname + '/public/images/favicon.ico'));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));


/* ============================================================
 * CONTROLLER
 *
 * client ユーザーの端末からの情報を処理する
 *
 ============================================================ */
var controllers = {
    'client': require('./controllers/client/')
};

app.set('controllers', controllers);


// ============================================================
// ROUTES
var router = {
  'index': require('./routes/'),
  'api': require('./routes/api/')
};

app.use('/api', router.api);
app.use('/', router.index);


// catch 404 and forward to error handler
app.use(function(req, res, next) {
    var err = new Error('Not Found');
    err.status = 404;
    next(err);
});

// error handlers

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
    app.use(function(err, req, res, next) {
        res.status(err.status || 500);
        res.render('error.swig.html', {
            message: err.message,
            error: err
        });
    });
}

// production error handler
// no stacktraces leaked to user
app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error.swig.html', {
        message: err.message,
        error: {}
    });
});


module.exports = app;
