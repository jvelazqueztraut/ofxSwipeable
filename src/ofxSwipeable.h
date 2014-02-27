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
    
    void update();
    
    void reset();
        
    void setMouse(bool m);
    
    void mouseDragged(ofMouseEventArgs& event);
    void mousePressed(ofMouseEventArgs& event);
    void mouseReleased(ofMouseEventArgs& event);
    
    void setIndicator(bool i);
            
    float width,height;
    
    int current;
    
    vector<ofTexture> tex;
    float texWidth,texHeight;
    
    float position;
    float destination;
    float velocity;
    
    bool mouse;
    float mouseOrigin;
    float desOrigin;

    bool indicator;
    int indicatorPos;
    float indicatorSize;
    float indicatorGap;
    float indicatorWidth;
    
    ofTexture fade;
    float fadeSize;
};
