#pragma once

#include "ofColor.h"
#include "ofMain.h"
#include "ofVec2f.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "lineSegment.hpp"
#include "contourData.hpp"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void coinFlip();
		void exit();
		void drawContour();

		ofVec2f currentPos;		
		ofVec2f newPos;
		ofVec2f	direction;
		ofVec2f	tileSize;
		char	result[2];
		int angle;
		int	nearTreshold;
		int	farTreshold;
		int       stride;
		int		width;
		int		height;

		vector<lineSegment> lineSegments;
		vector<contourData> contourVector;
		ofxKinect kinect;
};
