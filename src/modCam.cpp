#include "modCam.h"
#include "ofMath.h"
#include "ofUtils.h"

float jminDifference = 0.1e-5;
unsigned long jdoubleclickTime = 200;

modCam::~modCam(){
    disableMouseInput();
}

modCam::modCam(){
    lastTap	= 0;
    lastDistance = 0;
    drag = 0.9f;
    sensitivityRot = 1.0f;
    sensitivityXY = .5;
    sensitivityZ= .7;
    bDistanceSet = false;
    bMouseInputEnabled = false;
    bDoRotate = false;
    bApplyInertia =false;
    bDoTranslate = false;
    bInsideArcball = true;
    bValidClick = false;
    bEnableMouseMiddleButton = true;
    bAutoDistance = true;
    doTranslationKey = 'p';
    
    reset();
    enableMouseInput();
}

void modCam::update(ofEventArgs & args){
    if(!bDistanceSet && bAutoDistance){
        setDistance(getImagePlaneDistance(viewport), true);
    }
    if(bMouseInputEnabled){
        rotationFactor = sensitivityRot * 180 / min(viewport.width, viewport.height);
        if (bMouseInputEnabled) {
            updateMouse();
        }
        if (bDoRotate) {
            updateRotation();
        }else if (bDoTranslate) {
            updateTranslation();
        }
    }
}

void modCam::updateTranslation(){
    if (bApplyInertia) {
        moveX *= drag;
        moveY *= drag;
        moveZ *= drag;
        if (ABS(moveX) <= jminDifference && ABS(moveY) <= jminDifference && ABS(moveZ) <= jminDifference) {
            bApplyInertia = false;
            bDoTranslate = false;
        }
    }
    
    // set pan direction for mouse Y
    ofVec3f zTrns = getZAxis();
    zTrns.set(zTrns.x, 0, zTrns.z);
    zTrns.normalize();
    
    // set parameters for translations limitation
    ofVec3f movement =  (getXAxis() * moveX) + (getZAxis() * moveY) + (zTrns* moveZ);
    ofVec3f zero = ofVec3f(0, 0, 0);
    ofVec3f spotA = ofVec3f(0, 0, -30);
    ofVec3f spotB = ofVec3f(0, 0, 30);
    ofVec3f cPt = getGlobalPosition();
    ofVec3f nPt = cPt + movement;
    float cDist = distancePointLine(cPt, spotB, spotA);
    float nDist = distancePointLine(nPt, spotB, spotA);
    if( cDist > 100 && nDist > cDist){
        moveX = 0;
        moveZ = 0;
    }
    // set paramaters for zoom limitation
    float distLimit = getDistance();
    if(distLimit > 200 && moveY > 0){
        moveY = 0;
    } else if (distLimit < 40 && moveY < 0){
        moveY = 0;
    }
    
    move((getXAxis() * moveX) + (getZAxis() * moveY) + (zTrns* moveZ));
    target.move((getXAxis() * moveX) + 0 * (getYAxis() * moveY) + (zTrns * moveZ));
}

void modCam::updateRotation(){
    if (bApplyInertia) {
        xRot *=drag;
        yRot *=drag;
        zRot *=drag;
        if (ABS(xRot) <= jminDifference && ABS(yRot) <= jminDifference && ABS(zRot) <= jminDifference) {
            bApplyInertia = false;
            bDoRotate = false;
        }
    }
    ofVec3f rAxis = ofCamera::getZAxis();
    if(rAxis.dot(ofVec3f(0,1,0)) > 0.85 && xRot < 0){
        xRot = 0;
    } else if (rAxis.dot(ofVec3f(0,1,0)) <  0.35 && xRot > 0){
        xRot = 0;
    }
    curRot = ofQuaternion(xRot, ofCamera::getXAxis(), yRot, ofVec3f(0,1,0), 0, ofCamera::getZAxis());
    setPosition((ofCamera::getGlobalPosition()-target.getGlobalPosition())*curRot +target.getGlobalPosition());
    rotate(curRot);
    
}

