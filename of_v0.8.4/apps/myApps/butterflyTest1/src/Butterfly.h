#ifndef __word_bridge__Butterfly__
#define __word_bridge__Butterfly__

#include <stdio.h>

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofx3DModelLoader.h"
#include "ofxJSON.h"

#define BUTTERFLY_PATTERN 4

class Butterfly {
private:
    int _pattern;
    
    int _TRAVEL_LENGTH[BUTTERFLY_PATTERN];
    int _DIVISION[BUTTERFLY_PATTERN];
    int _INTERPOLATE[BUTTERFLY_PATTERN];
    
    int _VELOCITY_X[BUTTERFLY_PATTERN];
    
    int _N[BUTTERFLY_PATTERN];
    int _M[BUTTERFLY_PATTERN];
    
    int _OFFSET_X[BUTTERFLY_PATTERN];
    int _OFFSET_Y[BUTTERFLY_PATTERN];
    int _OFFSET_Z[BUTTERFLY_PATTERN];
    
    float _MAX_FLIP[BUTTERFLY_PATTERN];
    
    ofxJSONElement _setting;
    
    ofx3DModelLoader _body;
    ofx3DModelLoader _hone;
    ofx3DModelLoader _featherLT;
    ofx3DModelLoader _featherLU;
    ofx3DModelLoader _featherRT;
    ofx3DModelLoader _featherRU;
    
    ofxOscSender _maxSender;
    
    bool _isPowerMode = false;
    bool _isSlowFlip = false;
    
    int _flg = 0;
    
    vector <float *> _peakArray;
    vector <float *> _peak;
    
    int _directionX = -1;
    int _directionY = -1;
    float _pRadZ;

    vector <float *> _orbitArray;
    vector <float *> _orbit;
    
    vector <float *> _rotationArray;
    vector <float *> _rotation;
    
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
    
    string _objID;
    int _modelIndex;
    
    bool _isSendMHlampOSC;
    bool _isVisible;
    bool _isImagePublish;
public:
    Butterfly( string objID, int modelIndex );
    void setup();
    void update();
    void draw();
    void createPeak();
    void generateOrbit();
};

#endif