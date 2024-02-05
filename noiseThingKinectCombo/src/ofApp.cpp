#include "ofApp.hpp"
#include <cstddef>

//--------------------------------------------------------------
void ofApp::setup(){
	int tiles;

	tiles = 10;
	width = 640;
	height = 480;
	tileSize = ofVec2f(width / (float) tiles, height / (float) tiles);
	currentPos = ofVec2f(100, 100);
	newPos = ofVec2f(100, 100);

	kinect.init(false, false);
	kinect.open();

	angle = 0;
	nearTreshold = 100;
	farTreshold = 700;
	stride = 2;
	kinect.setCameraTiltAngle(angle);

	lineSegment segment(currentPos, newPos);
	lineSegments.push_back(segment);


	for (size_t x = 0; x < width; x+=stride) {
		for (size_t y = 0; y < height; y+=stride) {
			contourData contour(x, y, tileSize);
			contourVector.push_back(contour);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();

	coinFlip();
	lineSegment segment(currentPos, newPos);
	lineSegments.push_back(segment);
	currentPos = newPos;
	for (size_t i = 0; i < lineSegments.size(); i++) {
		lineSegments[i].update(contourVector);
		lineSegments[i].setTileSize(tileSize);
		if (lineSegments[i].currentPos.y > ofGetHeight() || lineSegments[i].timer < 0)
			lineSegments.erase(lineSegments.begin() + i);
	}

	// for (size_t i = 0; i < contourVector.size(); i++) {
	// 	contourVector[i].update();
	// }
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor(255), ofColor(0), OF_GRADIENT_CIRCULAR);
	drawContour();
	for (size_t i = 0; i < lineSegments.size(); i++) {
			lineSegments[i].draw();
	}
}

void ofApp::exit()
{
	kinect.setCameraTiltAngle(0);
	kinect.close();
}


void ofApp::coinFlip()
{
	float coinFlip;

	for (size_t i = 0; i < 2; i++) 
	{
		coinFlip = ofRandom(1);
		if (coinFlip > 0.5)
			result[i] = 'H'; //Heads
		else
			result[i] = 'T'; //Tails
	}
	if (result[0] == 'H' && result[1] == 'T')
		direction.set(-tileSize.x, 0);
	else if (result[0] == 'H' && result[1] == 'H')
		direction.set(0, -tileSize.y);
	else if (result[0] == 'T' && result[1] == 'H')
		direction.set(tileSize.x, 0);
	else if (result[0] == 'T' && result[1] == 'T')
		direction.set(0, tileSize.y);

	ofVec2f posCheck = newPos + direction;
	if (posCheck.x >= 0 && posCheck.x <= ofGetWidth() && posCheck.y >= 0 && posCheck.y <= ofGetHeight())
		newPos += direction;
}

void ofApp::drawContour()
{
	int i;

	i = 0;
	for (size_t x = 0; x < width; x+=stride) {
		for (size_t y = 0; y < height; y+=stride) {
			float distance = kinect.getDistanceAt(x, y);
			if (distance > nearTreshold && distance < farTreshold)
			{
				contourVector[i].draw();
				contourVector[i].visible = true;
			}
			else 
				contourVector[i].visible = false;
			i++;
		}
	}
}
