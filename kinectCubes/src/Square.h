#pragma once
// #include "ofApp.h"
#include "ofMain.h"

class Square {
    private:
        ofPoint _position;
        ofVec3f _dimensions;
        ofVec3f _initialDimensions;
        float   _speed;
        ofColor _col;
        bool    _bGrow;

    public:
        Square(ofPoint p, ofVec3f d);
        ~Square();
        void draw();
        void update(ofPoint p);
        void grow();
        void shrink();
        void setSpeed(int speed);
        void setDimensions(ofVec3f d);
};