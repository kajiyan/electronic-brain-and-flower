#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync( true );
    ofEnableSmoothing();
    ofSetFrameRate( 60.0 );
    ofBackground( 100 );
    ofSetLogLevel( OF_LOG_VERBOSE );
}

//--------------------------------------------------------------
void ofApp::update(){
    for(vector <Butterfly *>::iterator it = butterfrys.begin(); it != butterfrys.end(); ++it){
        (*it)->update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    for(vector <Butterfly *>::iterator it = butterfrys.begin(); it != butterfrys.end(); ++it){
        (*it)->draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == '1') {
        Butterfly *bt = new Butterfly( "test", 0 );
        bt->setup();
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
