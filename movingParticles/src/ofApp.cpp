#include "ofApp.h"
#include "ofEvents.h"

//--------------------------------------------------------------
void ofApp::setup(){
	myCam.setup(640, 480);	
	ofColor particleColor(255, 0, 0);
	camPixelData = myCam.getPixels();

	for (int i = 0; i < camPixelData.getWidth(); i+=16)
	{
		for (int j = 0; j < camPixelData.getHeight(); j+=16)
		{
			particle newParticle(i, j, particleColor);
			particles.push_back(newParticle);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	myCam.update();

	if (myCam.isFrameNew())
	{
		camPixelData = myCam.getPixels();
		int index = 0;
		for (int i = 0; i < camPixelData.getWidth(); i+=16)
		{
			for (int j = 0; j < camPixelData.getHeight(); j+=16)
			{
				if (particles[index].size <= 4)
					particles[index].update();
				index++;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
		int index = 0;

		for (int i = 0; i < camPixelData.getWidth(); i+=16)
		{
			for (int j = 0; j < camPixelData.getHeight(); j+=16)
			{
				ofColor sample = camPixelData.getColor(i, j);
				float brightness = sample.getBrightness();
				float mappedSize = ofMap(brightness, 0, 255, 0, 8);
				particles[index].size = mappedSize;
				particles[index].color = sample;
				particles[index].draw();
				index++;
			}
		}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
		case 'f':
		case 'F':
			ofToggleFullscreen();
			break;

		default:
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
particle::particle(int startX, int startY, ofColor color)
{
	position = ofVec2f(startX, startY);
	direction = ofVec2f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0));
	idle = ofVec2f(0, 0);
	size = 20;
}

particle::~particle()
{

}

void particle::update()
{
	// if (G_STATE == IDLE)
	// 	position += ofVec2f(0, 0);
	// else if (G_STATE == DRIFT)
	// 	position += direction;	
	position += direction;	
}

void particle::draw()
{
	ofSetColor(color);
	ofDrawCircle(position, size);
}

