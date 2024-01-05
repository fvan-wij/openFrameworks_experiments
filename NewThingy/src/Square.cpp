#include "Square.h"

Square::Square(ofVec3f p, float width, float height, float depth)
{
    _p      = p;
    _d.x    = width;
    _d.y    = height;
    _d.z    = depth;
    _col    = ofColor(40);
    _stdCol = ofColor(40);
    _bOn    = false;
};

Square::~Square()
{

};

void Square::draw(){
    ofSetColor(_col);
    ofDrawBox(_p, _d.x, _d.y, _d.z);
};

void Square::grow(int dist){
    int threshold = pow(dist, 1.35);
    _col = ofColor(255, 161, 38);
    _d.z += 10;
    _bOn = true;
    if (_d.z > threshold)
        _d.z = threshold;
};

void Square::shrink(){
    _col = _stdCol;
    _d.z -= 10;
    _bOn = false;
    if (_d.z < _d.x)
        _d.z = _d.x;
};

int Square::isOn()
{
    return _bOn;
}

void Square::setColor(ofColor col)
{
    _col = col;
}

void Square::resetColor()
{
    _col = ofColor(255, 161, 38);
}

ofVec2f Square::getPosition()
{
    return ofVec2f(_p.x, _p.y);
}