#include "WordSource.h"

#include "Poco/String.h"
#include "Poco/RegularExpression.h"

using Poco::replace;
using Poco::RegularExpression;

// 音楽のラベル
static string _sceneLabels[6] = { "introduction", "rain", "wind", "breathe", "internet", "coda" };


//--------------------------------------------------------------
WordSource::WordSource( int ID ) {
    cout << "WordSource -> constructor ID " << ID << "\n";
    
    // 設定ファイルを取得する
    _setting.open("config.json");
    
    _ID = ID;
    _uniqueID = 0; // ファイル名を一意にするもの
    
    _isDebug = true;
    
    _isGetNewWord = true;       // 新しいワードを取得できるかのフラグ
    _isPublishProgress = false; // 音声ファイルの書き出し中かのフラグ
    
    //_isPublishWord = false;   // 音声ファイルの書き出しが終了したかのフラグ
    //_isCompletePlay = false;  // 音声ファイルの読み上げが終了したかのフラグ
    
    _sceneIndex = 0; // 音楽のシーンID
    
//    if ( _isDebug ) {
//        _sceneIndex = 1;
//    }
    
    _MHlampSignal = 0;
    
    // Max/Mspに読み上げ用のシグナルを送ってから
    // /complete/wordが返ってくるまでの待機時間を指定する
    _wordPlayEndTime = 10000;
    _isTimerReached = true;
}

//--------------------------------------------------------------
void WordSource::setup() {
    cout << "WordSource -> setup\n";
    
    _nodeAppSender.setup(
        _setting["address"]["node"]["host"].asString(),
        _setting["address"]["node"]["port"].asInt()
    );
    
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
    _debugUpdateStream = "WordSource ID" + ofToString( _ID ) + " -> /updateStream \n" +
    "text: " + _debugUpdateStreamText + "\n";
    
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
        cout << "WordSource -> update -> uniqueID:" << _uniqueID << "\n";
        
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
        
        
        ofxOscMessage sendWebSocketMessage;
        sendWebSocketMessage.setAddress( "/add/word" );
        sendWebSocketMessage.addStringArg( "\"" + _text + "\"" );
        _nodeAppSender.sendMessage( sendWebSocketMessage );
        
        setTimeOut( 10000 );
    }

    
    // Max/Mspに読み上げ用のシグナルを送ってから特定の時間以内に
    // /complete/wordが返ってこない時、Sub Appからの応答がないときの処理
    float timer = ofGetElapsedTimeMillis() - _wordPlayStartTime;
    if( timer >= _wordPlayEndTime && !_isTimerReached ) {
        cout << "WordSource -> TimeOut\n";
        _isTimerReached = true;
        updateWordState( true, false, true ); // OSC受信可能状態に戻す
    }
}


//--------------------------------------------------------------
void WordSource::draw() {
    if ( _isDebug ) {
        ofPushStyle();
        
        ofSetColor(ofColor(255, 255, 255));
        
        ofDrawBitmapString(_debugString, 20, 100 * _ID + 100);
        ofDrawBitmapString(_debugUpdateStream, 20, 500);
        
        ofPopStyle();
    }
}



/* --------------------------------------------------------------
 /updateScene でMax から取得した音楽のシーンIDをセットする
 
 @access	public
 @param	    none
 @return	none
 --------------------------------------------------------------  */
void WordSource::setSceneIndex( int sceneIndex ) {
    _sceneIndex = sceneIndex;
}



/* --------------------------------------------------------------
 /add/word でof Sub Appに送信するデータをセットする
 
 @access	public
 @param	    none
 @return	none
 --------------------------------------------------------------  */
void WordSource::setWord( string language, string text ) {
    _debugUpdateStreamText = _wordValidation( text );
    
    int sceneLabelLen = sizeof(_sceneLabels) / sizeof(_sceneLabels[0]);
    
    if ( _isGetNewWord && ( _sceneIndex > 0 && _sceneIndex < sceneLabelLen ) ) {
        _language = _wordValidation( language );
        _text = _wordValidation( text );
        
        // 現在のシーン名を取得
        string sceneLabel = _sceneLabels[_sceneIndex - 1];
        
        // _textが空ではなく、_textの中にシーン名が含まれているか
        if( !_language.empty() && !_text.empty() && ofStringTimesInString(_text, sceneLabel) > 0 ){
            cout << "WordSource -> setWord: " << _text << "\n";
            // 新しいテキスト情報を設定できない状態にする
            _isGetNewWord = false;
        }
    }
}



