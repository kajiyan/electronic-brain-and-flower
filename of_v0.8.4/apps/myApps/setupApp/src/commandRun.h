#ifndef __word_publish__CommandRun__
#define __word_publish__CommandRun__

#include <stdio.h>

#include "ofMain.h"

class CommandRun {
private:
    int _ID;
    
    bool _isCommandRun;
    
    int _commandStatusCode;
    
    string _voiseActor;
    string _word;
    int _uniqueID;
    int _fileID;
    
    
public:
    CommandRun( int ID );
    void setup();
    void update();
};

#endif
