//
//  WordSource.cpp
//  word_bridge
//
//  Created by KAJIYA HIROKI on 2015/04/14.
//
//

#include "WordSource.h"

//--------------------------------------------------------------
WordSource::WordSource( int ID ) {
    cout << "WordSource -> constructor\n";
    
    // 設定ファイルを取得する
    _setting.open("config.json");
    
    _ID = ID;
}

//--------------------------------------------------------------
void WordSource::setup() {
    cout << "WordSource -> setup\n";
    
    // Max 用のOSCのセットアップ
    _maxSender.setup(
        _setting["address"]["max"]["host"].asString(),
        _setting["address"]["max"]["port"].asInt()
    );
}


//--------------------------------------------------------------
void WordSource::update() {
    _sendSignalToMax();
}


//--------------------------------------------------------------
void WordSource::draw() {
    
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
void WordSource::setID( int ID ) {
    cout << "WordSource -> setID\n";
    _ID = ID;
}