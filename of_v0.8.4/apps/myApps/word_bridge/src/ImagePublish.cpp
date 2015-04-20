#include "ImagePublish.h"

#include "Poco/RegularExpression.h"

using Poco::RegularExpression;

//--------------------------------------------------------------
ImagePublish::ImagePublish( int ID ) {
    cout << "WordSource -> constructor\n";
    
    _ID = ID;
    _isLoading = false;
    _isLoadingComplete = false;
    _isPublishComplete = false;
    ofRegisterURLNotification(this);
}


//--------------------------------------------------------------
void ImagePublish::update() {
    // ローディングが完了していなければ画像を読み込む
    if(!_isLoading && !_isLoadingComplete && _loadFileNames.size() > 0){
        cout << "ImagePublish -> update\n" << _loadFileNames.front();
        
        _isLoading = true;
        _image.clear();
        
        // 先頭からファイル名を取得する
        ofLoadURLAsync( _loadFileNames.front() );
    }
}


//--------------------------------------------------------------
void ImagePublish::draw() {
}


//--------------------------------------------------------------
void ImagePublish::urlResponse(ofHttpResponse & response){
    if(response.status == 200){
        _image.loadImage(response.data);
        
        // 画像を書き出す
        _publish();
        
        _isLoading = false;
        // すべてのファイルを読み込んだ時の処理
        if( _loadFileNames.size() == 0 ){
            _isLoadingComplete = true;
        }
    }else{
        cout << response.status << " " << response.error << endl;
        if(response.status != -1){
            _isLoading = false;
            _isLoadingComplete = true;
        }
    }
}


//--------------------------------------------------------------
void ImagePublish::_publish(){
    if(_image.bAllocated()){
        ofSetColor(255);
        
        // アルファブレンディングを無効にする
        ofDisableAlphaBlending();
        
        RegularExpression fileNameMatch("([^/]+?)([\?#].*)?$");
        string publishFileName;
        fileNameMatch.extract(_loadFileNames.front(), publishFileName);
        
        _image.saveImage( publishFileName );
        _loadFileNames.erase( _loadFileNames.begin() );
        
        // アルファブレンディングを有効にする
        ofEnableAlphaBlending();
    }
}


//--------------------------------------------------------------
void ImagePublish::addLoadFileName( string fileName ) {
    _loadFileNames.push_back( fileName );
    _isLoadingComplete = false;
}

