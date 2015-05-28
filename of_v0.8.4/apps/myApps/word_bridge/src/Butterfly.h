//
//  Butterfly.h
//  word_bridge
//
//  Created by KAJIYA HIROKI on 2015/04/19.
//
//

#ifndef __word_bridge__Butterfly__
#define __word_bridge__Butterfly__

#include <stdio.h>

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofx3DModelLoader.h"
#include "ofxJSON.h"

#include "ImagePublish.h"


class Butterfly {
    const int TRAVEL_LENGTH = 18;
    const int DIVISION = 3;
    const int INTERPOLATE = 14;
    
    const int VELOCITY_X = 130;
    
    const int N = 1 + (TRAVEL_LENGTH * 2);
    const int M = DIVISION;
    
    const int OFFSET_X = -700;
    const int OFFSET_Y = ofGetHeight() + 550;
    const int OFFSET_Z = -1000;
    
    const float MAX_FLIP = 0.8;
    
private:
    ImagePublish _imagePublish;
    
    ofxJSONElement _setting; // アプリケーションの設定ファイル
    
    ofx3DModelLoader _body;
    ofx3DModelLoader _hone;
    ofx3DModelLoader _featherLT;
    ofx3DModelLoader _featherLU;
    ofx3DModelLoader _featherRT;
    ofx3DModelLoader _featherRU;
    
    ofxOscSender _maxSender; // Max/Msp 用のOSC Sender
    
    Boolean _isPowerMode = false;
    Boolean _isSlowFlip = false;
    
    int _flg = 0;
    // float* _peakArray;
    vector <float *> _peakArray;
    vector <float *> _peak;
    
    int _directionX = -1;
    int _directionY = -1;
    float _pRadZ;
    
    //float* _orbitArray;
    vector <float *> _orbitArray;
    vector <float *> _orbit;
    
    //float* _rotationArray;
    vector <float *> _rotationArray;
    vector <float *> _rotation;
    
    //float* _flipArray;
    vector <float *> _flipArray;
    vector <float *> _flip;
    
    float _px = 0;
    float _py = 0;
    float _pz = 0;
    
    float _ang = 0;
    
    int _flipDirection = 1;
    
    int _count = 0;
    
    int _n;
    int _m;
    
    float _fps;
    string _objID;
    int _modelIndex;
    
    bool _isSendMHlampOSC;
    bool _isVisible;
    bool _isImagePublish;
    
public:
    Butterfly(float fps, string objID, int modelIndex);
    void setup();
    void update();
    void draw();
    void fly();
    void createPeak();
    void generateOrbit();
    
    void destinationPeak();
    void destinationOrbit();
    
    void updateVisible(string objID);
    void addLoadFileName( string fileName );
    void imagePublishCallback( bool & e );
};

#endif /* defined(__word_bridge__Butterfly__) */
