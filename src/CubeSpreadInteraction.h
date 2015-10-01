//
//  CubeSpreadInteraction.h
//  SimpleVjSystem
//
//  Created by 岩城勇磨 on 9/13/15.
//
//

#ifndef __SimpleVjSystem__CubeSpreadInteraction__
#define __SimpleVjSystem__CubeSpreadInteraction__

#include <stdio.h>
#include "ofMain.h"

class CubeSpreadInteraction {
public:
    CubeSpreadInteraction(ofVec3f pos);
    ~CubeSpreadInteraction();
    void setup();
    void update(float volume, ofFloatColor color, float multi_size);
    void draw();
    ofVec3f translatePos;
    const static int NUM = 6;
    ofVec3f positons[NUM];
    float Radius[NUM];
    float RadianS[NUM];
    float RadianT[NUM];
    float theta[NUM];
    float addTheta[NUM];
    float boxSize[NUM];
    ofBoxPrimitive box[NUM];
    float a = 0;
    float rotate[NUM];
    
    int LifeLimite;
    ofFloatColor color;
    
    
private:
    
};

#endif /* defined(__SimpleVjSystem__CubeSpreadInteraction__) */
