#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::drawCross(ofVec2f pos, int w, int h){

    //Vertical
    ofSetColor(ofColor::blue);
    ofDrawRectangle(pos.x, pos.y, w - (w * 0.60), h);
    // //Horizontal
    // ofSetColor(ofColor::green);
    ofDrawRectangle(pos.x, pos.y, w, h - (h * 0.60));
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(245);
    nTiles = 10;
    tW = WIDTH / nTiles;
    // tH = HEIGHT / nTiles;
    tH = WIDTH / nTiles;


}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    for (size_t x = 0; x < nTiles; x++)
    {
        for (size_t y = 0; y < nTiles; y++)
        {
            ofPushMatrix();
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofTranslate(x * tW + tW/2, y * tH + tH/2);
            if (dist(mouseX, x) < 50 && dist(mouseY, y) < 50)
                ofRotate(ofGetFrameNum() * 0.3);
            drawCross(ofVec2f(0), tW, tH);
            ofPopMatrix();
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
