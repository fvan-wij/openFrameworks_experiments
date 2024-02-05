#include "testApp.h"
float tuioXScaler = 1;
float tuioYScaler = 1;


//--------------------------------------------------------------
void testApp::setup() {	 
	//Scene management
	osc.setup(PORT);
	ofSetFrameRate(60);
	scene = 0;
	touchArtist = 0;
	strideXY = ofVec2f(4, 4);

	//secret scene
	img.load("images/sla.png");
	fbo.allocate(ofGetWidth(), ofGetHeight());

	// setup gui
	guiInitialize();
	titleScreenGuiInitialization();	

	// ofSettings
	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(false);

	// setup fluid
	setupFluid();

	// setup kinect

#ifdef USE_KINECT
	kinectSetup();
#endif

	// setup soundsettings
	ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);

	post.init(ofGetWidth(), ofGetHeight());
   	post.createPass<FxaaPass>()->setEnabled(false);
    post.createPass<BloomPass>()->setEnabled(false);
    post.createPass<DofPass>()->setEnabled(false);
    post.createPass<KaleidoscopePass>()->setEnabled(false);
    post.createPass<NoiseWarpPass>()->setEnabled(false);
    post.createPass<PixelatePass>()->setEnabled(false);
    post.createPass<EdgePass>()->setEnabled(false);
    post.createPass<VerticalTiltShifPass>()->setEnabled(false);

    int fontsize = 120;
	int aristsize = 70;
    title.loadFont("fonts/Morelife.otf", fontsize);
	artist.loadFont("fonts/WaterResistant.ttf", aristsize);
}

void testApp::update(){

while (osc.hasWaitingMessages())
{
	ofxOscMessage message;
	osc.getNextMessage(message);
	if (message.getAddress() == "/1/scene")
	{
		std::cout << "Message received!" << std::endl;
		scene += message.getArgAsFloat(0);
	}
	if (message.getAddress() == "/1/artist")
	{
		std::cout << "Message received!" << std::endl;
		touchArtist = ofMap(message.getArgAsFloat(0), 0, 1, 0, 5);
	}
	if (message.getAddress() == "/1/stridexy")
	{
		std::cout << "Message received!" << std::endl;
		strideXY.x = ofMap(message.getArgAsFloat(0), 0, 1, 64, 1);
		strideXY.y = ofMap(message.getArgAsFloat(1), 0, 1, 64, 1);
		xStride = strideXY.x;
		yStride = strideXY.y;
	}
	if (message.getAddress() == "/1/stridexy")
	{
		std::cout << "Message received!" << std::endl;
		strideXY.x = ofMap(message.getArgAsFloat(0), 0, 1, 64, 1);
		strideXY.y = ofMap(message.getArgAsFloat(1), 0, 1, 64, 1);
		xStride = strideXY.x;
		yStride = strideXY.y;
	}
	if (message.getAddress() == "/1/touchxy")
	{
		std::cout << "Message received!" << std::endl;
		touchXY.x = ofMap(message.getArgAsFloat(0), 0, 1, 0, ofGetWidth());
		touchXY.y = ofMap(message.getArgAsFloat(1), 0, 1, ofGetHeight(), 0);
	}
#ifdef USE_KINECT
	if (message.getAddress() == "/1/near")
	{
		std::cout << "Message received!" << std::endl;
		farThreshold = ofMap(message.getArgAsFloat(0), 0, 1, 0, 255);
	}
#endif

	if (message.getAddress() == "/1/minbox")
	{
		std::cout << "Message received!" << std::endl;
		touchMinBox = ofMap(message.getArgAsFloat(0), 0, 1, 10, 100);
	}
	if (message.getAddress() == "/1/maxbox")
	{
		std::cout << "Message received!" << std::endl;
		touchMaxBox = ofMap(message.getArgAsFloat(0), 0, 1, 50, 300);
	}

}
if (scene == INTERACTIVE_SCENE)
{
	#ifdef USE_KINECT
		kinectUpdateBlobs();
	#endif
	if (resizeFluid)
	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX / msa::getWindowAspectRatio());
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}
	fluidSolver.update();
}
else if (scene == TITLE_SCENE)
{
    float intensity;

    beat.update(ofGetElapsedTimeMillis());
    kickVolume = beat.kick();
    snareVolume = beat.snare();
    hatVolume = beat.hihat();

    if (toggBand)
        intensity = beat.getBand(listenToBand);
    else if (listenToKick)
        intensity = kickVolume;
    else if (listenToSnare)
        intensity = snareVolume;
    else if (listenToHat)
        intensity = hatVolume;

    if (intensity >= kickThresh)
    {
        inc = intensity * intensityMult;
    }
}

