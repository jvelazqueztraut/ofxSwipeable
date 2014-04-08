//
//  ofxScrollable.h
//  ofxScrollable
//
//  Created by jvelazqueztraut on 18/02/14.
//
//
#pragma once

#include "ofMain.h"

class ofxSwipeable{
public:
    
    ofxSwipeable();
    
    ~ofxSwipeable(){};
    
    void load(vector<ofPixels> pix, float w, float h, float f=50.);
    void load(vector<string> path, float w, float h, float f=50.);
    
    void update(float dt);
    
    void draw(int x, int y);
        
    void reset();
            
    bool pressed(ofPoint pos,int ID=0);
    bool dragged(ofPoint pos,int ID=0);
    bool released(ofPoint pos,int ID=0);
    
    float getWidth();
    float getHeight();
    
    void setAnchorPercent(float xPct, float yPct);
    
    void setIndicator(bool i);
    void setIndicatorStyle(float h, float s, float g);
    
    int getCurrent();
    
private:
    float width,height;
    int current;
    ofVec2f anchor;
    
    float position;
    float destination;
    float velocity;
    
    bool    p;
    int     pID;
    float   pOrigin;
    float   dOrigin;
    
    bool indicator;
    vector<int> indicators;
    float indicatorPos;
    float indicatorVel;
    float indicatorSize;
    float indicatorGap;
    float indicatorHeight;
    
    float fadeSize;
    float fade;
    ofFloatPixels fadePixels;
    ofTexture fadeTex;
    ofShader shader;
    
    ofMatrix4x4 reference;
    
    vector<ofTexture> tex;
};
