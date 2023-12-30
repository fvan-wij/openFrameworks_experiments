#include "testApp.h"
float tuioXScaler = 1;
float tuioYScaler = 1;

//--------------------------------------------------------------
void testApp::setup() {	 
#ifdef USE_KINECT
	// setup kinect
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetBackgroundAuto(false);
	kinect.setRegistration(true);
	kinect.init(false, false);
	kinect.open();
	grayImage.allocate(kinect.width, kinect.height);
	nearThreshold = 255;
	farThreshold = 218;
	ofSetFrameRate(60);
	angle = 0;
	kinect.setCameraTiltAngle(angle);
#endif

	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);
	
	fluidCellsX			= 150;
	
	drawFluid			= true;
	drawParticles		= true;
	
	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
	ofSetVerticalSync(false);

	// setup beat.h
	ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);
	
	
#ifdef USE_GUI 
	gui.addSlider("fluidCellsX", fluidCellsX, 20, 400);
	gui.addButton("resizeFluid", resizeFluid);
    gui.addSlider("colorMult", colorMult, 0, 100);
    gui.addSlider("velocityMult", velocityMult, 0, 100);
	gui.addSlider("fs.viscocity", fluidSolver.viscocity, 0.0, 0.01);
	gui.addSlider("fs.colorDiffusion", fluidSolver.colorDiffusion, 0.0, 0.0003); 
	gui.addSlider("fs.fadeSpeed", fluidSolver.fadeSpeed, 0.0, 0.1); 
	gui.addSlider("fs.solverIterations", fluidSolver.solverIterations, 1, 50); 
	gui.addSlider("fs.deltaT", fluidSolver.deltaT, 0.1, 5);
	gui.addComboBox("fd.drawMode", (int&)fluidDrawer.drawMode, msa::fluid::getDrawModeTitles());
	gui.addToggle("fs.doRGB", fluidSolver.doRGB); 
	gui.addToggle("fs.doVorticityConfinement", fluidSolver.doVorticityConfinement); 
	gui.addToggle("drawFluid", drawFluid); 
	gui.addToggle("drawParticles", drawParticles); 
	gui.addSlider("velDrawMult", fluidDrawer.velDrawMult, 0.0, 20);
	gui.addSlider("velDrawThreshold", fluidDrawer.velDrawThreshold, 0.0, 1);
	gui.addSlider("brightness", fluidDrawer.brightness, 0.0, 2);
	gui.addToggle("useAdditiveBlending", fluidDrawer.useAdditiveBlending);
	
	gui.addToggle("fs.wrapX", fluidSolver.wrap_x);
	gui.addToggle("fs.wrapY", fluidSolver.wrap_y);
    gui.addSlider("tuioXScaler", tuioXScaler, 0, 2);
    gui.addSlider("tuioYScaler", tuioYScaler, 0, 2);
	// particle colors
	gui.addSlider("particleRed", pRed, 0.0, 1.0);
	gui.addSlider("particleGreen", pGreen, 0.0, 1.0);
	gui.addSlider("particleBlue", pBlue, 0.0, 1.0);
	// beat detection
	gui.addSlider("kickThreshold", kickThreshold, 0.0, 1.0);
	gui.currentPage().setXMLName("ofxMSAFluidSettings.xml");
    gui.loadFromXML();
	gui.setDefaultKeys(true);
	gui.setAutoSave(true);
    gui.show();
#endif

	
	windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cos I don't think it is called)
	pMouse = msa::getWindowCenter();
	resizeFluid			= true;
	
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(false);
}


void testApp::fadeToColor(float r, float g, float b, float speed) {
    glColor4f(r, g, b, speed);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
}


// add force and dye to fluid, and create particles
void testApp::addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce) {
    float speed = vel.x * vel.x  + vel.y * vel.y * msa::getWindowAspectRatio() * msa::getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
    if(speed > 0) {
		pos.x = ofClamp(pos.x, 0.0f, 1.0f);
		pos.y = ofClamp(pos.y, 0.0f, 1.0f);
		
        int index = fluidSolver.getIndexForPos(pos);
		
		if(addColor) {
			ofColor drawColor;
			drawColor.setHsb(100, 255, 255);
			
			fluidSolver.addColorAtIndex(index, drawColor * colorMult);
			
			if(drawParticles)
				particleSystem.addParticles(pos * ofVec2f(ofGetWindowSize()), 10);
		}
		
		ofVec2f velocity = vel * velocityMult;
		velocity.normalize();
		if(addForce)
			fluidSolver.addForceAtIndex(index, velocity);
		
    }
}


