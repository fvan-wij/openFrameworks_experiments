#pragma once

#include "ofMain.h"
#include "ofColor.h"
#include "ofVec2f.h"

class contourData{
public:
	ofVec2f currentPos, size;
	ofColor color;
	bool visible;
	int strideX, strideY;

	void update();
	void draw();
	contourData(int x, int y,  ofVec2f size);
	~contourData();
	
};

