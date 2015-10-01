//
//  Totems.h
//  SimpleVjSystem
//
//  Created by 岩城勇磨 on 9/13/15.
//
//

#ifndef __SimpleVjSystem__Totems__
#define __SimpleVjSystem__Totems__

#include <stdio.h>
#include "ofMain.h"

class Totems {
public:
    Totems(ofVec3f _translatePos);
    void draw();
    void update(ofFloatColor _color, float volume, float _multi_r);
    ofMesh mesh;
    ofVbo vbo;
    vector<ofVec3f> positons_vector;
    
    static const int NUM = 4000;
    int HEIGHT_NUM = 100;
    ofVec3f positons[NUM];
    ofFloatColor colors[NUM];
    //float radius[NUM];
    vector<float> radius;
    float radianS[NUM];
    vector<float>  radianT;
    vector<float> t_x;
    vector<float> t_z;
    vector<float> t_y;
    float y;
    ofVec3f translatePos;
    float randomX, randomY, randomZ;
    ofFloatColor color;
private:
};

#endif /* defined(__SimpleVjSystem__Totems__) */
