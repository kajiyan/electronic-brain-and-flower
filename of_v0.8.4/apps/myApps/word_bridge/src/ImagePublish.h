#ifndef __word_bridge__ImagePublish__
#define __word_bridge__ImagePublish__

#include <stdio.h>

#include "ofMain.h"
#include "ofEvents.h"
#include "ofURLFileLoader.h"

class ImagePublish {
private:
    int _ID;
    ofImage _image;
    bool _isLoading;
    bool _isLoadingComplete;
    bool _isPublishComplete;
    string _publishPath;
    vector <string> _loadFileNames;
    void _publish();
    
public:
    ImagePublish( int ID, string publishPath );
    
    ofEvent<bool> publishComplete;
    
    void urlResponse(ofHttpResponse & response);
    // void setup();
    void update();
    void draw();
    void addLoadFileName( string fileName );
    
//    void publishCompleteEvent();
};

#endif
