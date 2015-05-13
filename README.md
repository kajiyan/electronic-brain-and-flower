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
- 1ch  メタルハライドランプ 安定器 1
- 2ch
- 3ch 
- 4ch 
- 5ch  読み上げ用の電球
- 6ch  LED 蛍光灯
- 7ch  LED 蛍光灯
- 8ch  LED 蛍光灯
- 9ch  超音波 噴霧機
- 10ch スポット型電球
- 11ch 汲み上げポンプ
- 12ch サーキュレーター 


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