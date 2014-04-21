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
    p=false;
    pOrigin=0;
    dOrigin=0;
    
    current = 0;
    
    indicator = true;
    indicatorPos=indicatorVel=0.;
    indicatorSize=6;
    indicatorGap=0.06f;
    indicatorHeight=0.96f;
        
    reset();
}

void ofxSwipeable::load(vector<string> path, float w, float h, float f){
    vector<ofPixels> pix;
    pix.assign(path.size(),ofPixels());
    for(int i=0;i<path.size();i++){
        ofLoadImage(pix[i], path[i]);
    }
    load(pix,w,h,f);
}

void ofxSwipeable::load(vector<ofPixels> pix, float w, float h, float f){
    width = w;
    height = h;
    
    ofFbo::allocate(width,height,GL_RGBA32F_ARB);
    
    tex.assign(pix.size(),ofTexture());
    for(int i=0;i<pix.size();i++){
        tex[i].loadData(pix[i]);
        texWidth = tex[i].getWidth();
        texHeight = tex[i].getHeight();
        tex[i].setAnchorPercent(0.5,0.5);
    }
    
    indicators.assign(tex.size(),0);
    for(int i=0;i<indicators.size();i++){
        indicators[i]=(i-0.5*indicators.size())*(width*indicatorGap);
    }
    indicatorPos=indicators[current];
    if(indicators.size()<=1) indicator=false;
    
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
}
    
void ofxSwipeable::update(float dt){
    ofPushStyle();
    ofFbo::begin();
    ofClear(0, 0);
    glBlendFuncSeparate(GL_ONE, GL_SRC_COLOR, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofSetColor(255);
    ofPushMatrix();
    ofTranslate(position,0);
    for(int i=0;i<tex.size();i++){
        if((position+i*width+width)>0 && (position+i*width)<width){
            tex[i].draw(width*0.5+i*width,height*0.5);
        }
    }
    ofPopMatrix();
    if(indicator){
        ofFill();
        ofPushMatrix();
        ofTranslate(width*0.5,height*indicatorHeight);
        for(int i=0;i<indicators.size();i++){
            ofSetColor(255,150);
            ofCircle(indicators[i],0,indicatorSize);
        }
        ofSetColor(255,250);
        ofCircle(indicatorPos,0,indicatorSize);
        ofPopMatrix();
    }
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
    ofFbo::end();
    ofPopStyle();
        
    float accel=destination-position;
    accel*=(K/MASS);
    accel-=(DAMPING/MASS)*velocity;
    velocity+=(accel*dt);
    position+=(velocity*dt);
    
    if(indicator){
        float indicatorAccel=indicators[current]-indicatorPos;
        indicatorAccel*=(K/MASS);
        indicatorAccel-=(DAMPING/MASS)*indicatorVel;
        indicatorVel+=(indicatorAccel*dt);
        indicatorPos+=(indicatorVel*dt);
    }
}

void ofxSwipeable::draw(int x, int y){
    ofFbo::draw(x,y);
}

void ofxSwipeable::setAnchorPercent(float xPct, float yPct){
    anchor.set(xPct,yPct);
    ofFbo::setAnchorPercent(xPct, yPct);
}

void ofxSwipeable::setIndicator(bool i){
    indicator = i;
}

void ofxSwipeable::setIndicatorStyle(float h, float s, float g){
    indicatorHeight=h;
    indicatorSize=s;
    indicatorGap=g;
}

void ofxSwipeable::setCurrent(int c){
    if(c>=0 && c<tex.size()){
        current = c;
        destination =-current*width;
    }
}

int ofxSwipeable::getCurrent(){
    return current;
}

bool ofxSwipeable::isTransitioning(){
	return ((abs(position-destination)>1) && (velocity>1./ofGetFrameRate()));
}
    
bool ofxSwipeable::pressed(ofPoint pos, int ID){
    pos+=ofPoint(anchor.x*width,anchor.y*height);
    if(indicator && abs(pos.y-height*indicatorHeight)<indicatorSize){
        for(int i=0;i<indicators.size();i++){
            if(abs(pos.x-width*0.5-indicators[i])<indicatorSize){
                current = i;
                destination =-current*width;
                return true;
            }
        }
    }
    p = true;
    pID = ID;
    pOrigin=pos.x;
    dOrigin=destination;
    return true;
}
    
bool ofxSwipeable::dragged(ofPoint pos, int ID){
    if(p && pID==ID){
        pos+=ofPoint(anchor.x*width,anchor.y*height);
        destination = dOrigin + (pos.x - pOrigin);
        return true;
    }
    return false;
}
    
bool ofxSwipeable::released(ofPoint pos, int ID){
    if(p && pID==ID){
        pos+=ofPoint(anchor.x*width,anchor.y*height);
        destination = dOrigin + (pos.x - pOrigin);
        float diff = abs(destination-dOrigin)/width;
		int d = diff;
		if((diff-d)>=0.5)
			d++;
        if((destination-dOrigin)>0)
            d*=-1;
        current = ofClamp(current+d,0,tex.size()-1);
        destination =-current*width;
        p = false;
        return true;
    }
    return false;
}

void ofxSwipeable::reset(){
    position=0;
    destination=0;
    velocity=0;
    current=0;
    p=false;
}