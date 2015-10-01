//
//  Totems.cpp
//  SimpleVjSystem
//
//  Created by 岩城勇磨 on 9/13/15.
//
//

#include "Totems.h"

Totems::Totems(ofVec3f _translatePos){
    translatePos = _translatePos;
    //NUM = ofRandom(1500, 2000);
    y = 0;
    randomX = ofRandom(-300, 300);
    randomY = ofRandom(-300, 300);
    randomZ = ofRandom(-300, 300);
    //radius = 20;
    /*
    for(int s = 0; s <= 180; s += 1){
        radianS = s;
        // 0 <= s <= 180 なので -1 <= cos(radianS) <= 1
        // よって z は -radius <= z <= radius
        float y = radius * cos(radianS);
        for(int t = 0; t < 360; t += 1){
            float radianT = t;
            float x = radius * cos(radianT);
            float z = radius * sin(radianT);
            positons[s] = ofVec3f(x, y, z);
        }
    }
     */
    for (int i = 0; i < HEIGHT_NUM; i++) {
        y += 10;
        float r = ofSignedNoise(ofRandom(0.0, 1.0))*10;
        radius.push_back(r);
        t_x.push_back(ofSignedNoise(ofRandom(0.0,1.0)) * 20);
        t_z.push_back(ofSignedNoise(ofRandom(0.0,1.0)) * 20);
        
        for (int j = 0; j < 360; j+=10) {
            float _t_y = ofSignedNoise(ofRandom(0.0,1.0)) * 10;
            radianT.push_back(j);
            float x = radius[i] * sin(j) + t_x[i];
            float z = radius[i] * cos(j) + t_z[i];
            positons_vector.push_back(ofVec3f(x, y+_t_y, z));
            t_y.push_back(_t_y);
            //mesh.addVertex(positons_vector[i*j + j]);
            //mesh.addIndex(i*j + j);
        }
    }
    
    for (int i = 0; i < positons_vector.size(); i++) {
        mesh.addVertex(positons_vector[i]);
        colors[i] = ofFloatColor(ofRandom(0.3,0.8), ofRandom(0.6, 0.8), ofRandom(0.3,0.5), ofRandom(0.3, 0.5));
        mesh.addColor(colors[i]);
    }
    
    
    float connectionDistance = 2;
    int numVerts = mesh.getNumVertices();
    
    for (int a=0; a<numVerts; ++a) {
        ofVec3f verta = mesh.getVertex(a);
        for (int b=a+1; b<numVerts; ++b) {
            ofVec3f vertb = mesh.getVertex(b);
            float distance = verta.distance(vertb);
            if (distance <= connectionDistance) {
                mesh.addIndex(a);
                mesh.addIndex(b);
            }
        }
    }
    
    
    
    
    /*
    for (int i = 0; i < NUM; i++) {
        radianS ++;
        radianT += 0.5;
        y +=0.5;
        float r = radius * sin(radianT);
        positons[i] = ofVec3f(r * cos(radianS), y, 0);
        colors[i] = ofFloatColor(ofRandom(0.1, 0.8), ofRandom(0.1, 0.5), ofRandom(0.1, 0.8), ofRandom(0, 1.0));
        //mesh.addVertex(positons[i]);
        //mesh.addIndex(i);
    }
     */
    //vbo.setVertexData(positons, NUM, GL_DYNAMIC_DRAW);
    //vbo.setColorData(colors, NUM, GL_DYNAMIC_DRAW);
    //vbo.setColorData(colors, NUM, GL_DYNAMIC_DRAW);
}

void Totems::update(ofFloatColor _color, float volume, float _multi_r) {
    color = _color;
    int count = 0;
    int numVerts = mesh.getNumVertices();
    y = 0;
    /*
    for (int i = 0; i < 50; i++) {
        
        for (int j = 0; j < 360; j+=10) {
            ofVec3f vert = mesh.getVertex(count);
            
            vert.x += radius[count] * cos(radianT[count]);
            vert.z += radius[count] * sin(radianT[count]);
            
            mesh.setVertex(count, vert);
            count++;
            if(count > numVerts) {
                count = 0;
            }
        }
    }
     */
    for (int i = 0; i < HEIGHT_NUM; i++) {
        y += 10;
        //float r = ofSignedNoise(ofRandom(0.0, 1.0))*10;
        //radius.push_back(r);
        
        float r =  radius[i] + 10 * sin(radianT[count]);
        r *= _multi_r;
        for (int j = 0; j < 360; j+=10) {
            radianT[count]+= ofRandom(-0.1, 0.1);
            //cout << "count :" << ofToString(radianT[count]) << endl;
            float x = r * sin(radianT[count]) + t_x[i];
            float z = r * cos(radianT[count]) + t_z[i];
            positons_vector[count] = ofVec3f(x, y+t_y[count], z);
            //mesh.addVertex(positons_vector[i*j + j]);
            //mesh.addIndex(i*j + j);
            mesh.setVertex(count, positons_vector[count]);
            count++;
        }
    }
    
    for (int i = 0; i < positons_vector.size(); i++) {
        colors[i] = color;
        colors[i].r = colors[i].r + volume;
        colors[i].b = colors[i].r + volume;
        colors[i].a = colors[i].r + volume;
        mesh.setColor(i,colors[i]);
    }
    
  
    
    
}

void Totems::draw() {
    mesh.setMode(OF_PRIMITIVE_LINES);

    ofPushMatrix();
    ofRotateX(randomX);
    ofRotateY(randomY);
    ofRotateY(randomZ);
    ofTranslate(translatePos);
    
    mesh.draw();
    //vbo.draw(GL_LINES, 0, NUM);
    ofPopMatrix();
}