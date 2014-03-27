#include "testApp.h"

#define FRAME_WIDTH 404
#define FRAME_HEIGHT 504
#define FADE_SIZE 50
//--------------------------------------------------------------
void testApp::setup(){
    vector<string> texts;
    texts.push_back("Since the initial publication of the chart of the electromagnetic spectrum humans have learnt that what they can touch, smell, see and hear is less than one millionth of reality.");
    texts.push_back("Since the initial publication of the chart of the electromagnetic spectrum humans have learnt that what they can touch, smell, see and hear is less than one millionth of reality.");
    texts.push_back("Since the initial publication of the chart of the electromagnetic spectrum humans have learnt that what they can touch, smell, see and hear is less than one millionth of reality.");
    texts.push_back("Since the initial publication of the chart of the electromagnetic spectrum humans have learnt that what they can touch, smell, see and hear is less than one millionth of reality.");
    texts.push_back("Since the initial publication of the chart of the electromagnetic spectrum humans have learnt that what they can touch, smell, see and hear is less than one millionth of reality.");
    texts.push_back("Since the initial publication of the chart of the electromagnetic spectrum humans have learnt that what they can touch, smell, see and hear is less than one millionth of reality.");
    text.setStyle("DIN-Medium.otf",14,ofColor(255,0,255),20);
    text.load(texts,FRAME_WIDTH,FRAME_HEIGHT,FADE_SIZE);
    text.setAnchorPercent(0.5,0.5);
    
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
    ofBackground(175);
    ofSetColor(255);
    
    //text.draw(ofGetMouseX(),ofGetMouseY());
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
    text.dragged(ofPoint(x,y)-ofPoint(ofGetWidth()*0.25,ofGetHeight()*0.5));
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(x<ofGetWidth()*0.5){
        text.pressed(ofPoint(x,y)-ofPoint(ofGetWidth()*0.25,ofGetHeight()*0.5));
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    text.released(ofPoint(x,y)-ofPoint(ofGetWidth()*0.25,ofGetHeight()*0.5));
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
