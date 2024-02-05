#include "ofApp.h"
#include "ofGraphics.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// int width = 320;
	// int height = 240;
	cam.setup(320, 240);
	cam.setDesiredFrameRate(30);
	// p = cam.getPixels();
	ofSetBackgroundColor(0);
	// tile.x = widt  / 16;
	// tile.y = height / 16;
}

//--------------------------------------------------------------
void ofApp::update(){
	cam.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	// cam.draw(0, 0);
	p = cam.getPixels();
	for (int i = 0; i < p.getWidth(); i += 3)
	{
		for (int j = 0; j < p.getHeight(); j += 3)
		{
			ofColor color = cam.getPixels().getColor(i, j);	
			float brightness = color.getBrightness();
			float radius = ofMap(brightness, 0, 255, 0, 8);
			if (brightness < 100)
				ofDrawLine(i * 4, j * 4, i * 4 + ofRandom(-5, 5), j * 4 + ofRandom(-5, 5));	
			else
				ofDrawCircle(i * 4, j * 4, radius);

			ofSetColor(255);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
