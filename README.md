# electronic-brain-and-flower
電脳と花

[Twitter App - 電脳と花](https://apps.twitter.com/app/8181167 "Twitter App")


### OSC RECEIVE PORT
- Node                  : 100001
- OpenFrameWorks [Main] : 100002
- OpenFrameWorks [Sub]  : 100003
- Max/Msp               : 100004


### Node
・Streaming APIでリアルタイムにTweetを取得する。


### Open Frameworks
word-bridge
・node.jsでリアルタイムに取得したTweetをOSC経由で受け取る。
・正規表現で該当のキーワードを含むか選別する。
・say コマンドで音声ファイルを書き出す。