void testApp::update(){
beat.update(ofGetElapsedTimeMillis());
#ifdef USE_KINECT
	kinect.update();
	if (kinect.isFrameNew())
	{
		grayImage.setFromPixels(kinect.getDepthPixels());
		ofPixels & pix 			= grayImage.getPixels();
		int 	   numPixels 	= pix.size();
		for (size_t i = 0; i < numPixels; i++)
		{
			if (pix[i] < nearThreshold && pix[i] > farThreshold)
				pix[i] = 255;
			else 
				pix[i] = 0;
		}
		grayImage.flagImageChanged();
		grayImage.mirror(false, true); // Flip horizontally
		contourFinder.findContours(grayImage, 10, (kinect.width * kinect.height) / 2, 20, false);
	}
#endif

	if(resizeFluid) 	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX / msa::getWindowAspectRatio());
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}
	
	fluidSolver.update();
	if (beat.kick() >= kickThreshold)
		drawParticles = true;
	else
		drawParticles = false;
}

void testApp::draw(){
	if(drawFluid) {
        ofClear(0);
		glColor3f(1, 1, 1);
		fluidDrawer.draw(0, 0, ofGetWidth(), ofGetHeight());
	} else {
            fadeToColor(0, 0, 0, 0.01);
	}
	if(drawParticles)
	{
		glColor3f(1, 0, 0);
		particleSystem.updateAndDraw(fluidSolver, ofGetWindowSize(), drawFluid, pRed, pGreen, pBlue);
	}

#ifdef USE_KINECT
	int i = 0;
	while (i < contourFinder.nBlobs)
	{
		contourFinder.blobs[i].draw(0, 0);

		ofRectangle r = contourFinder.blobs[i].boundingRect;

		if (r.width > 50.0 && r.height > 50.0)
		{
			float rx = ofMap(r.x + (r.width * 0.5), 0, kinect.getWidth(), 0, ofGetWindowWidth());
			float ry = ofMap(r.y + (r.height * 0.5), 0, kinect.getHeight(), 0, ofGetWindowHeight());
			ofVec2f handPos(rx, ry);
			ofVec2f eventPos = ofVec2f(handPos.x, handPos.y);
			ofVec2f handNorm = ofVec2f(eventPos) / ofGetWindowSize();
			ofVec2f handVel = ofVec2f(eventPos - pHand) / ofGetWindowSize();
			addToFluid(handNorm, handVel, true, true);
			pHand = eventPos;
		}
		i++;
	}
#endif

#ifdef USE_GUI 
	if (!bHideGui)
		gui.draw();
#endif

#ifdef USE_KINECT
	drawKinectSettings();
#endif
  std::cout << beat.kick() << std::endl;
}


void testApp::keyPressed  (int key){ 
    switch(key) {
		case '1': fluidDrawer.setDrawMode(msa::fluid::kDrawColor); break;
		case '2': fluidDrawer.setDrawMode(msa::fluid::kDrawMotion);	break;
		case '3': fluidDrawer.setDrawMode(msa::fluid::kDrawSpeed); break;
		case '4': fluidDrawer.setDrawMode(msa::fluid::kDrawVectors); break;
		case 'd': drawFluid ^= true; break;
		case 'p': drawParticles ^= true; break;
		case 'f': ofToggleFullscreen(); break;
		case 'r': fluidSolver.reset(); break;
		case 'g': bHideGui = !bHideGui;

	#ifdef USE_KINECT	
		case '>':
		case '.':
			farThreshold ++; if (farThreshold > 255) farThreshold = 255; break;
		case '<':
		case ',':
			farThreshold --; if (farThreshold < 0) farThreshold = 0; break;
		case '+':
		case '=':
			nearThreshold ++; if (nearThreshold > 255) nearThreshold = 255; break;
		case '-':
			nearThreshold --; if (nearThreshold < 0) nearThreshold = 0;	break;
			
		case 'w': kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite()); break;
		case 'o': kinect.setCameraTiltAngle(angle); kinect.open(); break;
		case 'c': kinect.setCameraTiltAngle(0);	kinect.close();	break;

		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
	#endif	
    }
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	ofVec2f eventPos = ofVec2f(x, y);
	ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
	ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
	addToFluid(mouseNorm, mouseVel, true, true);
	pMouse = eventPos;
}

void testApp::mouseDragged(int x, int y, int button) {
	ofVec2f eventPos = ofVec2f(x, y);
	ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
	ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
	addToFluid(mouseNorm, mouseVel, false, true);
	pMouse = eventPos;
}

//ofxBeat stuff
void testApp::audioReceived(float* input, int bufferSize, int nChannels) {
  beat.audioReceived(input, bufferSize, nChannels);
}


//ofxKinect stuff
#ifdef USE_KINECT
void testApp::drawKinectSettings()
{
	ofSetColor(255);
	stringstream reportStream;

	reportStream << "set near treshold " << nearThreshold << " (press: + -)" << endl
				 << "set far treshold " << farThreshold << " (press: < >)" << endl
				 << ", fps: " << ofGetFrameRate() << endl
				 << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
	if (kinect.hasCamTiltControl())
		reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl;

	ofDrawBitmapString(reportStream.str(), ofGetWidth() - 600, ofGetHeight() - 75);
}
#endif

