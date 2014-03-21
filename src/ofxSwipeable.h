//
//  ofxScrollable.h
//  ofxScrollable
//
//  Created by jvelazqueztraut on 18/02/14.
//
//
#pragma once

#include "ofMain.h"

class ofxSwipeable: public ofFbo{
public:
    
    ofxSwipeable();
    
    ~ofxSwipeable(){};
    
    void load(vector<string> path, float w, float h, float f=50.);
    
    void update(float dt);
    
    void draw(int x, int y);
        
    void reset();
            
    bool pressed(ofPoint pos,int ID=0);
    bool dragged(ofPoint pos,int ID=0);
    bool released(ofPoint pos,int ID=0);
    
    void setIndicator(bool i);
            
    float width,height;
    
    int current;
    
    vector<ofTexture> tex;
    float texWidth,texHeight;
    
    float position;
    float destination;
    float velocity;
    
    bool    p;
    int     pID;
    float   pOrigin;
    float   dOrigin;

    bool indicator;
    int indicatorPos;
    float indicatorSize;
    float indicatorGap;
    float indicatorWidth;
    
    ofTexture fade;
    float fadeSize;
};
