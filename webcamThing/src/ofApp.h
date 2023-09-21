#pragma once

#include "ofMain.h"
#include "ofTexture.h"
#include "ofVideoGrabber.h"

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
		void rasterizeCamera();
		void colourizeCameraPixels(int r, int g, int b);
		void glitchPixels();

		ofVideoGrabber 	cam;
		ofPixels		videoPixels;
		ofTexture		videoTexture;
		typedef enum e_state {
			OFF,
			RASTERIZE,
			COLOURIZE,
			GLITCH,
		} e_state;
		int state = 0;
};
