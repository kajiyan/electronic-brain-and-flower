#include "ofApp.h"


#include "Poco/String.h"
#include "Poco/RegularExpression.h"

using Poco::replace;
using Poco::RegularExpression;


GLfloat lightOnePosition[] = {40.0, 40, 100.0, 0.0};
GLfloat lightOneColor[] = {0.99, 0.99, 0.99, 1.0};

GLfloat lightTwoPosition[] = {-40.0, 40, 100.0, 0.0};
GLfloat lightTwoColor[] = {0.99, 0.99, 0.99, 1.0};


//--------------------------------------------------------------
void ofApp::setup(){    
    /*
     * Basic Configuration
     */
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetFrameRate(FPS);
    ofSetWindowTitle("電脳と花");
    
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

    
    ofBackground(100, 100, 100);
    
    // 設定ファイルを取得する
    setting.open("config.json");
    ofLogNotice("ofApp::setup") << setting.getRawString();
    
    /*
     * imagePublish
     */
    for(int i = 0; i < wordsSize; i++){
        // wordSourceのインスタンスを作る
        ImagePublish *imagePublishInstance = new ImagePublish( i );
        imagePublishs.push_back(imagePublishInstance);
    }
    
    /*
     * wordSource
     */
    for(int i = 0; i < wordsSize; i++){
        // wordSourceのインスタンスを作る
        WordSource *wordInstance = new WordSource( i );
        wordSources.push_back(wordInstance);
    }
    
    // wordSourceのインスタンスのセットアップ
    for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
        (*it)->setup();
        (*it)->update();
    }

    
    // OSCを受信するポートの設定
    receiver.setup(setting["address"]["openFrameWorks"]["main"]["port"].asInt());
}


//--------------------------------------------------------------
void ofApp::update(){
    // OSC メッセージを監視する
    while(receiver.hasWaitingMessages()){
        ofxOscMessage message;
        receiver.getNextMessage(&message);
        
        if(message.getAddress() == "/word"){
            // 受信したテキストを取り出す
            string messageBody = message.getArgAsString(0);
            
            for(vector <WordSource *>::iterator it = wordSources.begin(); it != wordSources.end(); ++it) {
                (*it)->update();
            }
            
            cout << wordValidation(messageBody) << "\n";
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

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'b'){
        Butterfly *bt = new Butterfly(FPS);
        butterfrys.push_back(bt);
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

//--------------------------------------------------------------
string ofApp::wordValidation(string message){
    // 正規表現で余計な文字列を削除する
    // 改行コードのパターン
    RegularExpression bReakEx1("\r\n");
    bReakEx1.subst(message, "");
    
    RegularExpression bReakEx2("\r");
    bReakEx2.subst(message, "");
    
    RegularExpression bReakEx3("\n");
    bReakEx3.subst(message, "");
    
    // 絵文字のパターン
    RegularExpression pictogramEx("[\xF0-\xF7][\x80-\xBF][\x80-\xBF][\x80-\xBF]");
    pictogramEx.subst(message, "");
    
    // Twitterアカウントのパターン
    RegularExpression accountEx("@([A-Za-z]+[A-Za-z0-9_]+)");
    accountEx.subst(message, "");
    
    // URLのパターン
    RegularExpression urlEx("(https?|ftp)(://[-_.!~*\'()a-zA-Z0-9;/?:@&=+$,%#]+)");
    urlEx.subst(message, "");
    
    // メールアドレスのパターン
    RegularExpression mailEx("[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9-]+(?:.[a-zA-Z0-9-]+)*");
    mailEx.subst(message, "");
    
    return message;
}























