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
    indicatorSize=5;
    indicatorGap=0.06f;
    indicatorHeight=0.95f;
    
    anchor.set(0.,0.);
    
    string shaderProgram =
    "#extension GL_ARB_texture_rectangle: enable\n\
    \n\
    uniform vec2 offset;\n\
    uniform sampler2DRect fade;\n\
    uniform sampler2DRect tex;\n\
    \n\
    void main (void){\n\
    vec2 posTex = gl_TexCoord[0].st;\n\
    vec2 posFade = gl_TexCoord[0].st + offset;\n\
    vec4 colorTex = texture2DRect(tex,posTex);\n\
    vec4 colorFade = texture2DRect(fade,posFade);\n\
    gl_FragColor = vec4( colorTex.rgb , colorTex.a * colorFade.a);\n\
    }";
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    shader.linkProgram();
    
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
    
    //ofFbo::allocate(width,height,GL_RGBA32F_ARB);
    
    tex.assign(pix.size(),ofTexture());
    for(int i=0;i<pix.size();i++){
        tex[i].loadData(pix[i]);
        tex[i].setAnchorPercent(0.5,0.5);
    }
    
    indicators.assign(tex.size(),0);
    for(int i=0;i<indicators.size();i++){
        indicators[i]=(i-0.5*indicators.size())*(width*indicatorGap);
    }
    indicatorPos=indicators[current];
    if(indicators.size()<=1)indicator=false;
    
    fadeSize = f;
    fadePixels.allocate(width,1,OF_PIXELS_RGBA);
    int i=0;
    for(int x=0;x<width;x++){
        fadePixels[i+0]=1.;
        fadePixels[i+1]=1.;
        fadePixels[i+2]=1.;
        fadePixels[i+3]=1.;
        i+=4;
    }
    fadePixels[0+3]=0.;
    fadePixels[(i-4)+3]=0.;
}

void ofxSwipeable::update(float dt){
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
    
    /*GLfloat m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    reference = ofMatrix4x4(m);*/
}

void ofxSwipeable::draw(int x, int y){
    ofPushStyle();
    ofSetColor(255);
    ofPushMatrix();
    ofTranslate(x-anchor.x*width,y-anchor.y*height);
    
    /*GLfloat mFade[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mFade);
    ofMatrix4x4 matFade(mFade);
    ofVec3f offsetFade = (ofPoint(0,0) * reference.getInverse()) * matFade;*/
    
    ofPushMatrix();
    ofTranslate(position,0);
    for(int i=0;i<tex.size();i++){
        if((position+i*width+width)>0 && (position+i*width)<width){
            shader.begin();
            shader.setUniform2f("offset",position+width*i,0);
            shader.setUniformTexture("fade", fadeTex, 1);
            shader.setUniformTexture("tex", tex[i], 0);
            tex[i].draw(width*0.5+i*width,height*0.5);
            shader.end();
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
    
    ofPopMatrix();
    ofPopStyle();
}

void ofxSwipeable::setIndicator(bool i){
    indicator = i;
}

void ofxSwipeable::setIndicatorStyle(float h, float s, float g){
    indicatorHeight=h;
    indicatorSize=s;
    indicatorGap=g;
}

float ofxSwipeable::getWidth(){
    return width;
}

float ofxSwipeable::getHeight(){
    return height;
}

int ofxSwipeable::getCurrent(){
    return current;
}

void ofxSwipeable::setAnchorPercent(float xPct, float yPct){
    anchor.set(xPct,yPct);
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
        int d = round(abs(destination-dOrigin)/width);
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