if (bFullscreen)
{
	post.setWidth(ofGetWindowWidth());
	post.setHeight(ofGetWindowHeight());
}
detectKick();

// Scene management
if (scene > 2)
	scene = 0;
}

void testApp::draw(){
	if (scene == INTERACTIVE_SCENE)
	{
		// post.begin();
		ofBackground(COL_INTERACTIVE);
		if (drawFluid)
		{
			ofClear(0);
			glColor3f(1, 1, 1);
			fluidDrawer.draw(0, 0, ofGetWidth(), ofGetHeight());
		}
		else
			fadeToColor(0, 0, 0, 0.01);
		if (drawParticles)
		{
			glColor3f(1, 0, 0);
			particleSystem.updateAndDraw(fluidSolver, ofGetWindowSize(), drawFluid, pRed, pGreen, pBlue);
		}
#ifdef USE_KINECT
		kinectAddBlobsToFluid();
		if (bHideDebugInfo)
			drawKinectSettings();
#endif
		if (!bHideGui)
			gui.draw();
		// post.end();
	}
	else if (scene == TITLE_SCENE)
	{
		ofBackground(COL_PHANTASM_PRIMARY);
		ofSetColor(COL_PHANTASM_SECONDARY);
		int offset = 20;
		for (size_t y = offset; y < ofGetViewportHeight() - (offset * 2); y += yStride)
		{
			ofPolyline polyline;
			for (size_t x = offset; x < ofGetViewportWidth() - offset; x += xStride)
			{
				float noise = ofNoise(x, y);
				ofSetLineWidth(noise * 2.0);
				ofVec2f point(x, y);
				ofVec2f mousePoint(mouseX, mouseY);

				//Disable if you want to use mouse instead of touchOSC
				mousePoint.x = touchXY.x;
				mousePoint.y = touchXY.y;

				float dist = point.distance(mousePoint);
				noise *= dist * inc;
				polyline.addVertex(x, y + noise * noiseMult);
				if (drawShape)
				{
					if (x % 32 == 0)
					{
						ofPushMatrix();
						ofSetRectMode(OF_RECTMODE_CENTER);
						ofNoFill();
						ofTranslate(x + noise * noiseMult, y + noise);
						ofRotate(mouseX);
						ofDrawCircle(0, 0, noise * noiseMult);
						// ofDrawRectangle(0, 0, noise * .1, noise * .1);
						// ofDrawRectangle(0, 0, noise * .1, noise * .1);
						ofPopMatrix();
					}
				}
			}
			if (drawLine)
				polyline.draw();
		}

		if (!bHideGui)
		{
			guiVisual.draw();
			guiAudio.draw();
		}
		ofSetColor(COL_PHANTASM_SECONDARY);
		ofNoFill();
		ofSetLineWidth(5);
		title.drawString("PHANTASM AUDIO", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0);
		if (touchArtist != 5)
			ofDrawLine(ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 40, ofGetViewportWidth() - 250, ofGetViewportHeight() / 2.0 + 40);
		if (touchArtist == CAS_AUGUST)
			artist.drawString("CAS AUGUST W/ RWA", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 150);
		if (touchArtist == IMMUTE)
			artist.drawString("IMMUTE", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 150);
		if (touchArtist == SUNNY_SAIMUNS)
			artist.drawString("SUNNY SAIMUNS", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 150);
		if (touchArtist == YICE)
			artist.drawString("YICE", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 150);
		if (touchArtist == CONIFEAR)
			artist.drawString("CONIFEAR", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 150);
	}
	else if (scene == SECRET_SCENE)
	{
	#ifdef USE_KINECT

		post.begin();
		ofBackground(25);
		kinectUpdateBlobs();
		for (int i = 0; i < contourFinder.nBlobs; i++)
		{
			ofRectangle r = contourFinder.blobs[i].boundingRect;
			float rx = ofMap(r.x + (r.width * 0.5), 0, kinect.getWidth(), 0, ofGetWidth());
			float ry = ofMap(r.y + (r.height * 0.5), 0, kinect.getHeight(), 0, ofGetHeight());
			ofVec2f handPos(rx, ry);
			img.draw(rx, ry, r.width, r.height);
			if (bHideDebugInfo)
			{
				contourFinder.blobs[i].draw(0, 0);
				drawKinectSettings();
			}
		}

		ofSetColor(COL_PHANTASM_SECONDARY);
		ofNoFill();
		ofSetLineWidth(5);
		title.drawString("PHANTASM AUDIO", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0);
		if (touchArtist != 5)
			ofDrawLine(ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 40, ofGetViewportWidth() - 250, ofGetViewportHeight() / 2.0 + 40);
		if (touchArtist == CAS_AUGUST)
			artist.drawString("CAS AUGUST W/ RWA", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 150);
		if (touchArtist == IMMUTE)
			artist.drawString("IMMUTE", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 150);
		if (touchArtist == SUNNY_SAIMUNS)
			artist.drawString("SUNNY SAIMUNS", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 150);
		if (touchArtist == YICE)
			artist.drawString("YICE", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 150);
		if (touchArtist == CONIFEAR)
			artist.drawString("CONIFEAR", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 150);
		post.end();
	#endif
	}
}


