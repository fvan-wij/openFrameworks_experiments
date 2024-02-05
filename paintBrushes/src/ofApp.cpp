#include "ofApp.h"
#include "ofColor.h"
#include "ofEvents.h"
#include "ofGraphics.h"
#include "ofVec2f.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundAuto(false);
	ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofVec2f p1(0, 12.5);
	ofVec2f p2(50, 0);
	ofVec2f p3(0, -12.5);
	ofSetColor(0, 0, 200, 50);
	int minOffset = 5;
	int maxOffset = 70;

		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT))
		{
			float offsetDistance = ofRandom(minOffset, maxOffset);
			ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());
			float rotation = ofRandom(360);
			p1.rotate(rotation);
			p2.rotate(rotation);
			p3.rotate(rotation);

			ofVec2f triangleOffset(offsetDistance, 0.0);
			triangleOffset.rotate(rotation);
			p1 += mousePos + triangleOffset;
			p2 += mousePos + triangleOffset;
			p3 += mousePos + triangleOffset;
			ofColor blue(0, 100, 200, 150);
			ofColor purple(200, 0, 205, 150);
			ofColor lerpColor = blue.getLerped(purple, ofRandom(1.0));
			ofSetColor(lerpColor);
			ofDrawTriangle(p1, p2, p3);
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