/* --------------------------------------------------------------
 指定時間以内にMax/Mspから読み上げ完了のフラグが戻ってこなければフラグを戻す
 
 @access	public
 @param	    float wordPlayEndTime タイムアウトまでの時間（ms）
 @return	none
 --------------------------------------------------------------  */
void WordSource::setTimeOut( float wordPlayEndTime ) {
    cout << "WordSource -> setTimeOut: " + ofToString( wordPlayEndTime ) + "ms\n";
    
    _wordPlayStartTime = ofGetElapsedTimeMillis();
    _wordPlayEndTime = wordPlayEndTime;
    _isTimerReached = false;
}



/* --------------------------------------------------------------
 /pump/status でMax/Mspに送信するデータをセットする
 
 @access	public
 @param	    none
 @return	none
 --------------------------------------------------------------  */
void WordSource::updatePumpSignal( string text ) {
    // 現在のシーンが雨なら処理をする
    if( _sceneIndex == 2 ) {
        string sceneLabel = _sceneLabels[1]; // Rain
        text = _wordValidation( text );
        
        if( (ofStringTimesInString(text, sceneLabel) > 0) ) {
            cout << "WordSource -> sendOSC | pump/status\n";
            
            ofxOscMessage sendMessage;
            sendMessage.setAddress( "/pump/status" );
            sendMessage.addIntArg( 1 );
            _maxSender.sendMessage( sendMessage );
        }
    }
}



/* --------------------------------------------------------------
 /MHlamp/status でMax/Mspに送信するデータをセットする
 
 @access	public
 @param	    none
 @return	none
 --------------------------------------------------------------  */
void WordSource::updateMHlampSignal( string text ) {
    text = _wordValidation( text );
    
    int sceneLabelLen = sizeof(_sceneLabels) / sizeof(_sceneLabels[0]);
    string sceneLabel = _sceneLabels[_sceneIndex - 1];
    
    // 現在のシーンのツイートの中に特定のキーワードが入っているか判別する
    // キーワードが含まれていた場合Max/Mspに対してOSCを発信する。
    // if( (ofStringTimesInString(text, sceneLabel) > 0) && (ofStringTimesInString(text, "lol") > 0 || ofStringTimesInString(text, "rofl")) ) {
    if( (ofStringTimesInString(text, "lol") > 0 || ofStringTimesInString(text, "rofl")) ) {
        cout << "WordSource -> sendOSC | /MHlamp/status\n";
        
        ofxOscMessage sendMessage;
        sendMessage.setAddress( "/MHlamp/status" );
        sendMessage.addIntArg( 1 );
        _maxSender.sendMessage( sendMessage );
    }
}



/* --------------------------------------------------------------
 Max/Msp から/complete/word が通知されたタイミングで実行される
 新しく読み上げファイルを書き出せる状態にする。 
--------------------------------------------------------------  */
void WordSource::updateWordState( bool isGetNewWord, bool isPublishProgress, bool isTimerReached ) {
    _isGetNewWord = isGetNewWord;
    _isPublishProgress = isPublishProgress;
    _isTimerReached = isTimerReached;
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
    
    // 改行コードのパターン
    RegularExpression bReakEx1( "\r\n" );
    bReakEx1.subst( message, "", RegularExpression::RE_GLOBAL );
    RegularExpression bReakEx2( "\r" );
    bReakEx2.subst( message, "", RegularExpression::RE_GLOBAL );
    RegularExpression bReakEx3( "\n" );
    bReakEx3.subst( message, "", RegularExpression::RE_GLOBAL );
    
    // 絵文字のパターン
    RegularExpression pictogramEx( "[\xF0-\xF7][\x80-\xBF][\x80-\xBF][\x80-\xBF]" );
    pictogramEx.subst( message, "", RegularExpression::RE_GLOBAL );
    
    // Twitterアカウントのパターン
    RegularExpression accountEx( "@([A-Za-z]+[A-Za-z0-9_]+)" );
    accountEx.subst( message, "", RegularExpression::RE_GLOBAL );
    
    // URLのパターン
    RegularExpression urlEx( "(https?|ftp)(://[-_.!~*\'()a-zA-Z0-9;/?:@&=+$,%#]+)" );
    urlEx.subst( message, "", RegularExpression::RE_GLOBAL );
    
    // メールアドレスのパターン
    RegularExpression mailEx( "[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9-]+(?:.[a-zA-Z0-9-]+)*" );
    mailEx.subst( message, "", RegularExpression::RE_GLOBAL );
    
    // 記号のパターン
    RegularExpression reSymbol( "[\x21-\x2f\x3a-\x40\x5b-\x60\x7b-\x7e]" );
    reSymbol.subst( message, "", RegularExpression::RE_GLOBAL );
    
    
    return message;
}