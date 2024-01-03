#pragma once

#include "MSAFluid.h"
#include "ParticleSystem.h"
#include "ofxBeat.h"
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSimpleGuiToo.h"
#include "ofxPostProcessing.h"
#include "ofxOsc.h"

#define PORT 9000
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

	typedef enum e_artist
	{
		CAS_AUGUST,
		IMMUTE,
		SUNNY_SAIMUNS,
		CONIFEAR,
		YICE
	} e_artist;

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


//----------------------sceneManagement------------------------------
	typedef enum e_scene
	{
		INTERACTIVE_SCENE,
		TITLE_SCENE,
		SECRET_SCENE
	} e_scene;
	int						scene;

//----------------------TITLE_SCENE------------------------------
		void titleScreenGuiInitialization(void);
		ofTrueTypeFont	title;
		ofTrueTypeFont	artist;

		//GUI Visuals
		ofxPanel 		guiVisual;
		ofxIntSlider 	xStride;
		ofxIntSlider 	yStride;
		ofxToggle		drawShape;
		ofxToggle		drawLine;
		ofxToggle		drawCircle;
		ofxToggle		fullScreen;
		ofxFloatSlider	noiseMult;

		//GUI Audio
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
		float 			inc;
		ofxOscReceiver 	osc;

		int uiScene;
		int	touchArtist;
		ofVec2f	strideXY;
		ofVec2f touchXY;
		float touchMinBox = 25;
		float touchMaxBox = 100;
		bool bHideDebugInfo = false;


		//Secret scene
		ofImage img;
		ofFbo	fbo;
		int		pW;
		int		pH;

		ofColor COL_PHANTASM_PRIMARY 	= ofColor(77, 131, 95);
		ofColor	COL_PHANTASM_SECONDARY 	= ofColor(211, 202, 151);
		ofColor	COL_INTERACTIVE 		= ofColor(0);
		ofColor COL_SECRET 				= ofColor(255, 192, 203);
};
