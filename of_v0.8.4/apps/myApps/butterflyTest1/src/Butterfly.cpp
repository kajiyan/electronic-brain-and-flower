#include "Butterfly.h"

Butterfly::Butterfly( string objID, int modelIndex ){
    cout << "Butterfly -> Constructor " << modelIndex << "\n";
    
    
    // 画像（テクスチャ）の読み込みが完了したらモデルデータ読み込み 第2引数はスケール
    _body.loadModel("butterfly/body.3ds", 2.0);
    _hone.loadModel("butterfly/hone.3ds", 2.0);
    _featherLT.loadModel("butterfly/featherLT.3ds", 2.0);
    _featherLU.loadModel("butterfly/featherLU.3ds", 2.0);
    _featherRT.loadModel("butterfly/featherRT.3ds", 2.0);
    _featherRU.loadModel("butterfly/featherRU.3ds", 2.0);
    
    
    // 設定ファイルを取得する
    _setting.open("config.json");
    
    _objID = objID;
    _modelIndex = modelIndex;
    
    // OSCを送信したかのフラグ
    _isSendMHlampOSC = false;
    
    // 表示のステータス 初期値は非表示
    _isVisible = false;
    
    // 画像書き出し完了のステータス
    _isImagePublish = false;
    
//    _pattern = (int)ofRandom(0, 3);
    _pattern = 0;
    
    int travelLength[] = { 18, 18, 24, 18 };
    int division[] = { 3, 3, 3, 3 };
    int interpolate[] = { 28, 14, 14, 14 };
    
    int velocityX[] = { 130, 130, 130, 80 };
    
    int n[] = {
        1 + ( travelLength[_pattern] * 2 ),
        1 + ( travelLength[_pattern] * 2 ),
        1 + ( travelLength[_pattern] * 2 ),
        1 + ( travelLength[_pattern] * 2 )
    };
    int m[] = {
        division[_pattern],
        division[_pattern],
        division[_pattern],
        division[_pattern]
    };
    
    cout << ofGetHeight() + 550 << "\n";
    
    int offsetX[] = { -700, -500, -2500, -600 };
    int offsetY[] = {
        ofGetHeight() + 550,
        ofGetHeight() + 1000,
        ofGetHeight() + 50,
        0
    };
    int offsetZ[] = { -1000, -1000, -1600, 1300 };
    
    float maxFlip[] = { 0.4, 0.8, 0.8, 0.8 };
    
    
    for ( int i = 0; i < BUTTERFLY_PATTERN; i++ ) {
        _TRAVEL_LENGTH[i] = travelLength[i];
        _DIVISION[i] = division[i];
        _INTERPOLATE[i] = interpolate[i];
        
        _VELOCITY_X[i] = velocityX[i];
        
        _N[i] = n[i];
        _M[i] = m[i];
        
        _OFFSET_X[i] = offsetX[i];
        _OFFSET_Y[i] = offsetY[i];
        _OFFSET_Z[i] = offsetZ[i];
        
        _MAX_FLIP[i] = maxFlip[i];
    }

    // 配列の初期化
    for (int i= 0; i < _N[_pattern]; i++){
        float* peak = new float[_M[_pattern]];
        _peakArray.push_back(peak);
        _peak.push_back(peak);
        
        _peak.push_back(new float[_M[_pattern]]);
        _peak[i][0] = 0;
        _peak[i][1] = 0;
        _peak[i][2] = 0;
    }
    
    createPeak();
    generateOrbit();
}



//--------------------------------------------------------------
void Butterfly::setup(){
    cout << "Butterfly -> Setup\n";

    // Max 用のOSC Senderのセットアップ
    _maxSender.setup(
        _setting["address"]["max"]["host"].asString(),
        _setting["address"]["max"]["port"].asInt()
    );
    
//    // 画像ローダーのセットアップ
//    _imagePublish.setup( 0, "butterfly" + ofToString(_modelIndex) + "/" );
//    _imagePublish.setFileName( "wing.jpg" );
//
//    // 画像が書き出された時のイベントリスナー
//    ofAddListener(_imagePublish.publishComplete, this, &Butterfly::imagePublishCallback);
}



