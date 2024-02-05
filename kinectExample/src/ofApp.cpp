#include "ofApp.hpp"

//--------------------------------------------------------------
void ofApp::setup() {
	kinect.setRegistration(true);
	
	kinect.init();
	
	kinect.open();		// opens first available kinect
	
	nearThreshold = 230;
	farThreshold = 70;
	
	ofSetFrameRate(60);
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	w = 640;
	h = 480;
	is_initialized = false;
}

void ofApp::init_mesh()
{
	int		step = 2;
	kinect.update();

	grayImage.setFromPixels(kinect.getDepthPixels());
	ofPixels & pix = grayImage.getPixels();
	for(int x = 0; x < w; x+=step)
	{
		for (int y = 0; y < h; y+=step)
		{
			mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
			mesh.addColor(kinect.getColorAt(x, y));
		}
	}
	is_initialized = true;
}

//--------------------------------------------------------------
void ofApp::update() {
	ofBackground(100, 100, 100);
	if (!is_initialized)
		init_mesh();
	kinect.update();
	if(kinect.isFrameNew())
	{
		int		step = 2;
		int numVertices = mesh.getNumVertices();
		for(int x = 0; x < w; x+=step)
		{
			for (int y = 0; y < h; y+=step)
			{
				int index = x + y * w;
				if (index < numVertices)
				{
					if(kinect.getDistanceAt(x, y) < nearThreshold && kinect.getDistanceAt(x, y) > farThreshold)
					{
						mesh.setVertex(index, kinect.getWorldCoordinateAt(x, y));
						mesh.setColor(index, kinect.getColorAt(x, y));
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255, 255, 255);
	// grayImage.draw(10, 320, 400, 300);
	cam.begin();
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards' 
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
	mesh.drawVertices();
	ofDisableDepthTest();
	ofPopMatrix();
	cam.end();
	display_kinect_settings();
}

// void ofApp::drawPointCloud() {
// 	int w = 640;
// 	int h = 480;
// 	ofMesh mesh;
// 	// mesh.setMode(OF_PRIMITIVE_POINTS);
// 	mesh.setMode(OF_PRIMITIVE_LINES);
// 	int step = 2;
// 	for(int y = 0; y < h; y += step) {
// 		for(int x = 0; x < w; x += step) {
// 			if(grayImage.(x, y) > 0) {
// 				mesh.addColor(kinect.getColorAt(x,y));
// 				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
// 			}
// 		}
// 	}
// 	glPointSize(3);
// 	ofPushMatrix();
// 	// the projected points are 'upside down' and 'backwards' 
// 	ofScale(1, -1, -1);
// 	ofTranslate(0, 0, -1000); // center the points a bit
// 	ofEnableDepthTest();
// 	mesh.drawVertices();
// 	ofDisableDepthTest();
// 	ofPopMatrix();
// }

//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}

void ofApp::display_kinect_settings(void){

	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
		
	if(kinect.hasAccelControl()) {
		reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
		<< ofToString(kinect.getMksAccel().y, 2) << " / "
		<< ofToString(kinect.getMksAccel().z, 2) << endl;
	} else {
		reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
		<< "motor / led / accel controls are not currently supported" << endl << endl;
	}

	reportStream << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
	<< "set near threshold " << nearThreshold << " (press: + -)" << endl
	<< "set far threshold " << farThreshold << endl
	<< ", fps: " << ofGetFrameRate() << endl
	<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;

	if(kinect.hasCamTiltControl()) {
		reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
		<< "press 1-5 & 0 to change the led mode" << endl;
	}

	ofDrawBitmapString(reportStream.str(), 20, 652);
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
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
void ofApp::mouseDragged(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}