void testApp::keyPressed  (int key){ 
 	unsigned idx = key - '0';
    if (idx < post.size()) post[idx]->setEnabled(!post[idx]->getEnabled());
    switch(key) {
		// case '1': fluidDrawer.setDrawMode(msa::fluid::kDrawColor); break;
		// case '2': fluidDrawer.setDrawMode(msa::fluid::kDrawMotion);	break;
		// case '3': fluidDrawer.setDrawMode(msa::fluid::kDrawSpeed); break;
		// case '4': fluidDrawer.setDrawMode(msa::fluid::kDrawVectors); break;
		case 'D': bHideDebugInfo = !bHideDebugInfo; break;
		case 'd': drawFluid ^= true; break;
		case 'p': drawParticles ^= true; break;
		case 'f': ofToggleFullscreen(); bFullscreen = !bFullscreen;  break;
		case 'r': fluidSolver.reset(); break;
		case 'g': bHideGui = !bHideGui; break;

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

		case OF_KEY_ALT:
			scene++;
			break;
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

//msaFluid stuff
void testApp::setupFluid()
{
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);
	
	fluidCellsX			= 150;
	drawFluid			= true;
	drawParticles		= true;
	windowResized(ofGetWidth(), ofGetHeight());
	pMouse = msa::getWindowCenter();
	resizeFluid			= true;
}

void testApp::fadeToColor(float r, float g, float b, float speed) {
    glColor4f(r, g, b, speed);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
}

void testApp::addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce) {
    float speed = vel.x * vel.x  + vel.y * vel.y * msa::getWindowAspectRatio() * msa::getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
    if(speed > 0) {
		pos.x = ofClamp(pos.x, 0.0f, 1.0f);
		pos.y = ofClamp(pos.y, 0.0f, 1.0f);
		
        int index = fluidSolver.getIndexForPos(pos);
		
		if(addColor) {
			ofColor drawColor;
			drawColor.setHsb(100 + (forceIntensity * 10.0), 255, 255);
			
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

//ofxBeat stuff
void testApp::audioReceived(float* input, int bufferSize, int nChannels) {
  beat.audioReceived(input, bufferSize, nChannels);
}

void testApp::detectKick(void)
{
	beat.update(ofGetElapsedTimeMillis());
	forceIntensity = beat.getMagnitude();

	if (bToggleParticles)
	{
		if (beat.kick() >= kickThreshold || forceIntensity >= 20.0)
			drawParticles = true;
		else
			drawParticles = false;
	}

	if (bToggleEdge)
	{
		if (beat.kick() >= kickThreshold)
			post[EDGE]->setEnabled(!post[EDGE]->getEnabled());
		else
			post[EDGE]->setEnabled(post[EDGE]->getEnabled());
	}

	if (beat.kick() >= kickThreshold && bToggleReactiveColors)
	{
		pRed = ofRandom(1.0);
		pGreen = ofRandom(1.0);
		pBlue = ofRandom(1.0);
	}
}

//ofxKinect stuff
#ifdef USE_KINECT

void	testApp::kinectSetup(void)
{
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
}

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

void	testApp::kinectUpdateBlobs(void)
{
	kinect.update();
	if (kinect.isFrameNew())
	{
		grayImage.setFromPixels(kinect.getDepthPixels());
		ofPixels &pix = grayImage.getPixels();
		int numPixels = pix.size();
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
}

void	testApp::kinectAddBlobsToFluid(void)
{
	int i;

	i = 0;
	while (i < contourFinder.nBlobs)
	{
		if (bHideDebugInfo)
			contourFinder.blobs[i].draw(0, 0);

		ofRectangle r = contourFinder.blobs[i].boundingRect;

		if (r.width > touchMinBox && r.height > touchMinBox && r.width < touchMaxBox && r.height < touchMaxBox)
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
}

#endif

void testApp::guiInitialize(void)
{
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
	gui.addToggle("toggleReactiveParticles", bToggleParticles);
	gui.addToggle("toggleReactiveEdge", bToggleEdge);
	gui.addToggle("toggleReactiveParticleColors", bToggleReactiveColors);

	// beat detection
	gui.addSlider("kickThreshold", kickThreshold, 0.0, 1.0);
	gui.currentPage().setXMLName("ofxMSAFluidSettings.xml");
    gui.loadFromXML();
	gui.setDefaultKeys(true);
	gui.setAutoSave(true);
    gui.show();
}

void testApp::titleScreenGuiInitialization(void)
{
    //GUI Visuals
    guiVisual.setup();
    guiVisual.add(xStride.setup("xStride", 4, 1, 64));
    guiVisual.add(yStride.setup("yStride", 4, 1, 64));
    guiVisual.add(drawLine.setup("drawLine", true));
    guiVisual.add(drawShape.setup("drawShape", false));
    guiVisual.add(drawCircle.setup("drawCircle", false));
    guiVisual.add(fullScreen.setup("fullscreen", false));
    guiVisual.add(noiseMult.setup("noiseMultiplier", 0.01, -1.0, 1.0));

    //GUI Audio
    guiAudio.setup();
    guiAudio.setPosition(10.0, 200.0);
    guiAudio.add(kickThresh.setup("kickThreshold", 0.0, 0.0, 1.0));
    guiAudio.add(kickIntensity.setup("kickIntensity", 2.0, 0.0, 10.0));
    guiAudio.add(listenToBand.setup("listenToBand", 0, 0, 31));

    guiAudio.add(toggBand.setup("listenToBand", true));
    guiAudio.add(listenToKick.setup("listenToKick", false));
    guiAudio.add(listenToSnare.setup("listenToSnare", false));
    guiAudio.add(listenToHat.setup("listenToHat", false));
    
    guiAudio.add(kickVolume.setup("kickVolume", 0.0, 0.0, 1.0));
    guiAudio.add(snareVolume.setup("snareVolume", 0.0, 0.0, 1.0));
    guiAudio.add(hatVolume.setup("hatVolume", 0.0, 0.0, 1.0));

    guiAudio.add(intensityMult.setup("intensityMultiplier", 1.0, 0.1, 10.0));
}