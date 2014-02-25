//
//  ofxScrollable.h
//  ofxScrollable
//
//  Created by jvelazqueztraut on 18/02/14.
//
//
#pragma once

#include "ofMain.h"

class ofxScrollable: public ofFbo{
public:
    
    ofxScrollable();
    
    ~ofxScrollable(){};
    
    void load(string path, float w, float h, float f=50.);
    
    void update();
    
    void reset();
        
    void setMouseDown(bool m);
    
    void mouseDragged(ofMouseEventArgs& event);
    void mousePressed(ofMouseEventArgs& event);
    void mouseReleased(ofMouseEventArgs& event);
            
    float width,height;
    
    ofTexture tex;
    float texWidth,texHeight;
    
    float position;
    float destination;
    float velocity;
    
    float mouse;
    float mouseDiff;
    float mouseOrigin;
    float posOrigin;
    bool mouseDown;
    
    ofTexture fade;
    float fadeSize;
};