//--------------------------------------------------------------
void Butterfly::update(){
//    _imagePublish.update();
    
    // スローと画像の書き出しが完了していたら一度だけOSCを発信する
    // MHlampを消灯する信号をMax/Mspに送る
    if ( _isVisible && _isImagePublish && !_isSendMHlampOSC ) {
        _isSendMHlampOSC = true;
        
        ofxOscMessage sendMessage;
        sendMessage.setAddress( "/MHlamp/status" );
        sendMessage.addIntArg( 0 );
        _maxSender.sendMessage( sendMessage );
    }
}



//--------------------------------------------------------------
void Butterfly::draw(){
//    _imagePublish.draw();

//    // スローと画像の書き出しが完了していたらアニメーションをはじめる
//    if ( _isVisible && _isImagePublish ) {
    float x = _orbit[_count][0];
    float y = _orbit[_count][1];
    float z = _orbit[_count][2];
    
    float rfy = .3;
    float ft = _flip[_count][0];
    float fu = _flip[_count][1];
    
    ofPushStyle();
    
    ofSetColor(255);
    
    ofEnableSmoothing();
    glShadeModel(GL_SMOOTH);
    
    glPushMatrix();
    
    // glTranslatef(ofGetWidth() / 2, ofGetHeight() / 2, 0);
    
    glTranslatef(_OFFSET_X[_pattern], _OFFSET_Y[_pattern], _OFFSET_Z[_pattern]);
    
    glTranslatef(x, y, z);
    
    glRotatef(_rotation[_count][0] * -1 * 180 / PI + -90, 1, 0, 0);   // x軸
    glRotatef(180, 0, 1, 0);                                          // y軸
    glRotatef(_rotation[_count][1] * -1 * 180 / PI, 0, 0, 1);         // z軸
    _body.draw();
    
    // featherLT
    glPushMatrix();
    glRotatef(ft * rfy * 180 / PI, 0, 0, 1);    // y軸
    glRotatef(-ft * 180 / PI, 0, 1, 0);         // z軸
    _featherLT.draw();
    glTranslatef(0, 0, 0);
    glPopMatrix();
    
    // featherLU
    glPushMatrix();
    glRotatef(-fu * 180 / PI, 0, 1, 0);         // y軸
    glRotatef(fu * rfy * 180 / PI, 0, 0, 1);    // z軸
    _featherLU.draw();
    glTranslatef(0, 0, 0);
    glPopMatrix();
    
    // featherRT
    glPushMatrix();
    glRotatef(ft * 180 / PI, 0, 1, 0);          // y軸
    glRotatef(-ft * rfy * 180 / PI, 0, 0, 1);   // z軸
    _featherRT.draw();
    glTranslatef(0, 0, 0);
    glPopMatrix();
    
    // featherRU
    glPushMatrix();
    glRotatef(fu * 180 / PI, 0, 1, 0);          // y軸
    glRotatef(-fu * rfy * 180 / PI, 0, 0, 1);   // z軸
    _featherRU.draw();
    glTranslatef(0, 0, 0);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(_rotation[_count][3] * -1 * 180 / PI, 1, 0, 0); // y軸
    _hone.draw();
    glPopMatrix();
    
    // glTranslatef(0, 0, 0);
    glPopMatrix();
    ofPopStyle();
    
    
    if(++_count >= _orbit.size()){
        for (int i = 0; i < _orbitArray.size(); i++) {
            delete _orbitArray[i];
        }
        _orbitArray.clear();
        
        for (int i = 0; i < _rotationArray.size(); i++) {
            delete _rotationArray[i];
        }
        _rotationArray.clear();
        
        for (int i = 0; i < _flipArray.size(); i++) {
            delete _flipArray[i];
        }
        _flipArray.clear();
        
        _orbit.clear();
        _rotation.clear();
        _flip.clear();
        
        for (int i = 0; i < _peakArray.size(); i++) {
            delete _peakArray[i];
        }
        _peakArray.clear();
        
        _count = 0;
        
        createPeak();
        generateOrbit();
    }

//    }
}



