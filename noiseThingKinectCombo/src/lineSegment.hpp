#pragma once

#include "ofColor.h"
#include "ofVec2f.h"
#include "ofMain.h"
#include "contourData.hpp"

class lineSegment{
	public:
	ofVec2f currentPos, newPos, direction, force, tileSize;
	ofColor color;
	char	result[2];
	bool	fall;
	int		timer;

	void	update(vector<contourData> contourVector);
	void	draw();
	void 	setTileSize(ofVec2f tile);
	lineSegment(ofVec2f p1, ofVec2f p2);
	~lineSegment();
};

