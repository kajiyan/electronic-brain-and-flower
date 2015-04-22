var config = require('config');
var io = require('socket.io-client');
var twitter = require('twitter');
var osc = require('osc-min');
var dgram = require('dgram');


var viewerSocket = io.connect('http://localhost:3030/viewer');
var oscSender = dgram.createSocket("udp4");


// ------------------------------------------------------------
// Web Socket
// ------------------------------------------------------------
viewerSocket.on('connect', function(socket){
  console.log('Bridge App | Socket -> Connect');

  // ROOM が破棄されていたらIDを再設定する必要がある
  viewerSocket.emit('join', {id: '463295'}, function(err, roomID){
    if( error ){
      console.log( error );
      return;
    }
  });
});


viewerSocket.on('post', function(data){
  console.log('Bridge App | Socket -> _postHandler');

  var buffer = osc.toBuffer({
    address: '/addImage',
    args: [
      {
        type: 'string',
        value: data.url
      }
    ]
  });
  oscSender.send(buffer, 0, buffer.length, 12002, "localhost");
});


// ------------------------------------------------------------
// TWITTER
// ------------------------------------------------------------
var twitterClient = new twitter({
  consumer_key: config.twitter.consumerKey,
  consumer_secret: config.twitter.consumerSecret,
  access_token_key: config.twitter.accessTokenKey,
  access_token_secret: config.twitter.accessTokenSecret
});

(function(){
  try {
    twitterClient.stream(
      'statuses/filter',
      { track: 'rain,wind,breathe,internet' },
      function( stream ) {
        stream.on( 'data', function( data ) {
          if( data.user.lang === 'en' || data.user.lang === 'ja' ){
            var buffer = osc.toBuffer({
              address: "/updateStream",
              args: [
                {
                  type: "string",
                  value: data.user.lang
                },
                {
                  type: "string",
                  value: data.text
                }
              ]
            });
            oscSender.send(buffer, 0, buffer.length, 12002, "127.0.0.1");
          }
        });

        stream.on('error', function(error) {
          console.log(error);
        });
      }
    );
  } catch (error) {
    console.log('error twitterGetStream: ' + error);
    return;
  }
})();



// // ------------------------------------------------------------
// // OSC RECEIVER
// // ------------------------------------------------------------
// // ------------------------------------------------------------
// // From Max
// var oscReceiverMaxMsp = dgram.createSocket( 'udp4', function( data, receiveInfo ) {
//   /**
//   * Twitter Stream APIに渡すキーワードを返す
//   *
//   * @access  private
//   * @param   Int
//   * @return  String
//   *
//   */
//   var getSceneWord = function( key ) {
//     var result = '';
//     switch ( key ) {
//       case 1:
//         result = 'rain';
//         break;
//       case 2:
//         result = 'wind';
//         break;
//       case 3:
//         result = 'breathe';
//         break;
//       case 4:
//         result = 'internet';
//         break;
//       default:
//         result = 'rain';
//         break;
//     }
//     return result;
//   };

//   try {
//     // バイナリからJSON形式に変換
//     data = osc.fromBuffer(data);
//     var address = data.address;

//     // 受信したアドレスによって処理を振り分ける
//     if ( address === '/updateScene' ) {
//       console.log('Bridge App -> OSC RECEIVE -> /updateScene');

//       // メインの音楽ファイルのシーンを受け取る
//       var values = data.args,
//           scene  = values[0].value;

//       // Twitter Stream API をアップデート
//       var keyData = {
//         keyWord: getSceneWord(scene)
//       };
//       twitterUpdateStream(keyData);

//       console.log(getSceneWord(scene));
//     }
//   } catch (error) {
//     console.log('error oscReceiverMaxMsp: ' + error);
//     return;
//   }
// });

// oscReceiverMaxMsp.bind( 12001 );