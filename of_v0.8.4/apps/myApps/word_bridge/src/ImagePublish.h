#ifndef __word_bridge__ImagePublish__
#define __word_bridge__ImagePublish__

#include "ofMain.h"
#include "ofEvents.h"
#include "ofURLFileLoader.h"

class ImagePublish {
    public:
        ImagePublish();
        ~ImagePublish();
    
        ofEvent<bool> publishComplete;
        
        void urlResponse(ofHttpResponse & response);
        void setup( int ID, string publishPath );
        void update();
        void draw();
        void setFileName( string fileName );
        void addLoadFileName( string fileName );
        
        //    void publishCompleteEvent();

    private:
        int _ID;
        ofImage _image;
        bool _isLoading;
        bool _isLoadingComplete;
        bool _isPublishComplete;
        bool _isConfigFileName;
        string _publishPath;
        string _fileName; // 書きだされるファイルの名前
        vector <string> _loadFileNames;
        void _publish();
};

#endif
