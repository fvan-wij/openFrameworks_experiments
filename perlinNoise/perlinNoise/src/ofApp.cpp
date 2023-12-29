#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //Kinect
	kinect.setRegistration(true);
	kinect.init(false, false);
	kinect.open();
	grayImage.allocate(kinect.width, kinect.height);
	nearThreshold = 255;
	farThreshold = 218;
	ofSetFrameRate(60);
	angle = 0;
	kinect.setCameraTiltAngle(angle);

    ofBackground(77, 131, 95);
    ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);
    inc = 0;

    //GUI Visuals
    guiVisual.setup();
    guiVisual.add(xStride.setup("xStride", 4, 1, 64));
    guiVisual.add(yStride.setup("yStride", 4, 1, 64));
    guiVisual.add(drawLine.setup("drawLine", true));
    guiVisual.add(drawShape.setup("drawShape", false));
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

    int fontsize = 120;
    title.loadFont("fonts/Morelife.otf", fontsize);

}

//--------------------------------------------------------------
void ofApp::update(){
    float intensity;
    //Kinect
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

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetHexColor(0xD3CA97);

    //Kinect
	int i = 0;
	while (i < contourFinder.nBlobs)
	{
		contourFinder.blobs[i].draw(0, 0);

		// ofRectangle r = contourFinder.blobs[i].boundingRect;

		// if (r.width > 50.0 && r.height > 50.0)
		// {
		// 	float rx = ofMap(r.x + (r.width * 0.5), 0, kinect.getWidth(), 0, ofGetWindowWidth());
		// 	float ry = ofMap(r.y + (r.height * 0.5), 0, kinect.getHeight(), 0, ofGetWindowHeight());
		// 	ofVec2f handPos(rx, ry);
		// 	ofVec2f eventPos = ofVec2f(handPos.x, handPos.y);
		// 	ofVec2f handNorm = ofVec2f(eventPos) / ofGetWindowSize();
		// 	ofVec2f handVel = ofVec2f(eventPos - pHand) / ofGetWindowSize();
		// 	addToFluid(handNorm, handVel, true, true);
		// 	pHand = eventPos;
		// }
		i++;
	}

    int offset = 20;
    for (size_t y = offset; y < ofGetViewportHeight() - (offset * 2); y += yStride)
    {
        ofPolyline polyline;
        for (size_t x = offset; x < ofGetViewportWidth() - offset; x+= xStride)
        {
            float noise = ofNoise(x, y);
            ofSetLineWidth(noise * 2.0);
            ofVec2f point(x, y);
            ofVec2f mousePoint(mouseX, mouseY);
            float dist = point.distance(mousePoint);
            
            // noise *= dist * ofMap(sin(inc), -1, 1, 0, 1);
            // noise *= dist * (inc / 3.0);
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
    guiVisual.draw();
    guiAudio.draw();

    // ofSetColor(77, 131, 95);
    ofSetHexColor(0xD3CA97);
    ofNoFill();
    ofSetLineWidth(2);
    // ofDrawRectangle(offset, offset, ofGetScreenWidth() - (offset * 2), ofGetScreenHeight() - (offset * 2));
    title.drawString("PHANTASM AUDIO", ofGetViewportWidth()/2.0, ofGetViewportHeight()/2.0);

    // for (size_t i = 0; i < 5; i++)
    // {
    //     ofPolyline customCircle;
    //     for (size_t res = 0; res < 100; res++)
    //     {
    //         float x = 10 * sin(res);
    //         float y = 10 * cos(res);

    //         customCircle.addVertex(x, y);
    //     }
    // customCircle.draw();
    // }
    drawKinectSettings();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f')
        ofToggleFullscreen();

    switch(key)
    {
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
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::audioReceived(float* input, int bufferSize, int nChannels) {
  beat.audioReceived(input, bufferSize, nChannels);
}

void ofApp::drawKinectSettings()
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