//
//  ofxSwipeableText.cpp
//  example
//
//  Created by Wanda on 26/03/14.
//
//

#include "ofxSwipeableText.h"

#define DAMPING 10.
#define MASS 1.
#define K 30.

ofxSwipeableText::ofxSwipeableText(){
    p=false;
    pOrigin=0;
    dOrigin=0;
    
    current = 0;
    
    indicator = true;
    indicatorPos=indicatorVel=0.;
    indicatorSize=5;
    indicatorGap=0.05f;
    indicatorHeight=0.95f;
    
    anchor.set(0.,0.);
    
    string shaderProgram =
    "#extension GL_ARB_texture_rectangle: enable\n\
    \n\
    uniform vec2 offset;\n\
    uniform sampler2DRect fade;\n\
    \n\
    void main (void){\n\
    vec2 pos = gl_FragCoord.xy - offset;\n\
    vec4 color = texture2DRect(fade,pos);\n\
    gl_FragColor = color;\n\
    }";
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    shader.linkProgram();
    
    reset();
}

void ofxSwipeableText::setStyle(string f, int s, ofColor c,int m){
    font = f;
    size = s;
    color = c;
    margin = m;
}

void ofxSwipeableText::load(vector<string> _texts, int w, int h, float f){
    width = w;
    height = h;
    
    //ofFbo::allocate(width,height,GL_RGBA32F_ARB);
    texts.assign(_texts.size(),ofxTextBlock());
    for(int i=0;i<_texts.size();i++){
        texts[i].init(font, size);
        texts[i].setText(_texts[i]);
        texts[i].wrapTextX(width-margin*2);
    }
    
    indicators.assign(texts.size(),0);
    for(int i=0;i<indicators.size();i++){
        indicators[i]=(i-0.5*indicators.size())*(width*indicatorGap);
    }
    indicatorPos=indicators[current];
    
    fadeSize = f;
    fadePixels.allocate(width,1,OF_PIXELS_RGBA);
    int i=0;
    for(int x=0;x<width;x++){
        fadePixels[i+0]=color.r/255.;
        fadePixels[i+1]=color.g/255.;
        fadePixels[i+2]=color.b/255.;
        fadePixels[i+3]=1.;
        i+=4;
    }
    fadePixels[0+3]=0.;
    fadePixels[(i-4)+3]=0.;
}

void ofxSwipeableText::update(float dt){
    fade = ofClamp(abs(position+width*current),0.,fadeSize);
    int i=0;
    for(int x=0;x<(int)fade;x++){
        fadePixels[i+3]=(x*x)/(fade*fade); //QUADRATIC_EASE_IN
        i+=4;
    }
    for(int x=(int)fade;x<width-(int)fade;x++){
        fadePixels[i+3]=1.;
        i+=4;
    }
    for(int x=(width-(int)fade);x<width;x++){
        int xInv=width-x;
        fadePixels[i+3]=(xInv*xInv)/(fade*fade); //QUADRATIC_EASE_OUT
        i+=4;
    }
    fadePixels[0+3]=0.;
    fadePixels[(i-4)+3]=0.;
    fadeTex.loadData(fadePixels);
    
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
    
    GLfloat m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    reference = ofMatrix4x4(m);
}

void ofxSwipeableText::draw(int x, int y){
    ofPushStyle();
    ofSetColor(255);
    ofFill();
    ofPushMatrix();
    ofTranslate(x-anchor.x*width,y-anchor.y*height);
    
    GLfloat m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    ofMatrix4x4 mat(m);
    
    ofPushMatrix();
    ofTranslate(position,0);
    shader.begin();
    shader.setUniformTexture("fade", fadeTex, 0);
    ofVec3f offset = (ofPoint(0,0) * reference.getInverse()) * mat;
    shader.setUniform2f("offset",offset.x,offset.y);
    for(int i=0;i<texts.size();i++){
        if((position+i*width+width)>0 && (position+i*width)<width){
            texts[i].drawJustified(margin+i*width,0.5*height-0.5*texts[i].getHeight(),width-margin*2);
        }
    }
    shader.end();
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
    
    ofPopMatrix();
    ofPopStyle();
}

void ofxSwipeableText::setIndicator(bool i){
    indicator = i;
}

void ofxSwipeableText::setIndicatorStyle(float h, float s, float g){
    indicatorHeight=h;
    indicatorSize=s;
    indicatorGap=g;
}

float ofxSwipeableText::getWidth(){
    return width;
}

float ofxSwipeableText::getHeight(){
    return height;
}

void ofxSwipeableText::setAnchorPercent(float xPct, float yPct){
    anchor.set(xPct,yPct);
}

bool ofxSwipeableText::pressed(ofPoint pos, int ID){
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

bool ofxSwipeableText::dragged(ofPoint pos, int ID){
    if(p && pID==ID){
        pos+=ofPoint(anchor.x*width,anchor.y*height);
        destination = dOrigin + (pos.x - pOrigin);
        return true;
    }
    return false;
}

bool ofxSwipeableText::released(ofPoint pos, int ID){
    if(p && pID==ID){
        pos+=ofPoint(anchor.x*width,anchor.y*height);
        destination = dOrigin + (pos.x - pOrigin);
        int d = round(abs(destination-dOrigin)/width);
        if((destination-dOrigin)>0)
            d*=-1;
        current = ofClamp(current+d,0,texts.size()-1);
        destination =-current*width;
        p = false;
        return true;
    }
    return false;
}

void ofxSwipeableText::reset(){
    position=0;
    destination=0;
    velocity=0;
    current=0;
    p=false;
}