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
        bool _loading;
        bool _loadingComplete;
    public:
        ImagePublish( int ID );
        void urlResponse(ofHttpResponse & response);
//        void setup();
        void update();
        void draw();
//
//        void setID(int ID);
};

#endif
