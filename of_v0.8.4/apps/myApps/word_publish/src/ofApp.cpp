#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    /*
     * Basic Configuration
     */
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetFrameRate(FPS);
    ofSetWindowTitle("電脳と花 - Sub App");
    
    // 設定ファイルを取得する
    setting.open("config.json");
    ofLogNotice("ofApp::setup") << setting.getRawString();

    /*
     * CommandRun
     */
    for(int i = 0; i < commandRunSize; i++){
        CommandRun *commandRunInstance = new CommandRun( i );
        commandRuns.push_back(commandRunInstance);
    }
    for(vector <CommandRun *>::iterator it = commandRuns.begin(); it != commandRuns.end(); ++it) {
        (*it)->setup();
    }
    
    // OSCを受信するポートの設定
    receiver.setup(setting["address"]["openFrameWorks"]["sub"]["port"].asInt());

    cout << ofFilePath::getAbsolutePath(ofToDataPath("")) << "\n";
}

//--------------------------------------------------------------
void ofApp::update(){
    // OSC メッセージを監視する
    while(receiver.hasWaitingMessages()){
        ofxOscMessage message;
        receiver.getNextMessage(&message);
        
        // 書き出し対象のTweetを受信する
        if(message.getAddress() == "/add/word"){
            string voiseActor = message.getArgAsString(0);
            string word = message.getArgAsString(1);
            int uniqueID = message.getArgAsInt32(2);
            int fileID = message.getArgAsInt32(3);
            
            cout << voiseActor << "\n";
            cout << word << "\n";
            cout << uniqueID << "\n";
            cout << fileID << "\n";
            
            for(vector <CommandRun *>::iterator it = commandRuns.begin(); it != commandRuns.end(); ++it) {
                (*it)->createSoundFile( voiseActor, word, uniqueID, fileID );
            }
        }
    }
    
    /*
     * CommandRun
     */
    for(vector <CommandRun *>::iterator it = commandRuns.begin(); it != commandRuns.end(); ++it) {
        (*it)->update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

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
