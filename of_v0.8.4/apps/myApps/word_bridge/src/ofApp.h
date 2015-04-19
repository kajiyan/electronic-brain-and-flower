#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxJSON.h"


#include "Butterfly.h"
#include "ImagePublish.h"
#include "WordSource.h"


#define FPS  60.0


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    
        // アプリケーションの設定ファイル
        ofxJSONElement setting;
    
    
        /* =============================================================================
         
         Butterfry
         蝶々のビジュアルクラス
         
         ============================================================================= */
        vector<Butterfly *> butterfrys;
    
    
        /* =============================================================================
     
         ImagePublish
         HTTP経由で画像を取得して書き出すクラス
     
         ============================================================================= */
        int ImagePublishSize = 1;
        vector<ImagePublish *> imagePublishs;
    
    
        /* =============================================================================
         
         WordSource
         取得した言葉を管理するクラス
         
         ============================================================================= */
        int wordsSize = 2;
        vector<WordSource *> wordSources;
    
    
        ofxOscReceiver receiver;
    
        string wordValidation(string message);
};
