# 電脳と花 仕様書

## Node
### WebSocket Bridge App
####【/addImage】
Webサーバーに画像データが追加されたタイミングで発火する  
保存された画像のURLをOSCで発行する

##### -  ■ Receive Port
12002

##### -  ■ Receive Value
address: /addImage  
value: http://54.65.140.79/images/api/{ ROOM ID }/{ IMAGE ID }.{ jpg|gif|png }



## OpenFrameWorks



## Max/Msp