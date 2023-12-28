#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(77, 131, 95);
    ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);
    inc = 0;
    gui.setup();
    gui.add(xStride.setup("xStride", 4, 1, 64));
    gui.add(yStride.setup("yStride", 4, 1, 64));
    gui.add(drawLine.setup("drawLine", true));
    gui.add(drawShape.setup("drawShape", false));
}

//--------------------------------------------------------------
void ofApp::update(){
    beat.update(ofGetElapsedTimeMillis());
    inc += beat.kick() / 2.0;
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofEnableAlphaBlending();
    ofSetHexColor(0xD3CA97);
    float mouseX = ofGetMouseX();
    float mouseY = ofGetMouseY();

    for (size_t y = 0; y < 768; y += yStride)
    {
        ofPolyline polyline;
        // ofSetLineWidth(ofRandom(3));
        ofSetLineWidth(2);
        for (size_t x = 0; x < 1024; x+= xStride)
        {
            float noise = ofNoise(x, y);
            ofVec2f point(x, y);
            ofVec2f mousePoint(mouseX, mouseY);
            float dist = point.distance(mousePoint);
            
            noise *= dist * ofMap(sin(inc), -1, 1, 0, 1);
            polyline.addVertex(x, y + noise * 0.5);
            if (drawShape)
            {
                if (x % 32 == 0)
                {
                    ofPushMatrix();
                    ofSetRectMode(OF_RECTMODE_CENTER);
                    ofNoFill();
                    ofTranslate(x + noise * .5, y + noise);
                    ofRotate(mouseX);
                    ofDrawCircle(0, 0, noise * .1);
                    // ofDrawRectangle(0, 0, noise * .1, noise * .1);
                    // ofDrawRectangle(0, 0, noise * .1, noise * .1);
                    ofPopMatrix();
                }
            }
        }
        if (drawLine)
        {
            polyline.draw();
        }
    }
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

void ofApp::audioReceived(float* input, int bufferSize, int nChannels) {
  beat.audioReceived(input, bufferSize, nChannels);
}