//--------------------------------------------------------------
void Butterfly::createPeak(){
    int len = _peak.size();
    
    float minX = -5;
    float maxX = 5;
    float minY = 20;
    float maxY = 40;
    float minZ = 0;
    float maxZ = 200;
    
    float minYoffset = 3;
    float maxYoffset = 5;
    float minZoffset = 0;
    float maxZoffset = 20;
    float downF = 40;
    float upF = -100;
    
    float radiusZ = 1000;
    
    float initX = _peak[len-1][0];
    float initY = _peak[len-1][1];
    float initZ = _peak[len-1][2];
    
    if(initX > 8000 || initX < -7000 ||
       initY > 4000 || initY < -3000 ||
       initZ > 1000 || initZ < -10000){
        
        initY = _peak[len-1][1] = 0;
        initZ = _peak[len-1][2] = 0;
        
        if(initX > 0){
            initX = _peak[len-1][0] = 1500;
        }else{
            initX = _peak[len-1][1] = -100;
        }
    }
    
    float offsetX = ofRandom(-100, 100);
    
    float rotation = ofRandom(0.9, 1.5);
    
    float velocity_x = _VELOCITY_X[_pattern];
    
    if( initX > 3000 || initX <- 1500 ){
        velocity_x *= 2;
    }
    
    int travel_len = _TRAVEL_LENGTH[_pattern];
    _n = 1 + ( travel_len * 2 );
    _m = _M[_pattern];
    
    int index = (int)ofRandom(0, _n / 2);
    int l = (int)ofRandom(12, 18);
    
    _isSlowFlip = false;
    _isPowerMode = false;
    
    if(initY > -100){
        upF = -150;
        downF = 10;
        index = (int)0;
        _isPowerMode = true;
        
    }else if(initY > -200){
        index = (int)ofRandom(0, 3);
    }else if(initY < -900){
        rotation = 0.5;
        //slider
        if(ofRandom(0, 1) > .0){
            _isSlowFlip = true;
            rotation = 0.4;
            velocity_x = velocity_x * 1.7;
            l = 0;
            downF = 70;
            travel_len = int(10);
            _n = 1 + (travel_len * 2);
            
        }else{
            upF = -60;
            downF = 60;
        }
    }
    
    _peak.clear();
    for (int i= 0; i < _n; i++){
        float* peak = new float[_m];
        _peakArray.push_back(peak);
        _peak.push_back(peak);
    }
    
    _peak[0][0] = initX;
    _peak[0][1] = initY;
    _peak[0][2] = initZ;
    
    _directionX = _directionX * -1;
    _directionY = _directionY * -1;
    
    float vx[_n];
    float vy[_n];
    float deltaY[_n];
    float offsetY[_n];
    
    for (int i = 0; i < _n; i++){
        vx[i] = velocity_x * _directionX * abs(sin(PI * rotation / travel_len * i));
        vy[i] = 1;
        deltaY[i] = downF;
    }
    
    for (int i = 0; i < l; i++){
        vx[index + i] = vx[index + i] * 0.8;
        vy[index + i] = ofRandom(minYoffset, maxYoffset);
        deltaY[index + i] = upF;
    }
    
    index = int(ofRandom(_n / 2 + 6, _n - 4));
    l = (initY <= -100) ? 0 : 4;
    
    for (int i = 0; i < l; i++){
        vx[index + i] = vx[index + i] * 0.8;
        vy[index + i] = ofRandom(minYoffset, maxYoffset);
        deltaY[index + i] = upF;
    }
    
    offsetY[0] = initY;
    for (int i = 1; i < sizeof(deltaY) / sizeof(deltaY[0]); i++){
        offsetY[i] = offsetY[i-1] + deltaY[i-1];
    }
    
    for (int i = 1; i < _n; i++){
        _peak[i][0] = _peak[i-1][0] + vx[i] + ofRandom(minX, maxX);
        _peak[i][1] = sin(PI / 2 * i) * -(ofRandom(minY, maxY)) * (vy[i]) + offsetY[i];
        _pRadZ += PI * rotation / travel_len;
        _peak[i][2] = _OFFSET_Z[_pattern] + sin(_pRadZ) * radiusZ + (ofRandom(minZoffset,maxZoffset));
    }
}



