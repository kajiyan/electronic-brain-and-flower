#include "ImagePublish.h"

#include "Poco/RegularExpression.h"

using Poco::RegularExpression;

//--------------------------------------------------------------
ImagePublish::ImagePublish() {
    cout << "ImagePublish -> constructor\n";
    
    // _ID = ID;
    // _publishPath = publishPath;
    
    _isLoading = false;
    _isLoadingComplete = false;
    _isPublishComplete = false;
    _isConfigFileName = false;
    ofRegisterURLNotification(this);
}


//--------------------------------------------------------------
ImagePublish::~ImagePublish() {
}


//--------------------------------------------------------------
void ImagePublish::setup( int ID, string publishPath ) {
    _ID = ID;
    _publishPath = publishPath;
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
            
            // 読み込みが終了した時のイベント
            ofNotifyEvent(publishComplete, _isLoadingComplete);
        }
    }else{
        cout << response.status << " " << response.error << endl;
        if(response.status != -1){
            _isLoading = false;
            _isLoadingComplete = true;
            ofNotifyEvent(publishComplete, _isLoadingComplete);
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
        
        // ファイルネームが指定されているかでファイル名を分岐
        if( _isConfigFileName ) {
            _image.saveImage( _publishPath + _fileName );
        } else {
            _image.saveImage( _publishPath + publishFileName );
        }
        
        _loadFileNames.erase( _loadFileNames.begin() );
        
        // アルファブレンディングを有効にする
        ofEnableAlphaBlending();
    }
}



//--------------------------------------------------------------
void ImagePublish::setFileName( string fileName ) {
    _isConfigFileName = true;
    _fileName = fileName;
}



//--------------------------------------------------------------
void ImagePublish::addLoadFileName( string fileName ) {
    _loadFileNames.push_back( fileName );
    _isLoadingComplete = false;
}

