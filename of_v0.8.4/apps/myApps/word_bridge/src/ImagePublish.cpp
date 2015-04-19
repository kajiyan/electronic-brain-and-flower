//
//  ImagePublish.cpp
//  word_bridge
//
//  Created by KAJIYA HIROKI on 2015/04/19.
//
//

#include "ImagePublish.h"

//--------------------------------------------------------------
ImagePublish::ImagePublish( int ID ) {
    cout << "WordSource -> constructor\n";
    
    _ID = ID;
    _loading = false;
    _loadingComplete = false;
    ofRegisterURLNotification(this);
}


//--------------------------------------------------------------
void ImagePublish::update() {
    // ローディングが完了していなければ画像を読み込む
    if(!_loading && !_loadingComplete){
        _loading = true;
        _image.clear();
        ofLoadURLAsync("http://images.wildmadagascar.org/pictures/bemaraha/tsingy_forest.JPG", "tsingy_forest");
    }
}


//--------------------------------------------------------------
void ImagePublish::draw() {
    if(_image.bAllocated()){
        ofSetColor(255);
        _image.draw(0, 0);
    }
}


//--------------------------------------------------------------
void ImagePublish::urlResponse(ofHttpResponse & response){
    if(response.status == 200 && response.request.name == "tsingy_forest"){
        _image.loadImage(response.data);
        _loading = false;
        _loadingComplete = true;
    }else{
        cout << response.status << " " << response.error << endl;
        if(response.status != -1){
            _loading = false;
            _loadingComplete = true;
        }
    }
}


