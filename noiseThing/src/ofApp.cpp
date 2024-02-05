#include "ofApp.h"
#include "ofGraphics.h"
#include "ofGraphicsConstants.h"

//Create a coin-flipping number generator;
//Spin twice, store output in array;
//HT = LEFT, TT = UP, TH = RIGHT, HH = DOWN
//Create variable for current position
//Increment direction and store position
//Repeat

//--------------------------------------------------------------
void ofApp::setup(){
	int tiles;

	tiles = 25;
	tileSize.x = ofGetWidth() / (float) tiles;
	tileSize.y = ofGetHeight() / (float) tiles;
	currentPos = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
	newPos = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
	lineSegment segment(currentPos.x, currentPos.y, newPos.x, newPos.y);
	lineSegments.push_back(segment);
	// ofSetFrameRate(10);
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);
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

//--------------------------------------------------------------
void ofApp::diceRoll()
{
	int diceRoll;

	diceRoll = ofRandom(8) + 1;
	if (diceRoll == 1)
		direction.set(-tileSize.x, 0);
	else if (diceRoll == 2)
		direction.set(-tileSize.x, -tileSize.y);
	else if (diceRoll == 3)
		direction.set(0, -tileSize.y);
	else if (diceRoll == 4)
		direction.set(tileSize.x, -tileSize.y);
	else if (diceRoll == 5)
		direction.set(tileSize.x, 0);
	else if (diceRoll == 6)
		direction.set(tileSize.x, tileSize.y);
	else if (diceRoll == 7)
		direction.set(0, tileSize.y);
	else if (diceRoll == 8)
		direction.set(-tileSize.x, tileSize.y);

	// std::cout << diceRoll << endl;

	ofVec2f posCheck = newPos + direction;
	if (posCheck.x >= 0 && posCheck.x <= ofGetWidth() && posCheck.y >= 0 && posCheck.y <= ofGetHeight())
		newPos += direction;
		
}

//--------------------------------------------------------------
void ofApp::update(){
	coinFlip();
	// diceRoll();
	lineSegment segment(currentPos.x, currentPos.y, newPos.x, newPos.y);
	lineSegments.push_back(segment);
	currentPos = newPos;
	for (size_t i = 0; i < lineSegments.size(); i++) {
		lineSegments[i].update();
		lineSegments[i].setTileSize(tileSize.x, tileSize.y);
		if (lineSegments[i].currentPos.y > ofGetHeight())
			lineSegments.erase(lineSegments.begin() + i);
	}
	// for (size_t i = 0; i < lineSegments.size(); i++) 
	// {
	// 	for (size_t j = i + 1; j < lineSegments.size(); j++) {
	// 		if (lineSegments[i].currentPos == lineSegments[j].currentPos && lineSegments[i].newPos == lineSegments[j].newPos)
	// 			lineSegments.erase(lineSegments.begin() + i);
	// 	}
	// }
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofColor c1(255);
	ofColor c2(0);
	ofBackgroundGradient(c1, c2, OF_GRADIENT_CIRCULAR);
	for (size_t i = 0; i < lineSegments.size(); i++) {
			lineSegments[i].draw();
	}

}

lineSegment::lineSegment(int x, int y, int x2, int y2)
{
	currentPos = ofVec2f(x, y);
	newPos = ofVec2f(x2, y2);
	direction = ofVec2f(0, ofRandom(3.0, 5.0));
	force = ofVec2f(0, 0.1);
	fall = false;
}



lineSegment::~lineSegment()
{
};

void	lineSegment::update()
{
	ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());
	float distance = currentPos.distance(mousePos);
	// float hue = ofMap(currentPos.x, 0, ofGetWidth(), 0, 255);
	// color.setHsb(hue, 255, 255, 200);
	color.set(0);
	if (distance < (tileSize.x))
	{
		fall = true;
		direction = currentPos - mousePos;
		direction.normalize();
	}
	if (fall == true)
	{
		currentPos += direction;
		newPos += direction;
		direction += force;
	}
	// if (mousePos.y == currentPos.y || mousePos.y == newPos.y)
	// 	fall = true;
	// if (fall == true)
	// {
	// 	currentPos.x--;
	// 	newPos.x--;
	//
	// }
}

void	lineSegment::draw()
{
	// ofSetColor(255, ofGetFrameNum() % 255);
	ofSetColor(color);
	ofDrawLine(currentPos.x, currentPos.y, newPos.x, newPos.y);
	// ofSetColor(255, 0, 0);
	// ofDrawCircle(newPos, tileSize.x * 0.1);
}

void 	lineSegment::setTileSize(int width, int height)
{
	tileSize.x = width;
	tileSize.y = height;
}