void modCam::updateMouse(){
    mouse = ofVec2f(ofGetMouseX(), ofGetMouseY());
    if(viewport.inside(mouse.x, mouse.y) && !bValidClick && ofGetMousePressed()){
        unsigned long curTap = ofGetElapsedTimeMillis();
        if(lastTap != 0 && curTap - lastTap < jdoubleclickTime){
            reset();
        }
        if ((bEnableMouseMiddleButton && ofGetMousePressed(OF_MOUSE_BUTTON_MIDDLE)) || ofGetKeyPressed(doTranslationKey)  || ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)){
            bDoTranslate = true;
            bDoRotate = false;
            bApplyInertia = false;
        }else if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
            bDoTranslate = false;
            bDoRotate = true;
            bApplyInertia = false;
        }
        lastTap = curTap;
        lastMouse = mouse;
        bValidClick = true;
        bApplyInertia = false;
    }
    if (bValidClick) {
        if (!ofGetMousePressed()) {
            bApplyInertia = true;
            bValidClick = false;
        }else {
            int vFlip;
            if(isVFlipped()){
                vFlip = -1;
            }else{
                vFlip =  1;
            }
            mouseVel = mouse  - lastMouse;
            if (bDoTranslate) {
                moveX = 0;
                moveY = 0;
                moveZ = 0;
                if (ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)) {
                    moveY = mouseVel.y * sensitivityZ * (getDistance() + FLT_EPSILON)/ viewport.height;
                }else {
                    moveX = -mouseVel.x * sensitivityXY * (getDistance() + FLT_EPSILON)/viewport.width;
                    moveZ = -mouseVel.y * sensitivityZ * (getDistance() + FLT_EPSILON)/ viewport.height;
                }
            }else {
                xRot = 0;
                yRot = 0;
                zRot = 0;
                
                xRot = vFlip * -mouseVel.y * rotationFactor;
                yRot = -mouseVel.x * rotationFactor;
                
            }
            lastMouse = mouse;
        }
    }
}

void modCam::begin(ofRectangle viewport){
    this->viewport = viewport;
    ofCamera::begin(viewport);
}

void modCam::reset(){
    resetTransform();
    setPosition(0, -lastDistance, lastDistance);
    target.resetTransform();
    target.setPosition(0,0, 0);
    lookAt(target);
    xRot = 0;
    yRot = 0;
    zRot = 0;
    moveX = 0;
    moveY = 0;
    moveZ = 0;
}

void modCam::setTarget(const ofVec3f& targetPoint){
    target.setPosition(targetPoint);
    lookAt(target);
}

void modCam::setTarget(ofNode& targetNode){
    target = targetNode;
    lookAt(target);
}

ofNode& modCam::getTarget(){
    return target;
}

void modCam::setDistance(float distance){
    setDistance(distance, true);
}

void modCam::setDistance(float distance, bool save){
    if (distance > 0.0f){
        if(save){
            this->lastDistance = distance;
        }
        setPosition(target.getPosition() + (distance * getZAxis()));
        bDistanceSet = true;
    }
}

float modCam::getDistance() const {
    return target.getPosition().distance(getPosition());
}

void modCam::setAutoDistance(bool bAutoDistance){
    this->bAutoDistance = bAutoDistance;
    if (bAutoDistance) {
        bDistanceSet = false;
    }
}

void modCam::setDrag(float drag){
    this->drag = drag;
}

float modCam::getDrag() const {
    return drag;
}

void modCam::setTranslationKey(char key){
    doTranslationKey = key;
}

char modCam::getTranslationKey(){
    return doTranslationKey;
}

void modCam::enableMouseInput(){
    if(!bMouseInputEnabled){
        bMouseInputEnabled = true;
        ofAddListener(ofEvents().update , this, &modCam::update);
    }
}

void modCam::disableMouseInput(){
    if(bMouseInputEnabled){
        bMouseInputEnabled = false;
        ofRemoveListener(ofEvents().update, this, &modCam::update);
    }
}

bool modCam::getMouseInputEnabled(){
    return bMouseInputEnabled;
}

void modCam::enableMouseMiddleButton(){
    bEnableMouseMiddleButton = true;
}

void modCam::disableMouseMiddleButton(){
    bEnableMouseMiddleButton = false;
}

bool modCam::getMouseMiddleButtonEnabled(){
    return bEnableMouseMiddleButton;
}

float modCam::distancePointLine( ofVec3f pt, ofVec3f x1, ofVec3f x2){
    float sqAB = x1.squareDistance(x2);
    float t = ( ( ( pt.x - x1.x ) * ( x2.x - x1.x ) ) + ( ( pt.y - x1.y ) * ( x2.y - x1.y ) ) + ( ( pt.z - x1.y ) * ( x2.z - x1.z ) ) ) / sqAB ;
    if( t < 0.0 ) return x1.distance(pt);
    if( t > 1.0 ) return x2.distance(pt);
    ofVec3f orthoPt = x1 + t * (x2-x1);
    return orthoPt.distance(pt);
}
