#include "contourData.hpp"
#include "ofApp.hpp"

contourData::contourData(int x, int y, ofVec2f size) {
	currentPos = ofVec2f(x, y);
	color = ofColor(255, 0, 0);
	this->size = size;
	visible = false;
	strideX = 1024 / 640;
	strideY = 768 / 480;
}

contourData::~contourData() {
}

void contourData::draw()
{
	ofSetColor(color);
	ofDrawCircle(currentPos.x * (strideX * 2), currentPos.y * (strideY * 2), 2);
}

void contourData::update()
{
	currentPos.x = currentPos.x * (strideX * 2);
	currentPos.y = currentPos.y * (strideY * 2);
}
