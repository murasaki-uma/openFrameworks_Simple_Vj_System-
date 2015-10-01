//
//  CubeSpreadInteraction.cpp
//  SimpleVjSystem
//
//  Created by 岩城勇磨 on 9/13/15.
//
//

#include "CubeSpreadInteraction.h"

CubeSpreadInteraction::CubeSpreadInteraction(ofVec3f pos) {
    translatePos = pos;
    for (int i = 0; i < NUM; i++) {
        RadianS[i] = ofRandom(180);
        RadianT[i] = ofRandom(360);
        Radius[i] = 30;
        positons[i] = ofVec3f(Radius[i] * sin(RadianS[i]) * cos(RadianT[i]), Radius[i] * sin(RadianS[i]) * sin(RadianT[i]), Radius[i] * cos(RadianS[i]));
        rotate[i] = ofRandom(0, 360);
        boxSize[i] = ofRandom(20,40);
        box[i].set(boxSize[i], boxSize[i], boxSize[i]);
        box[i].setPosition(positons[i]);
    }
    
    LifeLimite = ofRandom(50,100);
    color = ofFloatColor(ofRandom(0.0, 0.5), ofRandom(0.0, 0.5), ofRandom(0.0, 0.5), 1.0);
    
}

CubeSpreadInteraction::~CubeSpreadInteraction() {
    
}

void CubeSpreadInteraction::setup() {
    
}

void CubeSpreadInteraction::update(float volume, ofFloatColor _color, float mluti_size){
    LifeLimite --;
    //color = _color;
    for (int i = 0; i < NUM; i++) {
        rotate[i] += ofRandom(-3, 3);
        theta[i] += ofRandom(0.001, 0.05);
        float size = boxSize[i] * sin(ofSignedNoise(theta[i]));
        Radius[i] ++;
        RadianT[i]+= 0.1;
        positons[i] = ofVec3f((Radius[i]*mluti_size) * sin(RadianS[i]) * cos(RadianT[i]),
                                  (Radius[i]*mluti_size) * sin(RadianS[i]) * sin(RadianT[i]),
                                  (Radius[i]*mluti_size) * cos(RadianS[i]));
        
        box[i].setPosition(positons[i]);
        box[i].set(size*mluti_size);
        
        color += _color*0.5;
    }
    
    
}

void CubeSpreadInteraction::draw(){
    for (int i = 0; i < NUM; i++) {
        ofPushMatrix();
        ofSetColor(color);
        //ofRotate(ofGetElapsedTimef()*10);
        ofRotateX(rotate[i]);
        //ofTranslate(translatePos);
        box[i].drawWireframe();
        ofPopMatrix();
    }
}