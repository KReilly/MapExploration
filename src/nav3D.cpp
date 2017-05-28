#include "nav3D.h"

void nav3D::setup(){
    ofEnableDepthTest();
    
    cam.setDistance(60);
    cam.setNearClip(0.5);
    
    ofSetCircleResolution(64);
    cam.move(0, cam.getDistance(), 0);
    cam.setTarget(ofVec3f(0,0,0));
    
    glColorMaterial (GL_FRONT, GL_DIFFUSE);
    glEnable (GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    stylizedModel1.loadModel("setD.obj");
    stylizedModel2.loadModel("setC.obj");
    stylizedModel3.loadModel("setB.obj");
    stylizedModel4.loadModel("setA.obj");
    
    stylizedModel1.setScale(-1, -1, 1);
    stylizedModel2.setScale(-1, -1, 1);
    stylizedModel3.setScale(-1, -1, 1);
    stylizedModel4.setScale(-1, -1, 1);
    
    pin.addVertex(ofPoint( -1.125, 0, 0));
    pin.addVertex(ofPoint( -1.125, 4, 0));
    pin.addVertex(ofPoint( 1.125, 4, 0));
    pin.addVertex(ofPoint( 1.125, 0, 0));
    
    pin.addTriangle(2, 1, 0);
    pin.addTriangle(3, 2, 0);
    
    int U=1;
    int V=1;
    
    pin.addTexCoord(ofPoint(0 * U, 0 * V));
    pin.addTexCoord(ofPoint(0 * U, 1 * V));
    pin.addTexCoord(ofPoint(1 * U, 1 * V));
    pin.addTexCoord(ofPoint(1 * U, 0 * V));

    
    ground.set(500, 500);
    ground.setPosition(0, 0, 0);
    ground.setResolution(10, 10);
}

void nav3D::update(){
}

void nav3D::draw(){
    cam.begin();
    
    ofBackground(222, 224, 217);
    
    stylizedModel1.enableColors();
    stylizedModel2.enableColors();
    stylizedModel3.enableColors();
    stylizedModel4.enableColors();
    
    ofPushMatrix();
        ofRotateY(90);
        ofSetColor(250, 250, 250, 255);
        ofScale(0.75, 0.75, 0.75);
        
        stylizedModel1.drawFaces();
        stylizedModel2.drawFaces();
        stylizedModel3.drawFaces();
        stylizedModel4.drawFaces();
    ofPopMatrix();
    
    
    ofVec3f rotAxis = cam.getXAxis();
    
    ofPushMatrix();
        ofSetColor(0,255, 0);
        ofTranslate(0, 3, 0);
        ofRotate(cam.getRoll(), rotAxis.x, rotAxis.y, rotAxis.z );
        ofRotateY(cam.getHeading());
        pin.draw();
    ofPopMatrix();
    
    ofPushMatrix();
        ofSetColor(255,255, 0);
        ofTranslate(6, 3, -12);
        ofRotate(cam.getRoll(), rotAxis.x, rotAxis.y, rotAxis.z );
        ofRotateY(cam.getHeading());
        pin.draw();
    ofPopMatrix();
    
    ofPushMatrix();
        ofSetColor(255,0, 0);
        ofTranslate(12, 12, 24);
        ofRotate(cam.getRoll(), rotAxis.x, rotAxis.y, rotAxis.z );
        ofRotateY(cam.getHeading());
        pin.draw();
    ofPopMatrix();
    
    ofPushMatrix();
        ofSetColor(222, 224, 217);
        ofRotateX(90);
        ofTranslate(0, 0, 5);
        ground.draw();
    ofPopMatrix();

    
    cam.end();
}

void nav3D::keyPressed(int key){
    switch(key) {
        case 'C':
        case 'c':
            if(cam.getMouseInputEnabled())
                    cam.disableMouseInput();
            else
                    cam.enableMouseInput();
            break;
    }
}

void nav3D::keyReleased(int key){ }
void nav3D::mouseMoved(int x, int y ){ }
void nav3D::mouseDragged(int x, int y, int button){ }
void nav3D::mousePressed(int x, int y, int button){ }
void nav3D::mouseReleased(int x, int y, int button){ }
void nav3D::mouseEntered(int x, int y){ }
void nav3D::mouseExited(int x, int y){ }
void nav3D::windowResized(int w, int h){ }
void nav3D::gotMessage(ofMessage msg){ }
void nav3D::dragEvent(ofDragInfo dragInfo){ }
