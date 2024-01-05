#pragma once

#include "ofMain.h"

//Ofx Addons
#include "ofxKinect.h"
#include "ofxPDSP.h"
#include "ofxGui.h"

//Classes
#include "Square.h"
// #include <vector>
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

		ofEasyCam		cam;
		
		//	 			Kinect
		struct 			k_Settings
		{
			int 			angle 		= 0;
			int				farThresh 	= 255;
			int				nearThresh 	= 200;
			int				distThresh  = 50;
			int				stride		= 8;
			int				scaleX		= 0;
			int				scaleY		= 0;
		};

		ofxKinect 			kinect;
		k_Settings			kinectSettings;
		void 				initKinect();
		void 				displayKinectSettings();
		void 				initSquareGrid();

		//					Squares
		vector<Square> 		squares;
		vector<Square*> 	squarePtrs;
		size_t				highlightIndex = 0;

		//					Lighting
		ofLight				light;

		//					Audio
		pdsp::Engine            engine;
		
		pdsp::VAOscillator      osc;
		pdsp::Amp               amp;
		pdsp::ADSR              env;
		pdsp::TriggerControl    gate_ctrl;
		pdsp::ValueControl      pitch_ctrl;

        // float scale[8] = { 72.0f, 74.0f, 75.0f, 79.0f, 80.0f, 84.0f, 86.0f, 87.0f }; // akebono scale
		float scale[8] = { 60.0f, 62.0f, 64.0f, 67.0f, 69.0f, 72.0f, 74.0f, 76.0f }; //pentatonicMajorScale
		int octave;
		float p = 72.0f;
		//					Gui
        ofxPanel                    gui;
};
