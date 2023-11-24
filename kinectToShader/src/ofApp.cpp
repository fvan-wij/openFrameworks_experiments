#include "ofApp.h"
#include <ostream>

//--------------------------------------------------------------
void ofApp::setup(){
	//Kinect
	ofSetLogLevel(OF_LOG_VERBOSE);
	kinect.setRegistration(true);
	kinect.init(false, false);
	kinect.open();
	grayImage.allocate(kinect.width, kinect.height);
	nearThreshold = 255;
	farThreshold = 218;
	ofSetFrameRate(60);
	angle = 0;
	kinect.setCameraTiltAngle(angle);

	//Plane
	shader.load("shader.vert", "shader.frag");
	// shader.load("shader2.vert", "shader.frag");
	float planeScale = 1.0;
	int planeWidth = WIDTH * planeScale;
	int planeHeight = HEIGHT * planeScale;
	int planeGridSize = 20;
	int planeColumns = planeWidth / planeGridSize;
	int planeRows = planeHeight / planeGridSize;
	plane.set(planeWidth, planeHeight, planeColumns, planeRows, OF_PRIMITIVE_TRIANGLES);

	// Initialize hand vector array
	nHands = 16;
	for (size_t i = 0; i < nHands; i++)
	{
		hands.push_back(ofVec2f(0.0, 0.0));
	}
}

//--------------------------------------------------------------
void ofApp::update(){
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
		contourFinder.findContours(grayImage, 10, (kinect.width * kinect.height) / 2, 20, false);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	//Kinect
	drawKinectSettings();
	ofVec2f center = ofVec2f(WIDTH / 2.0, HEIGHT / 2.0);
	ofVec2f mousePos = ofVec2f(mouseX - center.x, mouseY - center.y);
	float width = 0.0;
	float height = 0.0;
	float percentX = 0.0;
	for (size_t i = 0; i < contourFinder.nBlobs; i++)
	{
		ofRectangle r = contourFinder.blobs[i].boundingRect;
		contourFinder.blobs[i].draw(0, 0);
		width += r.width;
		height += r.height;
		if (i < nHands){
			hands[i].x = r.x - center.x + (width / 2);
			hands[i].y = r.y - center.y + (height / 2);
			percentX += hands[i].x;
		}
	}

	//Shader
	shader.begin();
	for (size_t i = 0; i < nHands; i++)
	{
		std::string uniformName = "hands[" + std::to_string(i) + "]";
		shader.setUniform2f(uniformName, hands[i].x, hands[i].y);
	}
	percentX = percentX / kinect.getWidth();
	percentX = ofClamp(percentX, 0, 1);

	ofColor colorLeft = ofColor::magenta;
	ofColor colorRight = ofColor::cyan;
	ofColor colorMix = colorLeft.getLerped(colorRight, percentX);

	ofSetColor(colorMix);

	shader.setUniform1i("nHands", nHands);
	shader.setUniform1f("range", width / 2);
	shader.setUniform1f("mouseX", mouseX / (float) WIDTH);
	shader.setUniform1f("mouseY", mouseY / (float) HEIGHT);
	shader.setUniform2f("mousePos", mousePos.x, mousePos.y); 
	shader.setUniform1f("time", ofGetElapsedTimef());
	ofTranslate((float) WIDTH / 2, (float) HEIGHT / 2);
	plane.drawWireframe();
	shader.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;
			
		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;
			
		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;
			
		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;
			
		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
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

	ofDrawBitmapString(reportStream.str(), 20, 652);
}
