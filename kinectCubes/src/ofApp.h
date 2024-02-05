#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "Square.h"
#include <vector>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		struct kinectData {
			ofxKinect 			device;
			ofxCvGrayscaleImage grayImage;
			int 				angle;
			int 				farThresh;
			int 				nearThresh;
			int					width;
			int					height;
		};	

		kinectData 	kinectData;
		void 		initKinectData();
		void 		displayKinectData();
		void 		updateKinectGrayImage();
		void 		drawKinectBoxCloud();
		ofEasyCam	cam;
		int			stride;

		vector<Square> squares;
};
