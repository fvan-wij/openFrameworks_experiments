#pragma once
#include "ofMain.h"

class Square {
    private:
        ofVec3f _p;
        ofVec3f _d;
        ofColor _col;
        ofColor _stdCol;
        bool    _bOn;

    public:
        Square(ofVec3f p, float width, float height, float depth);
        ~Square();
        void draw();
        void grow(int dist);
        void shrink();
        int  isOn();
        void setColor(ofColor col);
        void resetColor();
        ofVec2f getPosition();
};