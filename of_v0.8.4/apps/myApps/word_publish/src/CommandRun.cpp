#include "CommandRun.h"


//--------------------------------------------------------------
CommandRun::CommandRun( int ID ) {
    _ID = ID;
    
    // コマンドの実行結果
    _commandStatusCode = -1;
    
    // 音声ファイルを書き出し中かのフラグ
    _isCommandRun = false;
    
    // 設定ファイルを取得する
    _setting.open("config.json");
    
    // OSC センダーのセットアップ
    _ofMainAppSender.setup(
       ofToString(_setting["address"]["openFrameWorks"]["main"]["host"]),
       _setting["address"]["openFrameWorks"]["main"]["port"].asInt()
    );
}

//--------------------------------------------------------------
void CommandRun::setup() {
    cout << "CommandRun-> setup" << "\n";
}

//--------------------------------------------------------------
void CommandRun::update() {
    //cout << "CommandRun-> update" << "\n";
}

//--------------------------------------------------------------
void CommandRun::createSoundFile( string voiseActor, string word, int uniqueID, int fileID ) {
    // 音声ファイルを書き出し中でなければコマンドを実行する
    if( !_isCommandRun ) {
        // 音声ファイルを書き出し中にする
        _isCommandRun = true;
        
        string _voiseActor = voiseActor;
        string _word       = word;
        int    _uniqueID   = uniqueID;
        int    _fileID     = fileID;
        
        // 書きだされるファイルの名前
        string fileName = "word-" + ofToString(_uniqueID) + "-" + ofToString(_fileID) + ".aif";
        
        // コマンドを作成
        // word_publish.sh はbin/word_publish(word_publishDebug)以下の
        // Contents/MacOS 以下に入っている
        // データは word_bridge/bin/data/sounds 以下に生成される
        char shellCommand[1024];
        sprintf(shellCommand, "sh word_publish.sh %s %s %s", _word.c_str(), _voiseActor.c_str(), fileName.c_str());
        
        // コマンドの実行結果を取得する
        _commandStatusCode = system(shellCommand);
        _commandStatusCode = WEXITSTATUS(_commandStatusCode);
        
        if( _commandStatusCode == 0 ){
            // コマンドが正常に終了した場合
            ofxOscMessage sendMessage;
            sendMessage.setAddress( "/publish/word" );
            sendMessage.addIntArg( _fileID );
            sendMessage.addStringArg( fileName );
            _ofMainAppSender.sendMessage( sendMessage );
        }
        
        _isCommandRun = false;
    }
}