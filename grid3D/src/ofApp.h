#pragma once

#include "of3dPrimitives.h"
#include "ofEasyCam.h"
#include "ofGraphicsBaseTypes.h"
#include "ofMain.h"
#include "ofTexture.h"
#include "ofVec2f.h"

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

		//Sketch parameters
		int camWidth;
		int camHeight;
		int screenWidth;
		int screenHeight;
		int	tileResolution;
		ofVec2f tileSize;

		//Pixel & texture data
		ofVideoGrabber 	myWebcam;
		ofPixels		camPixelData;
		ofTexture		myCamTexture;

		//3D Primitives
		ofBoxPrimitive	newBox;
		ofVec2f boxPos;
		
		//Camera
		ofEasyCam		cam;
		
};
