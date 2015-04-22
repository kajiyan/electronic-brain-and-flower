# 電脳と花 仕様書

## Node
### Bridge App

### SEND 系
####【/addImage】
Webサーバーに画像データが追加されたタイミングで発信される。  
保存された画像のURLをOSCで発信する。以下のポートで受信可能。    

##### ■ Receive Port
- 12002

##### ■ Receive Value
- address: /addImage  
- value: (1)[String] http://54.65.140.79/images/api/{ ROOM ID }/{ IMAGE ID }.{ jpg|gif|png }   
保存された画像のURL


####【/updateStream】
Twitterのタイムラインがアップデートされたタイミングで発信される。  
取得したTweet のテキストと言語設定をOSCで発信する。以下のポートで受信可能。　　

##### ■ Receive Port
- 12002

##### ■ Receive Value
- address: /updateStream  
- value: (1)[String] 本文 Tweet Body  
- value: (2)[String] 言語設定 { en | ja }


### RECEIVE 系 
port 12001に発信されたOSCを監視する。

##### ■【/updateScene】（from MAX/MSP）
- value: (1)[int] 0  

受信した値をキーにしてTwitter Stream API の  
取得パラメータを更新する。


---
## OpenFrameWorks
### Main App

### SEND 系


####【/add/word】（to OpenFrameWorks Sub App）
【/updateScene】で受信した本文を正規表現で無害化、  
言語設定を元にボイスアクターを選出してにOSCを発信する。  
ただし、OpenFrameWorks Sub App から【/publish/word】が返ってこない限り  
再度発信されることはない。以下のポートで受信可能。

##### ■ Receive Port
- 12003

##### ■ Receive Value
- address: /add/word    
- value: (1)[String] { Kyoko | Victoria } ボイスアクター  
- value: (2)[String] 読み上げるテキスト  
- value: (3)[Int] { 0 | 1 } ファイル名をユニークにするためのID  
- value: (4)[Int] ファイルのID  


####【/play/effect】（to Max/Msp）
【/updateScene】で受信した本文に  
特定のキーワードが入っていると判別されたタイミングで発信される。    
ただし、Max/Msp から【/effect/complete】が返ってこない限り  
再度発信されることはない。以下のポートで受信可能。　　

##### ■ Receive Port
- 12004

##### ■ Receive Value
- address: /play/effect  
- value: (1)[String] WordSource のインスタンスID  



####【/play/word】（to Max/Msp）
【/publish/word】で音声ファイルのパブリッシュが完了したことを検知して  
Max/Mspに再生対象のファイルについてOSCを発信する。以下のポートで受信可能。  
ただし、Max/Msp から【/complete/word】が返ってこない限り  
再度発信されることはない。以下のポートで受信可能。

##### ■ Receive Port
- 12004

##### ■ Receive Value
- address: /play/word  
- value: (1)[String] WordSource のインスタンスID   
- value: (2)[String] 音声ファイル名  


### RECEIVE 系 
port 12002に発信されたOSCを監視する。  

##### ■【/updateStream】（from Node Bridge App）
- value: (1)[String] 本文 Tweet Body  
- value: (2)[String] 言語設定 { en | ja } 

受信した本文に特定のキーワードが入っているか判別して  
入っていればMax/Msp にOSCを発信する。  
正規表現で本文を無害化して OpenFrameWorks Sub App にOSCを送信する。


##### ■【/publish/word】（from OpenFrameWorks Sub App）
- value: (1)[Int] \{ 0 , 1 \} シェルが正常に終了したかのステータス  
- value: (2)[Int] \{ 0 , 1 \} ファイル名をユニークにするためのID  
- value: (3)[String] word-\{ 0 , 1 \}-{ int }.aif 書きだされたファイル名


##### ■【/complete/effect】（from Max/Msp）
- value: (1)[Int] 再生が完了した音声クラスのID

受信したIDと同じクラスをOSC発信可能状態に戻す。


##### ■【/complete/word】（from Max/Msp）
- value: (1)[Int] 再生が完了した音声クラスのID

受信したIDと同じクラスをOSC発信可能状態に戻す。



---
### Sub App

### SEND 系
####【/publish/word】（to OpenFrameWorks Main App）
say コマンドで音声ファイルの書き出しが完了したタイミングで発信される。  
以下のポートで受信可能。  

##### ■ Receive Port
12002

##### ■ Receive Value
- address: /publish/word  
- value: (1)[Int] { 0 | 1 } シェルが正常に終了したかのステータス  
- value: (2)[Int] { 0 | 1 } ファイル名をユニークにするためのID  
- value: (3)[String] word-{ 0 | 1 }-{ int }.aif 書きだされたファイル名

### RECEIVE 系 
port 12003に発信されたOSCを監視する。  
##### ■【/add/word】（from OpenFrameWorks Main App）
- value: (1)[String] { Kyoko | Victoria } ボイスアクター  
- value: (2)[String] 読み上げるテキスト   
- value: (3)[Int] { 0 | 1 } ファイル名をユニークにするためのID    
- value: (4)[Int] ファイルのID  

受信した値をキーにしてTwitter Stream API の
取得パラメータを更新する。



---
## Max/Msp
### SEND 系

####【/updateScene】（to Node Bridge App）
メインの音楽のタイムラインが変化したタイミングで発信される。  
タイムラインの状態をOSCで発信する。以下のポートで受信可能。  

##### ■ Receive Port
- 12001
##### ■ Receive Value
- address: /updateScene  
- value: (1)[int] 0 


####【/complete/effec】（to OpenFrameWorks Main App）
効果音の再生が完了したタイミングで発信される。  
以下のポートで受信可能。
##### ■ Receive Port
- 12002
##### ■ Receive Value
- address: /complete/effec  
- value: (1)[String] 再生が完了した音声クラスのID  


####【/complete/word】（to OpenFrameWorks Main App）
音声の再生が完了したタイミングで発信される。  
以下のポートで受信可能。
##### ■ Receive Port
- 12002
##### ■ Receive Value
- address: /complete/word  
- value: (1)[String] 再生が完了した音声クラスのID    


### RECEIVE 系 
port 12004に発信されたOSCを監視する。  

##### ■【/play/effect】（from OpenFrameWorks Main App）
- value: (1)[String] WordSource のインスタンスID  

受信した値を元に効果音を再生する。

##### ■【/play/word】（from OpenFrameWorks Main App）
- value: (1)[String] WordSource のインスタンスID  
- value: (2)[String] 音声ファイル名  

受信した値を元に音声ファイルを再生する。


