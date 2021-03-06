#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    /*
     * Basic Configuration
     */
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetFrameRate(FPS);
    ofSetWindowTitle("電脳と花");
    
    ofBackground(0, 0, 0);
    
    // 設定ファイルを取得する
    bool parsingSuccessful = setting.open("config.json");
    
    if( parsingSuccessful ) {
        ofLogNotice("ofApp::setup") << setting.getRawString();

        // OSCを受信するポートの設定
        receiver.setup(setting["address"]["openFrameWorks"]["main"]["port"].asInt());
        
        // OSCを送信するポートの設定
        _maxMspAppSender.setup(
           ofToString(setting["address"]["max"]["host"]),
           setting["address"]["max"]["port"].asInt()
       );
    }
    
    // Debug 用の情報を取得するか
    isDebug = true;
    
    GLfloat lightOnePosition[] = { 40.0, 40, 100.0, 0.0 };
    GLfloat lightOneColor[]    = { 1.0, 1.0, 1.0, 1.0 };
    
    GLfloat lightTwoPosition[] = { -40.0, 40, 100.0, 0.0 };
    GLfloat lightTwoColor[]    = { 1.0, 1.0, 1.0, 1.0 };
    
    //some model / light stuff
    ofEnableDepthTest();
    glShadeModel(GL_SMOOTH);
    
//    light.enable();
//    ofEnableSeparateSpecularLight();

    /* initialize lighting */
    glLightfv(GL_LIGHT0, GL_POSITION, lightOnePosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightOneColor);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT1, GL_POSITION, lightTwoPosition);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightTwoColor);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

//    browserWindow.loadImage("images/frame.png");
    
    // 各クラスのセットアップ
    /*
     * Butterfly
     */
    butterfryModelIndex = 0;
    butterfryModelNum = 3;
    
    /*
     * imagePublish
     */
//    for(int i = 0; i < ImagePublishSize; i++){
//        // imagePublishのインスタンスを作る
//        ImagePublish *imagePublishInstance = new ImagePublish( i, "" );
//        imagePublishs.push_back(imagePublishInstance);
//    }

    /*
     * wordSource
     */
    for(int i = 0; i < wordsSize; i++){
        // wordSourceのインスタンスを作る
        cout << i << "\n";
        WordSource *wordInstance = new WordSource( i );
        wordSources.push_back(wordInstance);
    }

    // wordSourceのインスタンスのセットアップ
    for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
        (*it)->setup();
    }
    
    // Max/Mspに読み上げ用のシグナルを送ってから
    // /complete/wordが返ってくるまでの待機時間を指定する
//    wordPlayEndTime = 10000;
//    isTimerReached = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    // cout << receiver.hasWaitingMessages() << "\n";
    
    // OSC メッセージを監視する
    while(receiver.hasWaitingMessages()){
        ofxOscMessage message;
        receiver.getNextMessage(&message);

        // Tweet を検知したタイミングで受信する
        if( message.getAddress() == "/updateStream" ){
            // 受信したテキストを取り出す
            string language = message.getArgAsString(0);
            string text = message.getArgAsString(1);
            
            for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
                // OF Sub App に送信する値をセットする
                (*it)->updatePumpSignal( text );
                (*it)->updateMHlampSignal( text );
                (*it)->setWord( language, text );
            }
        }
        
        
        // from node.js
        if( message.getAddress() == "/addImage" ){
            // 投稿された写真のURL
            string imageURL = message.getArgAsString(0);
            // 投稿された写真のID
            string imageID = message.getArgAsString(1);
            
            cout << "ofApp.h Receive OSC: /addImage imageURL:" << imageURL << "\n";
            cout << "ofApp.h Receive OSC: /addImage imageID:" << imageID << "\n";
            
            // モデルデータをセットアップする
            butterfryModelIndex = (butterfryModelIndex % butterfryModelNum + butterfryModelNum) % butterfryModelNum;
            butterfryModelIndex++;
            Butterfly *bt = new Butterfly(FPS, imageID, (butterfryModelIndex - 1));
            bt->setup();
            bt->addLoadFileName( imageURL );
            // bt->updateVisible("test");
            butterfrys.push_back(bt);
        }
        
        
        // from node.js
        if( message.getAddress() == "/showImage" ){
            // 受信したテキストを取り出す
            string imageID = message.getArgAsString(0);
            
            cout << "ofApp.h Receive OSC: /showImage imageID:" << imageID << "\n";
            
            for(vector <Butterfly *>::iterator it = butterfrys.begin(); it != butterfrys.end(); ++it){
                (*it)->updateVisible( imageID );
            }
        }
        
        
        // from MAX/MSP
        if(message.getAddress() == "/updateScene"){
            // 受信したテキストを取り出す
            int index = message.getArgAsInt32(0);
            
            cout << "ofApp.h Receive OSC: /updateScene SceneID:" << index << "\n";
            
            // シーンのインデックスをセットする
            for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
                (*it)->setSceneIndex( index );
            }
        }


        // Sub App の音声書き出しが完了されたタイミングで受信する
        if(message.getAddress() == "/publish/word") {
            int commandStatusCode = message.getArgAsInt32(0);
            int fileID = message.getArgAsInt32(1);
            string fileName = message.getArgAsString(2);
            
            cout << "ofApp.h Receive OSC: /publish/word commandStatusCode:" << commandStatusCode << "\n";
            cout << "ofApp.h Receive OSC: /publish/word fileID:" << fileID << "\n";
            cout << "ofApp.h Receive OSC: /publish/word fileName:" << fileName << "\n";
            
            
            // 読み上げが終了しない場合のタイムアウトを設定する
            for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
                if( fileID == (*it)->getID() ){
                    (*it)->setTimeOut( 120000 ); // 120秒待つ
                }
            }
            
            
            // コマンドが正常に終了している場合
            if( commandStatusCode == 0 ){
                // Max/Msp に書きだした音声ファイルを送る（/play/word）
                ofxOscMessage sendMessage;
                sendMessage.setAddress( "/play/word" );
                sendMessage.addIntArg( fileID );
                sendMessage.addStringArg( fileName );
                _maxMspAppSender.sendMessage( sendMessage );
            } else {
                for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
                    // 受信したIDと同じIDを持つインスタンスのステータスをOSC受信可能状態にする
                    if( fileID == (*it)->getID() ){
                        bool isGetNewWord      = true;
                        bool isPublishProgress = false;
                        bool isTimerReached    = true;
                        (*it)->updateWordState( isGetNewWord, isPublishProgress, isTimerReached );
                    }
                }
            }
        }
        
        // Max/Msp 音声の再生が終了したタイミングで受信する
        if(message.getAddress() == "/complete/word") {
            int fileID = message.getArgAsInt32(0);
            
            for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
                // 受信したIDと同じIDを持つインスタンスのステータスを
                // OSC受信可能状態にする
                if( fileID == (*it)->getID() ){
                    bool isGetNewWord      = true;
                    bool isPublishProgress = false;
                    bool isTimerReached    = true;
                    (*it)->updateWordState( isGetNewWord, isPublishProgress, isTimerReached );
                }
            }
        }

