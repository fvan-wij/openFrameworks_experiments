#include "ofApp.h"
#include "ofEvents.h"
#include "ofGraphics.h"
#include "ofMath.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
	cam.setup(640, 480);
	cam.initGrabber(640, 480);
	videoPixels.allocate(640, 480, OF_PIXELS_RGB);
	videoTexture.allocate(640, 480, GL_RGB);
}

//--------------------------------------------------------------
void ofApp::update(){
	cam.update();

	if (cam.isFrameNew())
		videoPixels = cam.getPixels();
}

//--------------------------------------------------------------
void ofApp::rasterizeCamera()
{
	for (int i = 0; i < cam.getWidth(); i+=16) {
		for (int j = 0; j < cam.getHeight(); j+=16) {
			ofColor c = cam.getPixels().getColor(i, j);
			float brightness = c.getBrightness();
			float size = ofMap(brightness, 0, 255, 0, 16);
			ofDrawRectangle(i, j, size, size);
		}
	}
}

//--------------------------------------------------------------
void ofApp::colourizeCameraPixels(int r, int g, int b)
{
	ofColor c(r, g, b);
	for (int i = 0; i < cam.getWidth(); i++) {
		for (int j = 0; j < cam.getHeight(); j++) {
			ofColor sample = videoPixels.getColor(i, j);
			float brightness = sample.getBrightness();
			c.r = brightness;
			videoPixels.setColor(i, j, c);
			}
		}
	
	videoTexture.loadData(videoPixels);
	videoTexture.draw(0, 0);
}

void ofApp::glitchPixels()
{
	for (int i = 0; i < cam.getWidth(); i++) {
		for (int j = 0; j < cam.getHeight(); j++) {
			ofColor sample = videoPixels.getColor(i, j);
			float brightness = sample.getBrightness();
			ofColor inbetween = sample.getLerped(i, brightness);
			videoPixels.setColor(i, j, inbetween);
			}
		}
	
	videoTexture.loadData(videoPixels);
	videoTexture.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::draw(){
	// cam.draw(0, 0);
	// pixels = cam.getPixels();
	if (state == RASTERIZE)
		rasterizeCamera();
	else if (state == COLOURIZE)
		colourizeCameraPixels(255, 0, 0);
	else if (state == GLITCH)
		glitchPixels();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_LEFT)
		state = RASTERIZE;
	else if (key == OF_KEY_RIGHT)
		state = COLOURIZE;
	else if (key == OF_KEY_UP)
		state = GLITCH;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == OF_KEY_LEFT || key == OF_KEY_RIGHT || key == OF_KEY_UP)
		state = OFF;
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
