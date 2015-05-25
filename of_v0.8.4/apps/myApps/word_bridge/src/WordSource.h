//
//  WordSource.h
//  word_bridge
//
//  Created by KAJIYA HIROKI on 2015/04/14.
//
//

#ifndef __word_bridge__WordSource__
#define __word_bridge__WordSource__

#include <stdio.h>

#include <sys/stat.h>

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxOsc.h"
#include "ofxJSON.h"

class WordSource {
private:
    int _ID;
    int _uniqueID;
    
    bool _isDebug;
    string _debugString;
    
    ofxJSONElement _setting; // アプリケーションの設定ファイル
    
    ofxOscSender _ofSubAppSender; // OF Sub App 用のOSC Sender
    ofxOscSender _maxSender;      // Max/Msp 用のOSC Sender
    
    int _sceneIndex;
    int _MHlampSignal;
    
    bool _isGetNewWord;
    bool _isPublishProgress;
    //bool _isPublishWord;
    //bool _isCompletePlay;
    string _language;
    string _text;
    
    void _sendSignalToMax();
    string _wordValidation( string message );
    
public:
    WordSource( int ID );
    void setup();
    void update();
    void draw();
    
    void setID( int ID );
    int  getID();
    void setDebugState( bool state );
    void setSceneIndex( int sceneIndex );
    void setWord( string language, string text );
    void updateMHlampSignal( string text );
    void updateWordState( bool isGetNewWord, bool isPublishProgress );
};

#endif /* defined(__word_bridge__WordSource__) */
