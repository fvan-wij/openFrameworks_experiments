#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::initKinectData()
{
    //Initialize device
    kinectData.device.setRegistration(true);
    kinectData.device.init(true, true, false);
    kinectData.device.open();
    kinectData.device.setCameraTiltAngle(0);

    //Initialize integers
    kinectData.angle        = 0;
    kinectData.farThresh    = 255;
    kinectData.nearThresh   = 200;
    kinectData.width        = kinectData.device.width;
    kinectData.height       = kinectData.device.height;

    //ALlocate grayImage
    kinectData.grayImage.allocate(kinectData.width, kinectData.height);

}

//--------------------------------------------------------------
void ofApp::setup(){
    initKinectData();

    //Init globals;
    stride = 8;
}

//--------------------------------------------------------------
void ofApp::updateKinectGrayImage()
{
    kinectData.device.update();
    if (kinectData.device.isFrameNew())
    {
        kinectData.grayImage.setFromPixels(kinectData.device.getDepthPixels());
        ofPixels &pix = kinectData.grayImage.getPixels();
        int numPixels = pix.size();
        for (int i = 0; i < numPixels; i++)
        {
            if (pix[i] > kinectData.nearThresh && pix[i] < kinectData.farThresh)
            {
                pix[i] = 255;
            }
            else
            {
                pix[i] = 0;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    updateKinectGrayImage();
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    // drawKinectBoxCloud(8);
    kinectData.grayImage.draw(0, 0);
    cam.end();
    displayKinectData();
}

void ofApp::drawKinectBoxCloud()
{
    int xStep = 1024 / (kinectData.width / stride);
    int yStep = 768 / (kinectData.height / stride);
    ofPoint pos(-ofGetWidth() / 2, -ofGetHeight() / 2, 0);
    int w;
    int h;
    ofScale(1, -1, -1);
    for (size_t y = 0; y < kinectData.height; y+=stride)
    {
        w = 0;
        h = 0;
        for (size_t x = 0; x < kinectData.width; x+=stride)
        {
            ofPushMatrix();
                ofPoint pix = kinectData.device.getWorldCoordinateAt(x, y);
                float depth = ofMap(pix.z, 0, 4095, 0, 255);
                ofSetColor(kinectData.device.getColorAt(x, y));
                if (x != 0)
                {
                    w = x / stride;
                }
                if (y != 0)
                {
                    h = y / stride;
                }
                ofTranslate(xStep * w, yStep * h, pix.z - 1000);
                if (depth < kinectData.farThresh && depth > kinectData.nearThresh)
                {
                    ofDrawBox(pos, xStep, yStep, xStep);
                }
            ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key)
    {
        case '<':
        case ',':
            kinectData.farThresh--;
            if (kinectData.farThresh < 0)
                kinectData.farThresh = 0;
            break ;
        case '>':
        case '.':
            kinectData.farThresh++;
            if (kinectData.farThresh > 255)
                kinectData.farThresh = 255;
            break ;
        case '-':
            kinectData.nearThresh--;
            if (kinectData.nearThresh < 0)
                kinectData.nearThresh = 0;
            break ;
        case '+':
            kinectData.nearThresh++;
            if (kinectData.nearThresh > 255)
                kinectData.nearThresh = 255;
            break ;
		case OF_KEY_UP:
			kinectData.angle++;
			if(kinectData.angle>30) kinectData.angle=30;
			kinectData.device.setCameraTiltAngle(kinectData.angle);
			break;
		case OF_KEY_DOWN:
			kinectData.angle--;
			if(kinectData.angle<-30) kinectData.angle=-30;
			kinectData.device.setCameraTiltAngle(kinectData.angle);
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
void ofApp::displayKinectData()
{
	ofSetColor(255);
	stringstream reportStream;

	reportStream << "set near treshold " << kinectData.nearThresh << " (press: + -)" << endl
				 << "set far treshold " << kinectData.farThresh << " (press: < >)" << endl
				 << ", fps: " << ofGetFrameRate() << endl
				 << "press c to close the connection and o to open it again, connection is: " << kinectData.device.isConnected() << endl;
	if (kinectData.device.hasCamTiltControl())
		reportStream << "press UP and DOWN to change the tilt angle: " << kinectData.angle << " degrees" << endl;

	ofDrawBitmapString(reportStream.str(), ofGetWidth() - 600, ofGetHeight() - 75);
}