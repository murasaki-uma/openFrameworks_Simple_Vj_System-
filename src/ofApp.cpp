#include "ofApp.h"

int isAttack;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetCircleResolution(80);
    
    width = ofGetWidth();
    height = ofGetHeight();
    
    bool succ = true;
    image.loadImage("stars.jpg");
    image.resize(400, 200);
    if (!succ) {
        cerr << "loading image failed ...\n";
    }
    
    ofEnableAlphaBlending();
    
    mesh.setMode(OF_PRIMITIVE_LINES);
    nearPosMesh.setMode(OF_PRIMITIVE_LINES);
    nearPosMesh.enableColors();
    ofSetFrameRate(60);
    
    float intensityThreshold = 150.0;
    int w = image.getWidth();
    int h = image.getHeight();
    for (int x=0; x<w; ++x) {
        for (int y=0; y<h; ++y) {
            ofColor c = image.getColor(x, y);
            float intensity = c.getLightness();
            if (intensity >= intensityThreshold) {
                float saturation = c.getSaturation();
                float z = ofMap(saturation, 0, 255, -100, 100);
                ofVec3f pos(4*x, 4*y, z);
                mesh.addVertex(pos);
                mesh.addColor(c);
                
                offsets.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));
            }
        }
    }
    
    float connectionDistance = 30;
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
    
    meshCentroid = mesh.getCentroid();
    
    for (int i=0; i<numVerts; ++i) {
        ofVec3f vert = mesh.getVertex(i);
        float distance = vert.distance(meshCentroid);
        float angle = atan2(vert.y-meshCentroid.y, vert.x-meshCentroid.x);
        distances.push_back(distance);
        angles.push_back(angle);
    }
    
    orbiting = false;
    startOrbitTime = 0.0;
    meshCopy = mesh;
    
    /***************** AttackVisualizeSphere *****************/
    for (int i = 0; i < PARTICLE_NUM; i++) {
        nearPosRadianS[i] = ofRandom(180);
        nearPosRadianT[i] = ofRandom(360);;
        nearPosRadius = 200;
        nearPos_pos[i] = ofVec3f(nearPosRadius * sin(nearPosRadianS[i]) * cos(nearPosRadianT[i]), nearPosRadius * sin(nearPosRadianS[i]) * sin(nearPosRadianT[i]), nearPosRadius * cos(nearPosRadianS[i]));
        /*
        particle_theta[i] = ofVec3f(ofRandom(TWO_PI), ofRandom(TWO_PI), ofRandom(PI));
        particle_inc[i] = ofVec3f(ofRandom(0.01, 0.05), ofRandom(0.01, 0.05), ofRandom(0.01, 0.05));
        */
        nearPosMesh.addVertex(nearPos_pos[i]);
        meshColors.push_back( ofColor(ofRandom(170,235), ofRandom(80, 160), ofRandom(170,255), ofRandom(20, 60)));
        
        nearPosMesh.addColor(meshColors[i]);
    }
    
    float connectionDistance_sphere = 30;
    int numVerts_sphere = nearPosMesh.getNumVertices();
    for (int a=0; a<numVerts_sphere; a++) {
        ofVec3f verta = nearPosMesh.getVertex(a);
        for (int b=a+1; b<numVerts_sphere; ++b) {
            ofVec3f vertb = nearPosMesh.getVertex(b);
            float distance = verta.distance(vertb);
            if (distance <= connectionDistance_sphere) {
                nearPosMesh.addIndex(a);
                nearPosMesh.addIndex(b);
            }
        }
    }
    
    
    /***************** ofxOsc *****************/
    // listen on the given port
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup(PORT);
    
    current_msg_string = 0;
    mouseX = 0;
    mouseY = 0;
    mouseButtonState = "";
    
    ofBackground(30, 30, 130);
    
    
    
    /***************** ofSoundStream *****************/
    soundStream.listDevices();
    int bufferSize = 255;
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    //left2.assign(bufferSize, 0.0);
    //right2.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter   = 0;
    drawCounter     = 0;
    smoothedVol     = 0.0;
    scaledVol       = 0.0;
    
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    left2.assign(bufferSize, 0.0);
    right2.assign(bufferSize, 0.0);
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    
    
    /***************** vboSphere *****************/
    for (int i = 0; i < PARTICLE_NUM; i++) {
        radianS[i] = ofRandom(180);
        radianT[i] = ofRandom(360);
        nearPosRadius = 200;
        particle_pos[i] = ofVec3f(nearPosRadius * sin(radianS[i]) * cos(radianT[i]), nearPosRadius * sin(radianS[i]) * sin(radianT[i]), nearPosRadius * cos(radianS[i]));
        particle_theta[i] = ofVec3f(ofRandom(TWO_PI), ofRandom(TWO_PI), ofRandom(PI));
        particle_inc[i] = ofVec3f(ofRandom(0.01, 0.05), ofRandom(0.01, 0.05), ofRandom(0.01, 0.05));
    }
    
    positonVbo.setVertexData(particle_pos, PARTICLE_NUM, GL_DYNAMIC_DRAW);
    
    
    
    /***************** ofxGui *****************/
    
    gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
    gui.add(filled.set("bFill", true));
    gui.add(movingArea.setup("movingArea", 200, nearPosRadius, 400));
    gui.add(drawMesh.set("drawMesh", false));
    gui.add(meshSpeed.setup("meshSpeed", 1.0, 0.0, 10.0));
    gui.add(drawSphere.set("drawSphere", false));
    gui.add(drawParticle.set("drawParticle", false));
    gui.add(drawAttackInteraction.set("drawAttackInteraction", false));
    //gui.add(Attack_r.setup("Attack_r", 0, 0.0, 0.1));
    //gui.add(Attack_g.setup("Attack_g", 0, 0.0, 0.1));
    //gui.add(Attack_b.setup("Attack_b", 0, 0.0, 0.1));
    //gui.add(Attack_a.setup("Attack_a", 0, 0.0, 0.1));
    gui.add(AttackColor.set("AttackColor", ofFloatColor(0.0, 0.0, 0.0, 0.0),ofFloatColor(0.0,0,0.0,0.0),ofFloatColor(0.5,0.5,0.5,1.0)));
    gui.add(mluti_size.setup("mluti_size", 1, 0.0, 10.0));
    gui.add(drawTotems.set("drawTotems", false));
    gui.add(totemColor.set("totemColor", ofFloatColor(0.0, 0.0, 0.0, 0.0),ofFloatColor(0.0,0,0.0,0.0),ofFloatColor(1.0,1.0,1.0,1.0)));
    //gui.add(totem_r.setup("totem_r", 0, 0.0, 1.0));
    //gui.add(totem_g.setup("totem_g", 0, 0.0, 1.0));
    //gui.add(totem_b.setup("totem_b", 0, 0.0, 1.0));
    //gui.add(totem_a.setup("totem_a", 0, 0.0, 1.0));
    gui.add(totem_vol.setup("totem_vol", 0.5, 0.0, 1.0));
    gui.add(multi_r.setup("multi_r", 0, 0.0, 10.0));
    
    gui.add(speed.setup("speed", 0.01, 0.001, 0.1));
    
    
    line_gui.setup("Line");
    line_gui.setPosition(250,0);
    line_gui.add(drawSoundLine.set("drawSoundLine", false));
    //line_gui.add(r_line_x.setup("r_line_x", 0.0, -360.0, 360.0));
    line_gui.add(r_line_y.setup("line_y_speed", 0.0, -10.0, 10.0));
    line_gui.add(r_line_z.setup("line_z_speed", 0.0, -10.0, 10.0));
    
    //line_gui.add(r_line_x2.setup("r_line_x2", 0.0, -360.0, 360.0));
    line_gui.add(r_line_y2.setup("line_y2_speed", 0.0, -10.0, 10.0));
    line_gui.add(r_line_z2.setup("line_z2_speed", 0.0, -10.0, 10.0));
    
    //line_gui.add(r_line_x3.setup("r_line_x3", 0.0, -360.0, 360.0));
    line_gui.add(r_line_y3.setup("line_y3_speed", 0.0, -10.0, 10.0));
    line_gui.add(r_line_z3.setup("line_z3_speed", 0.0, -10.0, 10.0));
    
    //line_gui.add(r_line_x4.setup("r_line_x4", 0.0, -360.0, 360.0));
    line_gui.add(r_line_y4.setup("line_y4_speed", 0.0, -10.0, 10.0));
    line_gui.add(r_line_z4.setup("line_z4_speed", 0.0, -10.0, 10.0));
    
    line_gui.add(soundLineSize.setup("soundLineSize", 0.5, 0.0, 1.0));
    
    bg_gui.setup("bgColor");
    bg_gui.setPosition(500,0);
    bg_gui.add(bgColor.set("bgColor", ofColor(85, 78, 68),ofColor(0,0),ofColor(255,255)));
    
    bHide = true;
    
    //ring.loadSound("ring.wav");
    
    /***************** ofxGui *****************/
    for (int i = 0; i < 20; i ++) {
        totems.push_back(new Totems(ofVec3f(ofRandom(-width, width), -height/2+ofRandom(-height, height), ofRandom(-600, 600))));
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    int numVerts = mesh.getNumVertices();
    for (int i=0; i<numVerts; ++i) {
        ofVec3f vert = mesh.getVertex(i);
        
        float time = ofGetElapsedTimef();
        float timeScale = 5.0;
        float displacementScale = 0.75;
        ofVec3f timeOffsets = offsets[i];
        
        vert.x += (ofSignedNoise(time*timeScale+timeOffsets.x)) * displacementScale;
        vert.y += (ofSignedNoise(time*timeScale+timeOffsets.y)) * displacementScale;
        vert.z += (ofSignedNoise(time*timeScale+timeOffsets.z)) * displacementScale;
    }
    
    if(orbiting) {
        int numVerts = mesh.getNumVertices();
        for (int i=0; i<numVerts; ++i) {
            ofVec3f vert = mesh.getVertex(i);
            float distance = distances[i];
            float angle = angles[i];
            float elapsedTime = ofGetElapsedTimef() - startOrbitTime;
            
            float speed = ofMap(distance, 0, 200, 1, 0.25, true);
            
            float rotatedAngle = elapsedTime * speed + angle + meshSpeed;
            /*
             vert.x = distance * cos(rotatedAngle) + meshCentroid.x;
             vert.y = distance * sin(rotatedAngle) + meshCentroid.y;
             */
            vert.x += 10 * cos(rotatedAngle);
            vert.y += 10 * sin(rotatedAngle);
            
            mesh.setVertex(i, vert);
        }
    }
    meshCopy = mesh;
    
    /***************** AttackVisualizeSphere *****************/
    for (int i = 0; i < cubes.size(); i++) {
        float vol = ofRandom(30.0, 50.0);
        cubes[i]->update(vol, AttackColor, mluti_size);
        if (cubes[i]->LifeLimite < 0) {
            cubes.erase(cubes.begin() + i);
        }
    }
    
    
    /***************** ofxOsc *****************/
    
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msg_strings[i] = "";
        }
    }
    
    // check for waiting messages
    isAttack = 0;
    while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        if(m.getAddress() == "/mouse/position"){
            mouseX = m.getArgAsInt32(0);
            mouseY = m.getArgAsInt32(1);
        }
        else if(m.getAddress() == "/mouse/button"){
            mouseButtonState = m.getArgAsString(0);
        }
        else if(m.getAddress() == "/image" ){
            ofBuffer buffer = m.getArgAsBlob(0);
            receivedImage.loadImage(buffer);
        }
        else{
            string msg_string;
            msg_string = m.getAddress();
            msg_string += ": ";
            isAttack = 1;
            for(int i = 0; i < m.getNumArgs(); i++){
                msg_string += m.getArgTypeName(i);
                msg_string += ":";
                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                    msg_string += ofToString(m.getArgAsInt32(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                    msg_string += ofToString(m.getArgAsFloat(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                    msg_string += m.getArgAsString(i);
                    
                }
                else{
                    msg_string += "unknown";
                }
            }
            msg_strings[current_msg_string] = msg_string;
            timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
            current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
            msg_strings[current_msg_string] = "";
        }
        
    }
    kickVolume = scaledVol*totem_vol;
    /***************** AttackTimingFunction *****************/
    if(isAttack) {
        ofVec3f mousePositon = ofVec3f(ofGetMouseX(), ofGetMouseY(), 0);
        cubes.push_back(new CubeSpreadInteraction(mousePositon));
        
    } else {
        if(kickVolume < 0.1) {
            //kickVolume *= 0.85;
        }
        
    }
    
    /***************** Totems *****************/
    for (int i = 0; i < totems.size(); i++) {
        totems[i]->update(totemColor, kickVolume, multi_r+scaledVol*0.8);
    }
    
    
    /***************** ofSoundStream *****************/
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    vol_num = ofMap(smoothedVol, 0.0, 0.1, PARTICLE_NUM, 0.0, true);
    
    volHistory.push_back(scaledVol);
    
    if ( volHistory.size() >= 400 ) {
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }
    
    /***************** AttackVisualizeSphere *****************/
    theta += 0.01;
    //theta = ofRandom(10, 20);
    
    
    float neaPosAddRadius = ofMap(smoothedVol, 0.0, 0.17, 0, 50, true);
    
    //radianT[i] +=
    for (int i = 0; i < PARTICLE_NUM; i++) {
        //radianT[i] += speed;
        //nearPosRadianS[i] += speed;
        ofVec3f vert = nearPosMesh.getVertex(i);
        vert.x = (nearPosRadius + neaPosAddRadius) * sin(nearPosRadianS[i]) * cos(nearPosRadianT[i]);
        vert.y = (nearPosRadius + neaPosAddRadius) * sin(nearPosRadianS[i]) * sin(nearPosRadianT[i]);
        vert.z = (nearPosRadius + neaPosAddRadius) * cos(nearPosRadianS[i]);
        nearPosMesh.setVertex(i, vert);
        
        float c = ofMap(smoothedVol, 0.0, 0.25, 20, 100, true);
        meshColors[i].a = c;
        nearPosMesh.setColor(i, meshColors[i]);
        
        
        
    }
    
    /***************** vboSphere *****************/
    
    
    for (int i = 0; i < PARTICLE_NUM; i++) {
        //radianT[i] += speed;
        radianS[i] += speed;
        particle_pos[i] = ofVec3f((50+movingArea+neaPosAddRadius) * sin(radianS[i]) * cos(radianT[i]),
                                  (50+movingArea+neaPosAddRadius) * sin(radianS[i]) * sin(radianT[i]),
                                  (50+movingArea+neaPosAddRadius) * cos(radianS[i]));
        
        /*
         ofVec3f fromeP = particle_pos[i];
         for(int j = 0; j < PARTICLE_NUM; j++){
         ofVec3f toP = particle_pos[j];
         float dist = (fromeP - toP).length();
         if(dist < lengthLimit){
         nearPosMesh.addVertex(toP);
         }
         }
         */
        
        
    }
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofDisableDepthTest();
    ofEnableDepthTest();
    nearPosMesh.enableColors();
    
    positonVbo.bind();
    positonVbo.updateVertexData(particle_pos, PARTICLE_NUM);
    positonVbo.unbind();
    
    
    //ofColor centerColor = ofColor(85, 78, 68);
    ofColor edgeColor(0, 0, 0);
    ofBackgroundGradient(bgColor, edgeColor, OF_GRADIENT_CIRCULAR);
    
    
    ofPushStyle();
    easyCam.begin();
    
    if(drawMesh){
        ofPushMatrix();
        ofTranslate(-ofGetWidth()/2+cameraX, -ofGetHeight()/2+cameraY,cameraZ);
        mesh.draw();
         ofPopMatrix();
    }
   
    
    ofTranslate(cameraX, cameraY,cameraZ);
    nearPosMesh.addColor(ofFloatColor(1.0, 0.0, 0.0, 0.5));
    if(drawSphere){
        nearPosMesh.draw();
        //ofSetColor(255);
        
    }
    if (drawParticle) {
        positonVbo.draw(GL_POINTS, vol_num, PARTICLE_NUM);
    }
    ofPushMatrix();
    //ofTranslate(-ofGetWidth()/2+cameraX, -ofGetHeight()/2+cameraY,cameraZ);
    if(drawTotems){
        for (int i = 0; i < totems.size(); i++) {
            totems[i]->draw();
        }
    }
    
    ofPopMatrix();
    
    ofPushMatrix();
    //ofTranslate(-ofGetWidth()/2+cameraX, -ofGetHeight()/2+cameraY,cameraZ);
    if(drawAttackInteraction) {
        for (int i = 0; i < cubes.size(); i++) {
            
            cubes[i]->draw();
        }
    }
    ofPopMatrix();
    
    
    
/***************** SoundLine *****************/
    if(drawSoundLine) {
    ofPushStyle();
    ofSetLineWidth(1);
    ofBeginShape();
    ofPushMatrix();
    //ofRotateX(rotate_time_x++);
    ofRotateY(rotate_time_y+=r_line_y);
    ofRotateZ(rotate_time_z+=r_line_z);
    for (unsigned int i = 0; i < left.size(); i++) {
        ofSetColor(167,107,223);
        ofNoFill();
        ofVertex(-400+i*7, left[i]*180.0f);
    }
    ofPopStyle();
    ofEndShape(false);
    ofPopMatrix();
    
    ofSetLineWidth(1);
    ofBeginShape();
    ofPushMatrix();
    //ofRotateX(rotate_time_x2++);
    ofRotateY(rotate_time_y2+=r_line_y2);
    ofRotateZ(rotate_time_z2+=r_line_z2);
    for (unsigned int i = 0; i < left2.size(); i++) {
        ofSetColor(167,107,223);
        ofVertex(-200+i*7, left2[i]*180.0f);
    }
    ofPopStyle();
    ofEndShape(false);
    ofPopMatrix();
    
    
    
    ofPushStyle();
    ofPushMatrix();
    ofSetColor(245, 58, 135);
    ofSetLineWidth(1);
    
    //ofRotateX(rotate_time_x3++);
    ofRotateY(rotate_time_y3+=r_line_y3);
    ofRotateZ(rotate_time_z3+=r_line_z3);
     
    ofBeginShape();
    for (unsigned int i = 0; i < right.size(); i++){
        ofVertex(-200+i*7, -right[i]*180.0f);
    }
    ofEndShape(false);
    ofPopMatrix();
    ofPopStyle();
    
    ofPushStyle();
    ofPushMatrix();
    ofSetColor(245, 58, 135);
    ofSetLineWidth(1);
    
    //ofRotateX(rotate_time_x4++);
    ofRotateY(rotate_time_y4+=r_line_y4);
    ofRotateZ(rotate_time_z4+=r_line_z4);
    
    ofBeginShape();
    for (unsigned int i = 0; i < right2.size(); i++){
        ofVertex(-400+i*7, -right2[i]*180.0f);
    }
    ofEndShape(false);
    ofPopMatrix();
    ofPopStyle();
    }
    

    
    easyCam.end();
    ofPopStyle();
    ofDisableDepthTest();
    
    if( bHide ){
        gui.draw();
        line_gui.draw();
        bg_gui.draw();
        
    }
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'o') {
        orbiting = !orbiting;
        startOrbitTime = ofGetElapsedTimef();
        mesh = meshCopy;
    }
    
    if (key == 'b') {
        if(bHide){
            bHide = false;
        } else {
            bHide = true;
        }
    }
    
    if(key == 'f') {
        ofToggleFullscreen();
    }
    
    switch (key) {
            
        case 'w':
            cameraZ += 100;
            break;
            
        case 'a':
            cameraX -= 100;
            break;
            
        case 'd':
            cameraX += 100;
            break;
            
        case 'x':
            cameraZ -= 100;
            break;
            
        case 'r':
            cameraY += 100;
            break;
            
        case 'f':
            cameraY -= 100;
            break;
            
        default:
            break;
    }
    
}


//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels) {
    
    float curVol = 0.0;
    
    int numCounted = 0;
    
    for (int i = 0; i < bufferSize; i++) {
        left[i]     = input[i*2]*soundLineSize;
        right[i]    = input[i*2+1]*soundLineSize;
        
        left2[i]     = input[i*2]*soundLineSize;
        right2[i]    = input[i*2+1]*soundLineSize;
        
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted += 2;
    }
    
    curVol /= (float)numCounted;
    
    curVol = sqrt(curVol);
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    /*
    ofVec3f mousePositon = ofVec3f(ofGetMouseX(), ofGetMouseY(), 0);
    cubes.push_back(new CubeSpreadInteraction(mousePositon));
    */
    
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
