//
//  ScrolleableT.cpp
//  scrolleableText
//
//  Created by Zaira on 18/02/14.
//
//

#include "ofxScrollable.h"

#define DAMPING 10.
#define MASS 1.
#define K 30.

ofxScrollable::ofxScrollable(){
    mouse=0;
    mouseDiff=0;
    mouseOrigin=0;
    posOrigin=0;
    mouseDown=false;
    
    reset();
}

void ofxScrollable::load(string path, float w, float h, float f){
    width = w;
    height = h;
    
    ofFbo::allocate(width,height,GL_RGBA32F_ARB);
    
    ofAddListener(ofEvents().mousePressed,this,&ofxScrollable::mousePressed);
    ofAddListener(ofEvents().mouseDragged,this,&ofxScrollable::mouseDragged);
    ofAddListener(ofEvents().mouseReleased,this,&ofxScrollable::mouseReleased);
    
    ofPixels imagePixels;
    ofLoadImage(imagePixels, path);
    texWidth = imagePixels.getWidth();
    texHeight = imagePixels.getHeight();
    tex.loadData(imagePixels);
    tex.setAnchorPercent(0.5,0.0);
    
    fadeSize = f;
    ofFloatPixels fadePixels;
    fadePixels.allocate(width,fadeSize,OF_PIXELS_RGBA);
    int i=0;
    for(int y=0;y<(int)fadeSize;y++) {
        for(int x=0;x<(int)width;x++){
            fadePixels[i+0]=0.;
            fadePixels[i+1]=0.;
            fadePixels[i+2]=0.;
            fadePixels[i+3]=1.-(y*y)/(fadeSize*fadeSize); //QUADRATIC_EASE_IN
            i+=4;
        }
    }
    fade.loadData(fadePixels);
}
    
void ofxScrollable::update(){
        
    ofPushStyle();
    ofFbo::begin();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    glBlendFuncSeparate(GL_ONE, GL_SRC_COLOR, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofClear(255,255);
	ofClear(0, 0);
    ofSetColor(255);
    tex.draw(width*0.5,position);
    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    ofPushMatrix();
    ofScale(1.,ofClamp(-1*position/fadeSize,0.,1.));
    fade.draw(0,0);
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(width,height);
    ofRotateZ(-180);
    ofScale(1.,ofClamp((position+texHeight-height)/fadeSize,0.,1.));
    fade.draw(0,0);
    ofPopMatrix();
    ofFbo::end();
    ofPopStyle();
        
    float dt=1./ofGetFrameRate();
        
    if(texHeight<height && !mouseDown){
        destination = 0;
    }
    else if(position>0 && !mouseDown){
        destination = 0;
    }
    else if(position< (height-texHeight) && !mouseDown){
        destination = (height-texHeight);
    }
        
    float accel=destination-position;
    accel*=(K/MASS);
    accel-=(DAMPING/MASS)*velocity;
    velocity+=(accel*dt);
    position+=(velocity*dt);
}
    
void ofxScrollable::setMouseDown(bool m){
    mouseDown = m;
}

void ofxScrollable::mousePressed(ofMouseEventArgs& event){
    if(mouseDown){
        mouse=event.y;
        mouseOrigin=mouse;
        posOrigin=position;
    }
}
    
void ofxScrollable::mouseDragged(ofMouseEventArgs& event){
    if(mouseDown){
        mouseDiff = event.y - mouse;
        mouse = event.y;
        destination = posOrigin + (mouse - mouseOrigin);
    }
}
    
void ofxScrollable::mouseReleased(ofMouseEventArgs& event){
    mouseDown = false;
}

void ofxScrollable::reset(){
    position=0;
    destination=0;
    velocity=0;
}