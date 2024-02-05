#include "ofApp.h"
#include "ofGraphics.h"
#include "ofGraphicsConstants.h"

//--------------------------------------------------------------
void ofApp::setup(){
	camWidth = 640;
	camHeight = 480;
	screenWidth = 1280;
	screenHeight = 960;
	tileResolution = 100;
	tileSize.x =  (int) (camWidth / tileResolution);
	tileSize.y = (int) (camHeight / tileResolution);
	myWebcam.setup(camWidth, camHeight);
	newBox.set(tileSize.x * 0.5, tileSize.y * 0.5, tileSize.x * 0.5);
	newBox.setPosition(0, 0, 0);
	// newBox.setMode(OF_PRIMITIVE_POINTS);
}

//--------------------------------------------------------------
void ofApp::update(){
	myWebcam.update();	
	// ofColor c(255, 0, 0);
	if (myWebcam.isFrameNew())
	{
		camPixelData = myWebcam.getPixels();	
		// int totalBytes = camWidth * camHeight * 3;
		// for (int i = 0; i < camPixelData.getWidth(); i++)
		// {
		// 	for (int j = 0; j < camPixelData.getHeight(); j++)
		// 	{
		// 		ofColor sample = camPixelData.getColor(i, j);
		// 		float brightness = sample.getBrightness();
		// 		float size = ofMap(brightness, 0, 255, 0, 16);
		// 		camPixelData.setColor(i, j, sample);
		// 	}
		// }
	}
	// myCamTexture.loadData(camPixelData);
}

//--------------------------------------------------------------
void ofApp::draw(){
	// myCamTexture.draw(0, 0);

	cam.begin();
		for (int i = 0; i < camPixelData.getWidth(); i+= tileSize.x)
		{
			for (int j = 0; j < camPixelData.getHeight(); j+= tileSize.y)
			{
				ofColor sample = camPixelData.getColor(i, j);
				float brightness = sample.getBrightness();
				float size = ofMap(brightness, 0, 255, 0, 20);
				// camPixelData.setColor(i, j, sample);
				// ofDrawCircle(i, j, size);
				newBox.setDepth(size);
				newBox.setPosition(i, j, brightness);
				ofColor depthColor(brightness, brightness, brightness);
				for (int side = 0; side < 6; side++)
					newBox.setSideColor(side, depthColor);
				newBox.draw();	
			}
		}
	cam.end();
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
