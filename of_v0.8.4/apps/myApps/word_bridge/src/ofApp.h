#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxJSON.h"


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
         
         WordSource
         取得した言葉を管理するクラス
         
         ============================================================================= */
        int wordsSize = 2;
        vector<WordSource *> wordSources;
    
    
        ofxOscReceiver receiver;
    
        string wordValidation(string message);
};
