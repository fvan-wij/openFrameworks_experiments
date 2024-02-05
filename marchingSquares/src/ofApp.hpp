#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#define WIDTH 1680
#define HEIGHT 1050

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void initializeFieldVector();
		void marchingSquares();
		void exit();
		void keyPressed(int key);
		void displayReport();

		//Gobal Variables
		vector <vector<float>> FIELD;
		ofVec2f TILESIZE;
		int		ROWS;
		int		COLS;	
		int		RES;
		float		XOFFSET;
		float		YOFFSET;
		float		ZOFFSET;
		float		INC;

		ofxKinect kinect;
		int nearTreshold;
		int farTreshold;
		float widthMultiplier;
		float heightMultiplier;
		ofxCvGrayscaleImage grayImage;
};
