#include "testApp.h"

#define FRAME_WIDTH 404
#define FRAME_HEIGHT 504
#define FADE_SIZE 50
//--------------------------------------------------------------
void testApp::setup(){
    vector<string> path;
    path.push_back("text.png");
    path.push_back("text.png");
    path.push_back("text.png");
    path.push_back("text.png");
    path.push_back("text.png");

    text.load(path,FRAME_WIDTH,FRAME_HEIGHT,FADE_SIZE);
    text.setAnchorPercent(0.5,0.5);
}

//--------------------------------------------------------------
void testApp::update(){
    text.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(ofColor::white);
    ofSetColor(255);
    
    text.draw(ofGetWidth()*0.25,ofGetHeight()*0.5);
        
    //Just squares to show the dimensions of the scrollable zone and fade
    ofSetColor(0);
    ofNoFill();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRect(ofGetWidth()*0.25,ofGetHeight()*0.5,text.getWidth(),text.getHeight());
    ofLine(ofGetWidth()*0.25-text.getWidth()*0.5,ofGetHeight()*0.5-text.getHeight()*0.5+FADE_SIZE,ofGetWidth()*0.25+text.getWidth()*0.5,ofGetHeight()*0.5-text.getHeight()*0.5+FADE_SIZE);
    ofLine(ofGetWidth()*0.25-text.getWidth()*0.5,ofGetHeight()*0.5+text.getHeight()*0.5-FADE_SIZE,ofGetWidth()*0.25+text.getWidth()*0.5,ofGetHeight()*0.5+text.getHeight()*0.5-FADE_SIZE);
    
    ofSetRectMode(OF_RECTMODE_CORNER);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(x<ofGetWidth()*0.5){
        text.setMouse(true);
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
