#pragma once

#include "MSAFluid.h"
#include "ParticleSystem.h"
#include "ofxBeat.h"
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSimpleGuiToo.h"
#include "ofxPostProcessing.h"


#define USE_KINECT

#ifdef USE_KINECT
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#endif

class testApp : public ofBaseApp {
public:
	//----------------------GUI--------------------------------------

	//						GUI - MSAFluid settings
	float                   colorMult;
	float                   velocityMult;
	int                     fluidCellsX;
	bool                    resizeFluid;
	bool                    drawFluid;
	bool                    drawParticles;

	//						GUI sliders - Particle colors
	float 					pRed;
	float 					pGreen;
	float 					pBlue;

	// 						GUI slider - Beat detection treshold
	float 					kickThreshold;

	bool 					bHideGui				= false;
	void 					guiInitialize(void);

	//----------------------ofxMSAFluid------------------------------

	//						ofxMSAFluid Variables
	msa::fluid::Solver      fluidSolver;
	msa::fluid::DrawerGl	fluidDrawer;
	ParticleSystem          particleSystem;
	ofVec2f                 pMouse;
	float 					vectorLength;
	float					forceIntensity;

	//						ofxMSAFluid Methods
	void 					fadeToColor(float r, float g, float b, float speed);
	void 					addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce);
	void 					setupFluid();

	//----------------------ofApp------------------------------------
	void 					setup();
	void 					update();
	void 					draw();
	
	void 					keyPressed  (int key);
	void 					mouseMoved(int x, int y );
	void 					mouseDragged(int x, int y, int button);


	//----------------------ofxBeat----------------------------------
	ofxBeat 				beat;
	void 					audioReceived(float*, int, int);
	void 					detectKick(void);
	bool					bToggleParticles		= false;
	bool					bToggleEdge				= false;
	bool					bToggleReactiveColors	= false;
	long		 			start_time;
	long					end_time;

	//---------------------------------------------------------------
	ofxPostProcessing		post;
	bool					bFullscreen				= false;
	typedef enum e_postfx
	{
		FXAA,
		BLOOM,
		DOF,
		KALEID,
		WARP,
		PIXELATE,
		EDGE,
		VERTICALTILT
	} e_postfx;

#ifdef USE_KINECT
	//----------------------ofxKinect--------------------------------
	ofxKinect 				kinect;
	void 					drawKinectSettings();
	void					kinectAddBlobsToFluid(void);
	void					kinectUpdateBlobs(void);
	void					kinectSetup(void);
	int 					nearThreshold;
	int 					farThreshold;
	int 					angle;

	//						ofxOpenCV
	ofVec2f 				pHand;
	ofxCvGrayscaleImage 	grayImage;
	ofxCvContourFinder 		contourFinder;
#endif

};
