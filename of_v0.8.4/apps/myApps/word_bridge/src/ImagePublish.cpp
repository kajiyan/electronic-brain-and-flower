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
    _isLoading = false;
    _isLoadingComplete = false;
    _isPublishComplete = false;
    ofRegisterURLNotification(this);
    
    // string *loadFileName = new string( "i" );
    // _loadFileNames.push_back(loadFileName);
    string loadFileName = "i";
    _loadFileNames.push_back(loadFileName);

    for (int i = 0; i < _loadFileNames.size(); i++) {
        cout << _loadFileNames[i] << "\n";
    }
    
    //    imagePublishs.push_back(imagePublishInstance);
}


//--------------------------------------------------------------
void ImagePublish::update() {
    // ローディングが完了していなければ画像を読み込む
    if(!_isLoading && !_isLoadingComplete){
        _isLoading = true;
        _image.clear();
        ofLoadURLAsync("http://images.wildmadagascar.org/pictures/bemaraha/tsingy_forest.JPG", "material");
    }
}


//--------------------------------------------------------------
void ImagePublish::draw() {
    if(_image.bAllocated()){
        ofSetColor(255);
        
        // アルファブレンディングを無効にする
        ofDisableAlphaBlending();
        
        // 画像を書き出す
        if(_isPublishComplete) {
            _image.draw(0, 0);
        } else {
            _image.setImageType(OF_IMAGE_GRAYSCALE);
            _image.saveImage("test.jpg");
            _image.loadImage("test.jpg");
            _isPublishComplete = true;
        }
        
        // アルファブレンディングを有効にする
        ofEnableAlphaBlending();
    }
}


//--------------------------------------------------------------
void ImagePublish::urlResponse(ofHttpResponse & response){
    if(response.status == 200 && response.request.name == "material"){
        _image.loadImage(response.data);
        _isLoading = false;
        _isLoadingComplete = true;
    }else{
        cout << response.status << " " << response.error << endl;
        if(response.status != -1){
            _isLoading = false;
            _isLoadingComplete = true;
        }
    }
}


