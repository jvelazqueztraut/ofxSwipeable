//
//  ScrolleableT.cpp
//  scrolleableText
//
//  Created by Zaira on 18/02/14.
//
//

#include "ofxSwipeable.h"

#define DAMPING 10.
#define MASS 1.
#define K 30.

ofxSwipeable::ofxSwipeable(){
    mouse=false;
    mouseOrigin=0;
    desOrigin=0;
    
    indicator = true;
    
    reset();
}

void ofxSwipeable::load(vector<string> path, float w, float h, float f){
    width = w;
    height = h;
    
    ofFbo::allocate(width,height,GL_RGBA32F_ARB);
    
    tex.assign(path.size(),ofTexture());
    for(int i=0;i<path.size();i++){
        ofPixels imagePixels;
        ofLoadImage(imagePixels, path[i]);
        texWidth = imagePixels.getWidth();
        texHeight = imagePixels.getHeight();
        tex[i].loadData(imagePixels);
        tex[i].setAnchorPercent(0.5,0.5);
    }
    
    indicatorPos = 0;
    indicatorSize = 5;
    indicatorGap = 20;
    indicatorWidth = (tex.size()-1) * indicatorGap;
    
    fadeSize = f;
    ofFloatPixels fadePixels;
    fadePixels.allocate(fadeSize,height,OF_PIXELS_RGBA);
    int i=0;
    for(int y=0;y<(int)height;y++) {
        for(int x=0;x<(int)fadeSize;x++){
            fadePixels[i+0]=0.;
            fadePixels[i+1]=0.;
            fadePixels[i+2]=0.;
            fadePixels[i+3]=1.-(x*x)/(fadeSize*fadeSize); //QUADRATIC_EASE_IN
            i+=4;
        }
    }
    fade.loadData(fadePixels);
    
    ofAddListener(ofEvents().mousePressed,this,&ofxSwipeable::mousePressed);
    ofAddListener(ofEvents().mouseDragged,this,&ofxSwipeable::mouseDragged);
    ofAddListener(ofEvents().mouseReleased,this,&ofxSwipeable::mouseReleased);
}
    
void ofxSwipeable::update(){
        
    ofPushStyle();
    ofFbo::begin();
    ofClear(0, 0);
    glBlendFuncSeparate(GL_ONE, GL_SRC_COLOR, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(255);
    ofPushMatrix();
    ofTranslate(position,0);
    for(int i=0;i<tex.size();i++){
        if((position+i*width+width)>0 && (position+i*width)<width){
            tex[i].draw(width*0.5+i*width,height*0.5);
        }
    }
    ofPopMatrix();
    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    ofPushMatrix();
    ofScale(ofClamp(abs(position+width*current)/fadeSize,0.,1.),1.);
    fade.draw(0,0);
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(width,height);
    ofRotateZ(-180);
    ofScale(ofClamp(abs(position+width*current)/fadeSize,0.,1.),1.);
    fade.draw(0,0);
    ofPopMatrix();
    if(indicator){
        glBlendFuncSeparate(GL_ONE, GL_SRC_COLOR, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofFill();
        ofPushMatrix();
        ofTranslate(width*0.5-indicatorWidth*0.5,height*0.95);
        for(int i=0;i<tex.size();i++){
            ofSetColor(127,150);
            ofCircle(i*indicatorGap,0,indicatorSize);
        }
        ofSetColor(127,250);
        ofCircle(indicatorPos,0,indicatorSize);
        ofPopMatrix();
        
        if((indicatorPos-current*indicatorGap)>0)
            indicatorPos-=5;
        else if((indicatorPos-current*indicatorGap)<0)
            indicatorPos+=5;
    }
    ofFbo::end();
    ofPopStyle();
        
    float dt=1./ofGetFrameRate();
        
    float accel=destination-position;
    accel*=(K/MASS);
    accel-=(DAMPING/MASS)*velocity;
    velocity+=(accel*dt);
    position+=(velocity*dt);
}

void ofxSwipeable::setIndicator(bool i){
    indicator = i;
}
    
void ofxSwipeable::setMouse(bool m){
    mouse = m;
}

void ofxSwipeable::mousePressed(ofMouseEventArgs& event){
    if(mouse){
        mouseOrigin=event.x;
        desOrigin=destination;
    }
}
    
void ofxSwipeable::mouseDragged(ofMouseEventArgs& event){
    if(mouse){
        destination = desOrigin + (event.x - mouseOrigin);
    }
}
    
void ofxSwipeable::mouseReleased(ofMouseEventArgs& event){
    if(mouse){
        destination = desOrigin + (event.x - mouseOrigin);
        int d = ceil(abs(destination-desOrigin)/width);
        if((destination-desOrigin)>0)
            d*=-1;
        current = ofClamp(current+d,0,tex.size()-1);
        destination =-current*width;
        mouse = false;
    }
}

void ofxSwipeable::reset(){
    position=0;
    destination=0;
    velocity=0;
    current=0;
}