//--------------------------------------------------------------
void Butterfly::generateOrbit(){
    int len = (_peak.size() - 1) / 2;
    int interpolate = _INTERPOLATE[_pattern];
    float tmp;
    float plus = 1.0 / (interpolate - 1);
    
    for (int i= 0; i < _INTERPOLATE[_pattern] * ((_n - 1) / 2); i++){
        float* orbit = new float[3];
        _orbitArray.push_back(orbit);
        _orbit.push_back(orbit);
    }
    
    for (int i= 0; i < _INTERPOLATE[_pattern] * ((_n - 1) / 2); i++){
        float* rotation = new float[4];
        _rotationArray.push_back(rotation);
        _rotation.push_back(rotation);
    }
    
    for (int i= 0; i < _INTERPOLATE[_pattern] * ((_n - 1) / 2); i++){
        float* flip = new float[2];
        _flipArray.push_back(flip);
        _flip.push_back(flip);
    }
    
    float vfl = 0.8;
    float vfu = 0.5;
    
    for(int j = 0 ; j < len ; j++){
        vector <float> p0;
        for (int i = 0; i < _peak.size(); i++) {
            p0.push_back(_peak[j * 2][i]);
        }
        
        vector <float> p1;
        for (int i = 0; i < _peak.size(); i++) {
            p1.push_back(_peak[j * 2 + 1][i]);
        }
        
        vector <float> p2;
        for (int i = 0; i < _peak.size(); i++) {
            p2.push_back(_peak[j * 2 + 2][i]);
        }
        
        float d = 0;
        
        int offset = j * interpolate;
        int n = offset + interpolate;
        
        float fs = 0.2;
        
        float slow = ofRandom(0.01, .8);
        float slowFl = slow;
        float slowFu = slow / 4;
        
        for(int i = offset; i < n; i++){
            _orbit[i][0] = 0;
            _orbit[i][1] = 0;
            _orbit[i][2] = 0;
            
            tmp = (1 - d) * (1 - d);
            _orbit[i][0] += tmp * p0[0];
            _orbit[i][1] += tmp * p0[1];
            _orbit[i][2] += tmp * p0[2];
            
            tmp = 2 * d * (1 - d);
            _orbit[i][0] += tmp * p1[0];
            _orbit[i][1] += tmp * p1[1];
            _orbit[i][2] += tmp * p1[2];
            
            tmp = d * d;
            _orbit[i][0] += tmp * p2[0];
            _orbit[i][1] += tmp * p2[1];
            _orbit[i][2] += tmp * p2[2];
            
            float x = _orbit[i][0];
            float y = _orbit[i][1];
            float z = _orbit[i][2];
            
            float dx = x - _px;
            float dy = y - _py;
            float dz = z - _pz;
            
            _px = x;
            _py = y;
            _pz = z;
            
            fs = sqrt((dy * dy) + (dy * dy));
            fs = sqrt((fs * fs) + (dz * dz)) * 0.4;
            fs = min(_MAX_FLIP[_pattern], fs);
            
            if(_isPowerMode){
                fs = _MAX_FLIP[_pattern];
            }else if(dy > -10 && dy < 15){
                fs = fs * 0.3;
            }
            
            _ang += fs * _flipDirection;
            if (_ang > PI / 2){
                _ang = PI / 2;
                _flipDirection *= -1;
            }else if (_ang < -PI / 2){
                _ang = -PI / 2;
                _flipDirection *= -1;
            }
            
            if(dy > -10 && dy < 15 && !_isPowerMode){
                if(_isSlowFlip){
                    vfl += (0.1 - vfl) / _INTERPOLATE[_pattern];
                    vfu += (0.08 - vfu) / _INTERPOLATE[_pattern];
                }else{
                    vfl += (0.6 - vfl) / _INTERPOLATE[_pattern];
                    vfu += (0.4 - vfu) / _INTERPOLATE[_pattern];
                }
            }else{
                vfl += (1 - vfl) / _INTERPOLATE[_pattern];
                vfu += (0.8 - vfu) / _INTERPOLATE[_pattern];
            }
            
            _flip[i][0] = _ang * vfl;
            _flip[i][1] = _ang * vfu;
            
            float rx;
            float ry;
            float rz;
            
            rx = PI / 10 - (atan2(dy, dx * _directionX)) * 0.25;
            rx += ofRandom(0, 0.05);
            rx += .025;
            ry = atan2(-dz, dx) + PI / 2;
            rz = atan2(dy, dx) - PI / 2;
            
            _rotation[i][0] = rx;
            _rotation[i][1] = ry;
            _rotation[i][2] = rz;
            float rh = ofRandom(-1, 0.3);
            _rotation[i][3] = (rh > 0.2) ? rh : 0;
            _rotation[i][3] = 0;
            
            d += plus;
        }
        
        _px = _orbit[n-2][0];
        _py = _orbit[n-2][1];
        _pz = _orbit[n-2][2];
        
        vector<float>(p0).swap(p0);
        vector<float>(p1).swap(p1);
        vector<float>(p2).swap(p2);
    }
}


