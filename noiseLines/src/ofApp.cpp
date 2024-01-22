#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    guiVisual.setup();
    guiVisual.add(xStride.setup("xStride", 4, 1, 64));
    guiVisual.add(yStride.setup("yStride", 4, 1, 64));
    guiVisual.add(drawLine.setup("drawLine", true));
    guiVisual.add(drawShape.setup("drawShape", false));
    guiVisual.add(drawCircle.setup("drawCircle", false));
    guiVisual.add(fullScreen.setup("fullscreen", false));
    guiVisual.add(noiseMult.setup("noiseMultiplier", 0.01, -1.0, 1.0));

    int fontsize 	= 80;
    int subsize 	= 30;
    title.loadFont("fonts/Morelife.otf", fontsize);
    subtitle.loadFont("fonts/WaterResistant.ttf", subsize);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
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

				float dist = point.distance(mousePoint);
				noise *= dist;
				polyline.addVertex(x, y + noise * noiseMult);

				if (!drawLine)	{
					if (x % 32 == 0) {
						ofPushMatrix();
						ofSetRectMode(OF_RECTMODE_CENTER);
						ofNoFill();
						ofTranslate(x + noise * noiseMult, y + noise);
						ofRotateDeg(mouseX);
						if (drawCircle) {
							ofDrawCircle(0, 0, noise * noiseMult);
						}
						if (drawShape) {
							ofDrawRectangle(0, 0, noise * .1, noise * .1);
						}
						ofPopMatrix();
					}
				}
			}
			if (drawLine) {
				polyline.draw();
			}
		}
		ofSetColor(COL_PHANTASM_SECONDARY);
		ofNoFill();
		ofSetLineWidth(5);
		// title.drawString("Creative Coding", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0);
		// subtitle.drawString("6 February, 13:00", ofGetViewportWidth() / 2.0, ofGetViewportHeight() / 2.0 + 40);
		if (bGui) {
			guiVisual.draw();
		}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'g')
		bGui = !bGui;
	if (key == 'l')
		bDrawLine = !bDrawLine;
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
