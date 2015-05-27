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
        
        cout << setting["address"]["openFrameWorks"]["main"]["port"].asInt() << "\n";

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
    GLfloat lightOneColor[]    = { 0.99, 0.99, 0.99, 1.0 };
    
    GLfloat lightTwoPosition[] = { -40.0, 40, 100.0, 0.0 };
    GLfloat lightTwoColor[]    = { 0.99, 0.99, 0.99, 1.0 };
    
    //some model / light stuff
    ofEnableDepthTest();
    glShadeModel (GL_SMOOTH);

    /* initialize lighting */
    glLightfv (GL_LIGHT0, GL_POSITION, lightOnePosition);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, lightOneColor);
    glEnable (GL_LIGHT0);
    glLightfv (GL_LIGHT1, GL_POSITION, lightTwoPosition);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, lightTwoColor);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
    glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable (GL_COLOR_MATERIAL);

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
        
//            imagePublishs[0]->addLoadFileName( imageURL );
        }
        
        
        // from node.js
        if( message.getAddress() == "/showImage" ){
            // 受信したテキストを取り出す
            string imageID = message.getArgAsString(0);
            
            cout << "ofApp.h Receive OSC: /showImage imageID:" << imageID << "\n";
            
//            for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
//                
//            }
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
            
            // Max/Msp に書きだした音声ファイルを送る（/play/word）
            ofxOscMessage sendMessage;
            sendMessage.setAddress( "/play/word" );
            sendMessage.addIntArg( fileID );
            sendMessage.addStringArg( fileName );
            _maxMspAppSender.sendMessage( sendMessage );
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
                    (*it)->updateWordState( isGetNewWord, isPublishProgress );
                }
            }
        }

        // Webサーバーに画像データが追加されたタイミングで受信する
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
    for(vector <Butterfly *>::iterator it = butterfrys.begin(); it != butterfrys.end(); ++it){
        (*it)->update();
    }
    
    /*
     * WordSource
     */
    for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
        (*it)->update();
    }
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
    for(vector <Butterfly *>::iterator it = butterfrys.begin(); it != butterfrys.end(); ++it){
        (*it)->draw();
    }

    /*
     * WordSource
     */
    for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
        (*it)->draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'b'){
        // モデルデータのインデックスを進める
        butterfryModelIndex = (butterfryModelIndex % butterfryModelNum + butterfryModelNum) % butterfryModelNum;
        butterfryModelIndex++;
        Butterfly *bt = new Butterfly(FPS, "test", (butterfryModelIndex - 1));
        bt->updateVisible("test");
        butterfrys.push_back(bt);
    
        // imagePublishs[0]->addLoadFileName("http://two-tone-cat.c.blog.so-net.ne.jp/_images/blog/_bec/two-tone-cat/calpis201303-2f009.jpg");
    }
    
    if(key == 'a'){
        butterfryModelIndex = (butterfryModelIndex % butterfryModelNum + butterfryModelNum) % butterfryModelNum;
        butterfryModelIndex++;

        cout << butterfryModelIndex << "\n";
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
