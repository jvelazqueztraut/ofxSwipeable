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
    
    position.set(ofGetWidth()*0.25,ofGetHeight()*0.5);
    
    time = 0;
}

//--------------------------------------------------------------
void testApp::update(){
    float t = ofGetElapsedTimef();
    float dt = t -time;
    time = t;
    text.update(dt);
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(ofColor::grey);
    ofSetColor(255);
    
    text.draw(position.x,position.y);
        
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
    text.dragged(ofPoint(x,y)-position);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(x<ofGetWidth()*0.5){
        text.pressed(ofPoint(x,y)-position);
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    text.released(ofPoint(x,y)-position);
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