//        // Webサーバーに画像データが追加されたタイミングで受信する
//        if(message.getAddress() == "/addImage"){
//            // 受信したテキストを取り出す
//            string messageBody = message.getArgAsString(0);
//
//            cout << "OSC: /addImage " << messageBody << "\n";
//
//            imagePublishs[0]->addLoadFileName(messageBody);
//        }
    }

    /*
     * ImagePublish
     */
//    for(vector <ImagePublish *>::iterator it = imagePublishs.begin(); it != imagePublishs.end(); ++it){
//        (*it)->update();
//    }

    /*
     * Butterfly
     */
    for(vector <Butterfly *>::iterator it = butterfrys.begin(); it != butterfrys.end();){
        (*it)->update();
        // 表示する時間を越えていたらインスタンスを削除する
        if( (*it)->getStateDead() ) {
            //オブジェクトを解放
            delete *it;
            //動的配列から、オブジェクトを削除
            it = butterfrys.erase(it);
        } else {
            ++it;
        }
    }
    
    /*
     * WordSource
     */
    for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
        (*it)->update();
    }
    
    
    // Max/Mspに読み上げ用のシグナルを送ってから特定の時間以内に
    // /complete/wordが返ってこない時の処理
//    float timer = ofGetElapsedTimeMillis() - wordPlayStartTime;
//    
//    if( timer >= wordPlayEndTime && !isTimerReached ) {
//        isTimerReached = true;
//        
//        // OSC受信可能状態に戻す
//        for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
//            bool isGetNewWord      = true;
//            bool isPublishProgress = false;
//            (*it)->updateWordState( isGetNewWord, isPublishProgress );
//        }
//            
//        cout << "test\n";
//    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    /*
     * ImagePublish
     */
//    for(vector <ImagePublish *>::iterator it = imagePublishs.begin(); it != imagePublishs.end(); ++it){
//        (*it)->draw();
//    }

    /*
     * Butterfly
     */
    ofEnableDepthTest();
    glShadeModel(GL_SMOOTH);
    light.enable();
    ofEnableSeparateSpecularLight();
    for(vector <Butterfly *>::iterator it = butterfrys.begin(); it != butterfrys.end(); ++it){
        (*it)->draw();
    }
    ofDisableDepthTest();
    light.disable();
    ofDisableLighting();
    ofDisableSeparateSpecularLight();
    
    /*
     * WordSource
     */
    for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
        (*it)->draw();
    }
    
    ofDrawBitmapString("http://54.65.140.79/", 20, 40);
    
//    browserWindow.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'b'){
        // モデルデータのインデックスを進める
        butterfryModelIndex = (butterfryModelIndex % butterfryModelNum + butterfryModelNum) % butterfryModelNum;
        butterfryModelIndex++;
        Butterfly *bt = new Butterfly(FPS, "test", (butterfryModelIndex - 1));
        bt->setup();
        bt->updateVisible("test");
        butterfrys.push_back(bt);
    
        // imagePublishs[0]->addLoadFileName("http://two-tone-cat.c.blog.so-net.ne.jp/_images/blog/_bec/two-tone-cat/calpis201303-2f009.jpg");
    }
    
    if(key == 'a'){
        butterfryModelIndex = (butterfryModelIndex % butterfryModelNum + butterfryModelNum) % butterfryModelNum;
        butterfryModelIndex++;

        cout << butterfryModelIndex << "\n";
    }
    
    if(key == 'f'){
        ofSetFullscreen(true);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
