#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofTrueTypeFont	title;
		ofTrueTypeFont	subtitle;

		ofxPanel 		guiVisual;
		ofxIntSlider 	xStride;
		ofxIntSlider 	yStride;
		ofxToggle		drawShape;
		ofxToggle		drawLine;
		ofxToggle		drawCircle;
		ofxToggle		fullScreen;
		ofxFloatSlider	noiseMult;
		bool			bGui = false;
		bool			bDrawLine = false;

		float inc;

		ofColor COL_PHANTASM_PRIMARY 	= ofColor(77, 131, 95);
		ofColor	COL_PHANTASM_SECONDARY 	= ofColor(211, 202, 151);
		ofColor	COL_INTERACTIVE 		= ofColor(0);
		ofColor COL_SECRET 				= ofColor(255, 192, 203);
};
