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
        ofxJSONElement _setting; // アプリケーションの設定ファイル
        ofxOscSender _maxSender; // Max/Msp 用のOSC Sender
        void _sendSignalToMax();
    public:
        WordSource( int ID );
        void setup();
        void update();
        void draw();
    
        void setID(int ID);
};

#endif /* defined(__word_bridge__WordSource__) */
