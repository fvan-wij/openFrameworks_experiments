#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

#define WIDTH 1024
#define HEIGHT 768

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

		//Kinect
		void drawKinectSettings();
		ofxKinect			kinect;
		ofxCvGrayscaleImage grayImage;
		ofxCvContourFinder	contourFinder;
		int					nearThreshold;
		int					farThreshold;
		int					angle;
	
		//Shader
		ofShader 			shader;
		ofPlanePrimitive	plane; //Allows us to create a plane based on the vertex data we give it. We can then proceed to move the vertices around using uniforms in our shader.vert;
		vector<ofVec2f>		hands;
		int					nHands; // Size of hands vector

};
