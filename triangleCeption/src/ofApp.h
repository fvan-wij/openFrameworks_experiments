#pragma once

#include "ofMain.h"
#include "ofxGui.h"
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

		//Kinect
		void initKinectSettings();
		void drawKinectSettings();
		void findKinectContours();
		void drawKinectContours();
		ofxKinect			kinect;
		ofxCvGrayscaleImage grayImage;
		ofxCvContourFinder	contourFinder;
		int					angle = 0;

		//Gui
		void initGui();
		ofxPanel 				gui;
		ofParameter <int> 		uiAmount;
		ofParameter <ofVec3f> 	uiPower;
		ofParameter <float> 	uiRadius;
		ofParameter <ofVec2f> 	uiPosition;
		ofParameter <int>		uiNearThresh;
		ofParameter <int>		uiFarThresh;
		ofParameter <ofColor>	uiTrianCol;
		ofParameter <ofColor>	uiBgCol;
		
		//LinkedList
		class Node {
			public:
			vector <ofVec3f> p;
			Node 	*next;
		};
		Node *triangleList;
		Node *addTriangleNode(Node *head);
		Node *addLast(Node *head, Node *newNode);
		void deleteTriangleList(Node **head);

		ofEasyCam				cam;
		bool					bHideGui = false;

		void drawTriangleCeption(ofVec2f p1, ofVec2f p2, ofVec2f p3);
};
