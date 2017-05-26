#pragma once

#include "ofCamera.h"
#include "ofEvents.h"

class modCam : public ofCamera {
public:
    modCam();
    ~modCam();
    
    virtual void begin(ofRectangle viewport = ofGetCurrentViewport());
    
    void reset();
    void setTarget(const ofVec3f& target);
    void setTarget(ofNode& target);
    ofNode& getTarget();
    
    void setDistance(float distance);
    float getDistance() const;
    
    void setDrag(float drag);
    float getDrag() const;
    
    void setTranslationKey(char key);
    char getTranslationKey();
    
    void enableMouseInput();
    void disableMouseInput();
    bool getMouseInputEnabled();
    
    void enableMouseMiddleButton();
    void disableMouseMiddleButton();
    bool getMouseMiddleButtonEnabled();
    
    void setAutoDistance(bool bAutoDistance);
    
private:
    ofNode target;
    ofQuaternion curRot;
    ofRectangle viewport;
    ofVec2f mouse, lastMouse, mouseVel;
    
    bool bEnableMouseMiddleButton;
    bool bApplyInertia;
    bool bDoTranslate;
    bool bDoRotate;
    bool bValidClick;
    bool bInsideArcball;
    bool bMouseInputEnabled;
    bool bDistanceSet;
    bool bAutoDistance;
    float lastDistance;
    float drag;
    float xRot, yRot, zRot;
    float moveX, moveY, moveZ;
    float sensitivityXY, sensitivityZ, sensitivityRot;
    float rotationFactor;
    char doTranslationKey;
    unsigned long lastTap;
    
    void updateRotation();
    void updateTranslation();
    void update(ofEventArgs & args);
    void updateMouse();
    void setDistance(float distance, bool save);
    float distancePointLine( ofVec3f pt, ofVec3f x1, ofVec3f x2);
    
};
