#ifndef __word_publish__CommandRun__
#define __word_publish__CommandRun__

#include <stdio.h>

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxJSON.h"

class CommandRun {
    private:
        int _ID;
    
        bool _isCommandRun;
    
        int _commandStatusCode;
    
        string _voiseActor;
        string _word;
        int _uniqueID;
        int _fileID;
    
        ofxJSONElement _setting;
        ofxOscSender _ofMainAppSender;
    
    public:
        CommandRun( int ID );
        void setup();
        void update();
        void createSoundFile( string voiseActor, string word, int uniqueID, int fileID );
};

#endif
