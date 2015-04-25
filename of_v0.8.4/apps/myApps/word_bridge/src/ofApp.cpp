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
    }
    
    // Debug 用の情報を取得するか
    isDebug = true;
    
    GLfloat lightOnePosition[] = { 40.0, 40, 100.0, 0.0 };
    GLfloat lightOneColor[]    = { 0.99, 0.99, 0.99, 1.0 };
    
    GLfloat lightTwoPosition[] = { -40.0, 40, 100.0, 0.0 };
    GLfloat lightTwoColor[]    = { 0.99, 0.99, 0.99, 1.0 };

    // 各クラスのセットアップ
    /*
     * imagePublish
     */
    for(int i = 0; i < ImagePublishSize; i++){
        // imagePublishのインスタンスを作る
        ImagePublish *imagePublishInstance = new ImagePublish( i );
        imagePublishs.push_back(imagePublishInstance);
    }

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
    // OSC メッセージを監視する
    while(receiver.hasWaitingMessages()){
        ofxOscMessage message;
        receiver.getNextMessage(&message);

        // Tweet を検知したタイミングで受信する
        if(message.getAddress() == "/updateStream"){
            // 受信したテキストを取り出す
            string language = message.getArgAsString(0);
            string text = message.getArgAsString(1);

            for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
                // OF Sub App に送信する値をセットする
                (*it)->setWord( language, text );
            }
        }

        // Sub App の音声書き出しが完了されたタイミングで受信する
        if(message.getAddress() == "/publish/word") {
            int fileID = message.getArgAsInt32(0);
            string fileName = message.getArgAsString(1);
            
            cout << "OSC: /publish/word " << fileID << "\n";
            cout << "OSC: /publish/word " << fileName << "\n";
            
            // Max/Msp に書きだした音声ファイルを送る（/play/word）
        }

        // Webサーバーに画像データが追加されたタイミングで受信する
        if(message.getAddress() == "/addImage"){
            // 受信したテキストを取り出す
            string messageBody = message.getArgAsString(0);

            cout << "OSC: /addImage " << messageBody << "\n";

            imagePublishs[0]->addLoadFileName(messageBody);
        }
    }

    /*
     * ImagePublish
     */
    for(vector <ImagePublish *>::iterator it = imagePublishs.begin(); it != imagePublishs.end(); ++it){
        (*it)->update();
    }

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
    for(vector <ImagePublish *>::iterator it = imagePublishs.begin(); it != imagePublishs.end(); ++it){
        (*it)->draw();
    }

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
