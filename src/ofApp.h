#pragma once

#include "ofMain.h"
#include "CubeSpreadInteraction.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "Totems.h"

// listen on port
#define PORT 7400
#define NUM_MSG_STRINGS 20

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    float width, height;
    
    ofMesh mesh;
    ofImage image;
    ofEasyCam easyCam;
    vector<ofVec3f> offsets;
    
    ofMesh meshCopy;
    bool orbiting;
    float startOrbitTime;
    
    vector<float> distances;
    vector<float> angles;
    ofVec3f meshCentroid;
    
    float cameraX, cameraY, cameraZ;
    
    
    /***************** ofxOsc *****************/
    
    ofTrueTypeFont font;
    ofxOscReceiver receiver;
    
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    
    int mouseX, mouseY;
    string mouseButtonState;
    
    ofImage receivedImage;
    
    
    
    /***************** AttackVisualizeSphere *****************/
    static const int PARTICLE_NUM = 5000;
    ofVec3f nearPos_pos[PARTICLE_NUM];
    float nearPosRadius;
    ofMesh nearPosMesh;
    int lengthLimit = 10;
    vector<ofColor> meshColors;
    float nearPosRadianS[PARTICLE_NUM];
    float nearPosRadianT[PARTICLE_NUM];
    
    /***************** CubeSpreadInteraction *****************/
    vector<CubeSpreadInteraction *> cubes;
    
    
    /***************** ofxGui *****************/
    ofxIntSlider movingArea;
    ofParameter<ofColor> bgColor;
    ofParameter<ofVec2f> center;
    ofParameter<int> circleResolution;
    ofParameter<bool> filled;
    ofParameter<bool> drawMesh;
    ofParameter<bool> drawSphere;
    ofParameter<bool> drawParticle;
    ofParameter<bool> drawAttackInteraction;
    ofParameter<bool> drawSoundLine;
    ofxFloatSlider Attack_r;
    ofxFloatSlider Attack_g;
    ofxFloatSlider Attack_b;
    ofxFloatSlider Attack_a;
    ofParameter<ofFloatColor> AttackColor;
    ofxFloatSlider mluti_size;
    ofParameter<bool> drawTotems;
    ofxFloatSlider totem_r;
    ofxFloatSlider totem_g;
    ofxFloatSlider totem_b;
    ofxFloatSlider totem_a;
    ofxFloatSlider totem_vol;
    ofParameter<ofFloatColor> totemColor;
    ofxFloatSlider multi_r;
    ofxFloatSlider speed;
    ofxFloatSlider meshSpeed;
    
    ofxFloatSlider r_line_x, r_line_x2, r_line_x3, r_line_x4;
    ofxFloatSlider r_line_y, r_line_y2, r_line_y3, r_line_y4;
    ofxFloatSlider r_line_z, r_line_z2, r_line_z3, r_line_z4;
    float rotate_time_x, rotate_time_y, rotate_time_z;
    float rotate_time_x2, rotate_time_y2, rotate_time_z2;
    float rotate_time_x3, rotate_time_y3, rotate_time_z3;
    float rotate_time_x4, rotate_time_y4, rotate_time_z4;
    ofxFloatSlider soundLineSize;
    ofParameter<string> screenSize;
    
    ofxPanel gui;
    ofxPanel line_gui;
    ofxPanel bg_gui;
    
    bool bHide;
    
    
    /***************** totems *****************/
    vector<Totems *> totems;
    
    
    /***************** ofSoundStream *****************/
    float vol_num;
    void audioIn(float * input, int bufferSize, int nChannels);
    
    vector<float> left;
    vector<float> left2;
    vector<ofVec3f> sound_line_left_pos;
    vector<float> right;
    vector<float> right2;
    vector<ofVec3f> sound_line_right_pos;
    vector<float> volHistory;
    
    int bufferCounter;
    int drawCounter;
    
    float smoothedVol;
    float scaledVol;
    
    ofSoundStream soundStream;
    
    float theta;
    
    float kickVolume;
    
    
     /***************** vboSphere *****************/
    ofVbo positonVbo;
    ofVbo thetaVbo;
    ofVbo incVbo;
    //vector<ofVec3f> particle_vec;
    ofVec3f particle_pos[PARTICLE_NUM];
    //vector <ofVec3f *> particle_pos;
    ofVec3f particle_theta[PARTICLE_NUM];
    //vector<ofVec3f *> particle_theta;
    
    ofVec3f particle_inc[PARTICLE_NUM];
    //vector<ofVec3f *> particle_inc;
    
    float rotate[PARTICLE_NUM];
    //vector<float *> rotate;
    float radianS[PARTICLE_NUM];
    //vector<float *> radianS;
    float radianT[PARTICLE_NUM];
    //vector<float *> radianT;
    //float Radius[PARTICLE_NUM];
};
