#pragma once

#include "ofGraphicsBaseTypes.h"
#include "ofMain.h"
#include "ofVec2f.h"

class particle {

public:

	ofColor color;
	float	size;
	ofVec2f force, position, direction, idle;


	void update();
	void draw();

	particle(int x, int y, ofColor color);
	~particle();
	
};

typedef enum e_state {
	IDLE,
	DRIFT,
} e_state;


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

		//Cam & Pixeldata
		ofVideoGrabber	myCam;
		ofPixels		camPixelData;

		//Particle array
		vector<particle> particles;
};

