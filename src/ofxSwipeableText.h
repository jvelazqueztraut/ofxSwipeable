//
//  ofxSwipeableText.h
//  example
//
//  Created by Wanda on 26/03/14.
//
//
#pragma once

#include "ofMain.h"
#include "ofxTextSuite.h"

class ofxSwipeableText{
public:
    
    ofxSwipeableText();
    
    ~ofxSwipeableText(){};
    
    void load(vector<string> _texts, int w, int h, float f=50.);
    void setStyle(string f, int s, ofColor c,int m);
    
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
    
    vector<ofxTextBlock> texts;
    string font;
    int size;
    ofColor color;
    int margin;
};
