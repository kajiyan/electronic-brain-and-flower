var fs = require('fs');
var defer = require('config/defer').deferConfig;
var path = require('path');

module.exports = {
  "host": "54.65.140.79",
  "port": 3030,
  "AVAILABLE_PERIOD": 0.25 * 24 * 60 * 60 * 1000,
  "models": path.join(__dirname, '../models/'),
  "views": path.join(__dirname, '../views/'),
  "controllers": path.join(__dirname, '../controllers/'),
  "helpers": path.join(__dirname, '../helpers/'),
  "ntwitter": {
    "consumerKey": "mwcK16ayqNkU6sdJEXQUeyXZH",
    "consumerSecret": "IFVqtnxoQIAtPyRv2q3qFRlETiilOOiYoy4XomZmVejyrGQOig",
    "accessTokenKey": "95014161-DEiLio5xicgrpZm6FKqNFdhgD43hTZ5wAnALhyvpY",
    "accessTokenSecret": "8RkDxkexgqXz9YPBEDNcRJss6Mw1IiwHK1FBBFMnVT3Ta",
  }
};