#include "Square.h"
// #include "ofApp.h"

Square::Square(ofPoint p, ofVec3f d)
{
    _position           = p;
    _dimensions         = d;
    _initialDimensions  = d;
    _col                = ofColor(250);
    _bGrow              = false;
    _speed              = 1.0;
};

Square::~Square()
{

};

void Square::draw(){
    ofDrawBox(_position.x, _position.y , _position.z, _dimensions.x, _dimensions.y, _dimensions.z);
};

void Square::update(ofPoint p)
{
    _position.x = p.x;
    _position.y = p.y;
    _position.z = p.z;
};

void Square::grow()
{
    _dimensions.z += _speed;
};

void Square::shrink()
{
    _dimensions.z -= _speed;
    if (_dimensions.z <= _initialDimensions.z)
        _dimensions.z = _initialDimensions.z;
};

void Square::setSpeed(int speed)
{
    _speed = speed;
}

void Square::setDimensions(ofVec3f d)
{
    _dimensions.x = d.x;
    _dimensions.y = d.y;
    _dimensions.z = d.z;
}