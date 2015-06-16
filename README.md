# 電脳と花 electronic-brain-and-flower

[各APIについて](https://github.com/kajiyan/electronic-brain-and-flower/blob/master/docs/api.md)  
[Twitter App - 電脳と花](https://apps.twitter.com/app/8181167 "Twitter App")


#### SERVER
amazon EC2  
[54.65.140.79](http://54.65.140.79)  


接続にはvagrantを使用する。  
vagrantの設定ファイルは開発用クラウドのhtdocs に入っている。  
htdocs/chaos1/Vagrantfile


秘密鍵は開発用クラウドの/AWS/chaos1 に入っている。  
/AWS/chaos1


sshする際はデバイスの環境に合わせてVagrantfile の秘密鍵までのパスを  
適切に設定する必要がある。


node 関連のファイルは下記のディレクトリにアップする。  
/usr/share/nginx/node/ebaf/


EC2のSecurity Groups はNodeの実行環境に合わせて適切に設定する。


#### OSC RECEIVE PORT
- Node                  : 12001
- OpenFrameWorks [Main] : 12002
- OpenFrameWorks [Sub]  : 12003
- Max/Msp               : 12004


#### DMX CHANNEL
- 1ch  読み上げ用のスポット型電球（effect）
- 2ch  送風機（effect）
- 3ch  
- 4ch  
- 5ch  超音波 噴霧機
- 6ch  スポット型電球
- 7ch  サーキュレーター
- 8ch  
- 9ch  汲み上げポンプ・汲み上げポンプ
- 10ch 汲み上げポンプ・汲み上げポンプ
- 11ch 
- 12ch 
- 13ch LED 蛍光灯 Left(led_left) 
- 14ch LED 蛍光灯 Right(led_right)
- 15ch LED 蛍光灯 Center(led_center)
- 16ch  
- 17ch メタルハライドランプ 安定器
- 18ch 
- 19ch 
- 20ch 


## Node
- Server PORT: 3030
- Front End DEBUG PORT: 3031

- Streaming APIでリアルタイムにTweetを取得する。


## Open Frameworks


### word-bridge App

- node.jsでリアルタイムに取得したTweetをOSC経由で受け取る。
- 正規表現で該当のキーワードを含むか選別する。
- say コマンドで音声ファイルを書き出す。


#### Depends
word-bridgeは以下のアドオン、ファイルに依存する。

- ofxOSC
- [ofxJSON](https://github.com/jefftimesten/ofxJSON "ofxJSON")
- bin/data/config.json


## Max/Msp
- openFrameWorks からOSCを受け取る。
- サウンドファイルを加工して再生する。
- サウンドシグナルに合わせてArt-Net経由でDMXを送る。

#### Depends
electronic-brain-and-flower.maxpat は以下のアドオン、ファイルに依存する。

- [IMP.DMX](http://www.theimpersonalstereo.com/software/imp-dmx/ "IMP.DMX")