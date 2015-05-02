#include "WordSource.h"

#include "Poco/String.h"
#include "Poco/RegularExpression.h"

using Poco::replace;
using Poco::RegularExpression;

//--------------------------------------------------------------
WordSource::WordSource( int ID ) {
    cout << "WordSource -> constructor" << ID << "\n";
    
    // 設定ファイルを取得する
    _setting.open("config.json");
    
    _ID = ID;
    _uniqueID = 0; // ファイル名を一意にするもの
    
    _isDebug = true;
    
    _isGetNewWord = true;       // 新しいワードを取得できるかのフラグ
    _isPublishProgress = false; // 音声ファイルの書き出し中かのフラグ
    
    //_isPublishWord = false;   // 音声ファイルの書き出しが終了したかのフラグ
    //_isCompletePlay = false;  // 音声ファイルの読み上げが終了したかのフラグ
}

//--------------------------------------------------------------
void WordSource::setup() {
    cout << "WordSource -> setup\n";
    
    // OF Sub App 用のOSC Senderのセットアップ
    _ofSubAppSender.setup(
        _setting["address"]["openFrameWorks"]["sub"]["host"].asString(),
        _setting["address"]["openFrameWorks"]["sub"]["port"].asInt()
    );
    
    // Max 用のOSC Senderのセットアップ
    _maxSender.setup(
        _setting["address"]["max"]["host"].asString(),
        _setting["address"]["max"]["port"].asInt()
    );
}


//--------------------------------------------------------------
void WordSource::update() {
    // 新しいテキストが設定されてあり、音声書き出し中でない状態であれば
    // Sub App にOSCでテキスト情報を送る
    if( !_isGetNewWord && !_isPublishProgress ) {
        _isPublishProgress = true;
        
        // 言語情報を元にアクターを設定する
        string voiseActor;
        if ( _language == "en" ) {
            voiseActor = "Victoria";
        } else if ( _language == "ja" ) {
            voiseActor = "Kyoko";
        }
        
        // ファイル名を一意にするIDをアップデート
        _uniqueID = ( ( _uniqueID + 1 ) % 2 + 2 ) % 2;
        cout << "uniqueID:" << _uniqueID << "\n";
        
        // デバッグ用のテキストを設定
        _debugString = "WordSource ID" + ofToString( _ID ) + " -> /add/word \n" +
        "actor: " + ofToString( voiseActor ) + "\n" +
        "text: " + ofToString( _text ) + "\n" +
        "uniqueID: " + ofToString( _uniqueID ) + "\n"
        "ID: " + ofToString( _ID ) + "\n";
        
        ofxOscMessage sendMessage;
        sendMessage.setAddress( "/add/word" );
        sendMessage.addStringArg( voiseActor );
        sendMessage.addStringArg( "\"" + _text + "\"" );
        sendMessage.addIntArg( _uniqueID );
        sendMessage.addIntArg( _ID );
        _ofSubAppSender.sendMessage( sendMessage );
    }
}


//--------------------------------------------------------------
void WordSource::draw() {
    if ( _isDebug ) {
        ofPushStyle();
        
        ofSetColor(ofColor(255, 255, 255));
        ofDrawBitmapString(_debugString, 20, 100 * _ID + 100);
        
        ofPopStyle();
    }
}





/* --------------------------------------------------------------
 /add/word でof Sub Appに送信するデータをセットする
 
 @access	public
 @param	    none
 @return	none
 --------------------------------------------------------------  */
void WordSource::setWord( string language, string text ) {
    if ( _isGetNewWord ) {
        // 新しいテキスト情報を設定できない状態にする
        _isGetNewWord = false;
        _language = _wordValidation( language );
        _text = _wordValidation( text );
    }
}


/* --------------------------------------------------------------
 of Sub App から/complete/word が通知されたタイミングで実行される
 新しく読み上げファイルを書き出せる状態にする。 
--------------------------------------------------------------  */
void WordSource::updateWordState( bool isGetNewWord, bool isPublishProgress ) {
    _isGetNewWord = isGetNewWord;
    _isPublishProgress = isPublishProgress;
}


//--------------------------------------------------------------
void WordSource::setDebugState( bool state ) {
    _isDebug = state;
}


//--------------------------------------------------------------
void WordSource::setID( int ID ) {
    cout << "WordSource -> setID\n";
    _ID = ID;
}


//--------------------------------------------------------------
int WordSource::getID() {
    cout << "WordSource -> getID\n";
    return _ID;
}


/* --------------------------------------------------------------
 Max/Msp に OSCでデータを送信する
 
 @access	private
 @param	none
 @return	none
 --------------------------------------------------------------  */
void WordSource::_sendSignalToMax() {
    ofxOscMessage message;
    
    message.setAddress("/instanceID");
    message.addIntArg(_ID);
    
    message.setAddress("/smileTrigger");
    message.addIntArg(_ID);
    
    _maxSender.sendMessage(message);
}


//--------------------------------------------------------------
string WordSource::_wordValidation( string message ){
    // 正規表現で余計な文字列を削除する
    RegularExpression test( "\"" );
    test.subst( message, "" );
    
    // 改行コードのパターン
    RegularExpression bReakEx1( "\r\n" );
    bReakEx1.subst( message, "" );
    
    RegularExpression bReakEx2( "\r" );
    bReakEx2.subst( message, "" );
    
    RegularExpression bReakEx3( "\n" );
    bReakEx3.subst( message, "" );
    
    // 絵文字のパターン
    RegularExpression pictogramEx( "[\xF0-\xF7][\x80-\xBF][\x80-\xBF][\x80-\xBF]" );
    pictogramEx.subst( message, "" );
    
    // Twitterアカウントのパターン
    RegularExpression accountEx( "@([A-Za-z]+[A-Za-z0-9_]+)" );
    accountEx.subst( message, "" );
    
    // URLのパターン
    RegularExpression urlEx( "(https?|ftp)(://[-_.!~*\'()a-zA-Z0-9;/?:@&=+$,%#]+)" );
    urlEx.subst( message, "" );
    
    // メールアドレスのパターン
    RegularExpression mailEx( "[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9-]+(?:.[a-zA-Z0-9-]+)*" );
    mailEx.subst( message, "" );
    
    return message;
}