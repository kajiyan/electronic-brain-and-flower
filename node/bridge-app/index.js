// node index.js -m DEBUG -i int000000
// node index.js -m PRODUCTION -i int000000


var util = require('util');
var config = require('config');
var io = require('socket.io-client');
var twitter = require('twitter');
var osc = require('osc-min');
var dgram = require('dgram');
var minimist = require('minimist');


// var viewerSocket = io.connect('http://localhost:3030/viewer');
// var clientSocket = io.connect('http://localhost:3030/client');

var viewerSocket = io.connect('http://54.65.140.79:10022/viewer');
var clientSocket = io.connect('http://54.65.140.79:10022/client');


var oscSender = dgram.createSocket("udp4");
// var oscReceiver = dgram.createSocket("udp4", function(message, info){
//   console.log( osc.fromBuffer(message) );
//   console.log( message, info );
// });
// oscReceiver.bind(12001);


// コマンドラインからわたってきた引数
var options = minimist(process.argv.slice(2));

var defaults = {
  m: 'DEBUG', // or PRODUCTION
  i: 'int'
};

for ( var key in options ) {
  if ( options.hasOwnProperty( key ) ) {
    defaults[key] = options[key];
  }
}

var setting = {};
setting.mode = defaults.m;
setting.roomId = defaults.i.replace('int', '');


// ------------------------------------------------------------
// Web Socket
// ------------------------------------------------------------
if(setting.roomId.length > 0){
  viewerSocket.on('connect', function(socket){
    console.log('Bridge App | Socket -> Connect');

    // ROOM が破棄されていたらIDを再設定する必要がある
    viewerSocket.emit('join', {id: setting.roomId}, function(error, roomID){
      if( error ){
        console.log( error );
        return;
      }
    });
  });


  // ファイルが設定された時に受信
  viewerSocket.on('post', function(data){
    console.log('Bridge App | Socket -> _postHandler');

    var buffer = osc.toBuffer({
      address: '/addImage',
      args: [
        {
          type: 'string',
          value: data.url
        },
        {
          type: "string",
          value: data.id
        }
      ]
    });
    
    oscSender.send(buffer, 0, buffer.length, 12002, "localhost");
  });


  // 加速度センサーが閾値を超えた時に受信
  viewerSocket.on( 'trigger', function( data ) {
    console.log( 'Bridge App | Socket -> _triggerHandler' );

    var buffer = osc.toBuffer({
      address: '/showImage',
      args: [
        {
          type: 'string',
          value: data.id
        }
      ]
    });

    oscSender.send(buffer, 0, buffer.length, 12002, "localhost");
  });




  // ------------------------------------------------------------
  // Web Socket -> Client
  // ------------------------------------------------------------
  clientSocket.on('connect', function(socket){
    console.log('Bridge App | Client Socket -> Connect');

    var oscReceiver = dgram.createSocket( "udp4", function( message, info ){
      // console.log(message);
      try {
        message = osc.fromBuffer(message);
        // console.log(message);
        
        // openFrameWorksかMaxかで分岐
        if( typeof message.elements !== 'undefined' && message.elements !== null ){
          message = message.elements[0];
          args = message.args;
        } else {
          args = message.args;
        }

        // 読み上げ用のメッセージを受信した時
        if( message.address == '/add/word' ){
          // OSC で受信した読み上げテキストをWebSocketで送信する
          clientSocket.emit(
            'readText',
            {
              roomId: setting.roomId,
              text: args[0].value
            },
            function( error, roomID ){
              if( error ){
                console.log( error );
                return;
              }
            }
          );
        }

        // 読み上げ用のメッセージの明るさの強度
        if( message.address == '/get/effect/signal' ){
          // OSC で受信した読み上げテキストをWebSocketで送信する
          clientSocket.emit(
            'readLightValue',
            {
              roomId: setting.roomId,
              value: args[0].value
            },
            function( error, roomID ){
              if( error ){
                console.log( error );
                return;
              }
            }
          );
        }

      } catch (error) {
        console.log('error oscReceiver: ' + error);
        return;
      }

    });

    oscReceiver.bind( 12001 );
  });
}


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
  var isSend = false;

  

  twitterClient.stream(
    'statuses/filter',
    {
      track: 'rain,wind,breathe,internet'
      // stall_warnings: false
    },
    function( stream ) {
      stream.on( 'data', function( data ) {
        // console.log(util.inspect(data,false,null));

        try {
          isSend = true;

          if(
            ( typeof data !== 'undefined' && data !== null ) &&
            ( data.user.lang === 'en' || data.user.lang === 'ja' )
          ){

            // if( typeof data.user.lang === 'en' || data.user.lang === 'ja' ){
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
              // console.log(data.text);
              oscSender.send(buffer, 0, buffer.length, 12002, "127.0.0.1");
              isSend = false;
            // } else {
              // isSend = false;
            // }
          } else {
            isSend = false;
          }
        } catch (error) {
          console.log('error twitterGetStream: ' + error);
          
        } finally {
          isSend = false;
        }
      });

      // stream.on('error', function(error) {
      //   console.log('error event');
      //   console.log(error);
      // });
    }
  );
})();


// var twitterClient = new twitter({
//   consumer_key: config.twitter.consumerKey,
//   consumer_secret: config.twitter.consumerSecret,
//   access_token_key: config.twitter.accessTokenKey,
//   access_token_secret: config.twitter.accessTokenSecret
// });

// (function(){
//   var isSend = false;

//   try {
//     twitterClient.stream(
//       'statuses/filter',
//       {
//         track: 'rain,wind,breathe,internet',
//         stall_warnings: false
//       },
//       function( stream ) {
//         stream.on( 'data', function( data ) {
//           isSend = true;

//           if(
//             ( typeof data !== 'undefined' && data !== null ) &&
//             ( typeof data.user.lang !== 'undefined' && data.user.lang !== null )
//           ){

//             // typeof data.user.lang === 'en' || data.user.lang === 'ja'
//             // var buffer = osc.toBuffer({
//             //   address: "/updateStream",
//             //   args: [
//             //     {
//             //       type: "string",
//             //       value: data.user.lang
//             //     },
//             //     {
//             //       type: "string",
//             //       value: data.text
//             //     }
//             //   ]
//             // });
//             // // console.log(data.text);
//             // oscSender.send(buffer, 0, buffer.length, 12002, "127.0.0.1");
//             isSend = false;
//           } else {
//             isSend = false;
//           }
//         });

//         stream.on('error', function(error) {
//           console.log(error);
//         });
//       }
//     );
//   } catch (error) {
//     console.log('error twitterGetStream: ' + error);
//     return;
//   }
// })();






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