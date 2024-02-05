#pragma once

#include "ofColor.h"
#include "ofMain.h"
#include "ofVec2f.h"

class lineSegment{
	public:
	ofVec2f currentPos, newPos, direction, force, tileSize;
	ofColor color;
	char	result[2];
	bool	fall;

	void	update();
	void	draw();
	void 	setTileSize(int width, int height);
	lineSegment(int x, int y, int x2, int y2);
	~lineSegment();
};

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void coinFlip();
		void diceRoll();

	ofVec2f currentPos;		
	ofVec2f newPos;
	ofVec2f	direction;
	ofVec2f	tileSize;
	char	result[2];
	vector<lineSegment> lineSegments;
	vector<lineSegment> it;
};
