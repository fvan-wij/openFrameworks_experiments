#pragma once

#include "ofMain.h"
#include "ofxBeat.h"
#include "ofxGui.h"


//Kinect
#include "ofxKinect.h"
#include "ofxOpenCv.h"

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

		void audioReceived(float* input, int bufferSize, int nChannels);
		ofxBeat 	beat;
		float 		inc;

	//----------------------GuiVisuals-------------------------------
		ofxPanel 		guiVisual;
		ofxIntSlider 	xStride;
		ofxIntSlider 	yStride;
		ofxToggle		drawShape;
		ofxToggle		drawLine;
		ofxToggle		drawCircle;
		ofxToggle		fullScreen;
		ofxFloatSlider	noiseMult;

	//----------------------GuiAudio---------------------------------
		ofxPanel 		guiAudio;
		ofxIntSlider 	listenToBand;
		ofxToggle		toggBand;
		ofxFloatSlider 	kickThresh;
		ofxFloatSlider 	kickIntensity;

		ofxToggle		listenToKick;
		ofxToggle		listenToSnare;	
		ofxToggle		listenToHat;	

		ofxFloatSlider 	kickVolume;
		ofxFloatSlider	snareVolume;
		ofxFloatSlider	hatVolume;

		ofxFloatSlider	intensityMult;

	//----------------------Fonts------------------------------------
		ofTrueTypeFont	title;

	//----------------------ofxKinect--------------------------------
	ofxKinect 				kinect;
	void 					drawKinectSettings();
	int 					nearThreshold;
	int 					farThreshold;
	int 					angle;

	//						ofxOpenCV
	ofVec2f 				pHand;
	ofxCvGrayscaleImage 	grayImage;
	ofxCvContourFinder 		contourFinder;
};
