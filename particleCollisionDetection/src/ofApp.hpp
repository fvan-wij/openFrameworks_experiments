#pragma once

#include "ofColor.h"
#include "ofMain.h"
#include "ofSoundBaseTypes.h"
#include "ofSoundBuffer.h"
#include "ofSoundPlayer.h"
#include "ofSoundStream.h"
#include "particle.hpp"
#include "ofxKinect.h"
#include "kinectPoint.hpp"
#include "ofxOpenCv.h"
#include <vector>

//Initialize globals
#define WIDTH 1920
#define HEIGHT 1080
#define TILES 8
#define PARTICLES 500
#define STRIDE 16

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawMatrixRasterization();
		void triggerTimer();
		void updateMatrixRasterization(bool move);
		void resolveCollision(particle& p1, particle& p2);
		void spacePartitionParticles();
		vector<particle *> particleIsWithinCell(ofVec2f cell);
		void checkCollision(vector<particle *> particlePointers);
		void exit();
		void drawContour();
		void keyPressed(int key);
		
		void audioIn(ofSoundBuffer & input);

		ofVec2f tileSize;
		vector<particle> particles;
		vector<vector<ofVec2f>> cellVector;

		// Kinect
		ofxKinect kinect;
		float			angle;
		int			nearTreshold;
		int			farTreshold;
		int			stride;
		int			state;
		int			kinectWidth;
		int			kinectHeight;
		vector<vector<kinectPoint>> kinectPoints;
		ofxCvGrayscaleImage grayImage;
		ofTrueTypeFont font;
		ofColor			newColor;


		// Audio in
		vector <float>  left;
		vector <float>  right;
		vector <float>  volHistory;

		int				bufferCounter;
		int				drawCounter;
		float			smoothedVol;
		float			scaledVol;
		ofSoundStream	soundStream;
		bool			toggle;

		// float			volume;
		// float			decay;
		// ofSoundStream	input;
		// float			*fft;
		// float			*soundSpectrum;
		// int				bands;
};
