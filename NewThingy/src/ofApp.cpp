#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::initKinect()
{
    //Initialize device
    kinect.setRegistration(true);
    kinect.init(true, true, true);
    kinect.open();
    kinect.setCameraTiltAngle(kinectSettings.angle);
    kinectSettings.stride = 16;
}


//--------------------------------------------------------------
void ofApp::initSquareGrid()
{
    kinectSettings.scaleX = ofGetScreenWidth() / (kinect.width / kinectSettings.stride);
    kinectSettings.scaleY = ofGetScreenHeight() / (kinect.height / kinectSettings.stride);
    int scaleX = kinectSettings.scaleX;
    int scaleY = kinectSettings.scaleY;
    size_t col = 0;
    size_t row = 0;

    for (size_t y = 0; y < kinect.height; y += kinectSettings.stride)
    {
        for (size_t x = 0; x < kinect.width; x += kinectSettings.stride)
        {
            Square square(ofVec3f(col * scaleX, row * scaleY, 0), scaleX * 0.95, scaleY * 0.95, scaleX * 0.95);
            squares.push_back(square);
            col++;
        }
        row++;
        col = 0;
    }
}

//--------------------------------------------------------------
void ofApp::setup(){

    // Initializing Objects
    initKinect();
    initSquareGrid();

    // OF settings
	ofBackgroundGradient(ofColor(10), ofColor(50));
    ofEnableDepthTest();

    // Lighting
    light.setPosition(200, 300, 400);
    light.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0)); // Set diffuse color
    ofEnableLighting();
    light.enable();

    // Aduio unit chain
    gate_ctrl.out_trig() >> env;
                            env >> amp.in_mod();
        
    pitch_ctrl >> osc.in_pitch();
                  osc >> amp * dB(-12.0f) >> engine.audio_out(0);
                         amp * dB(-12.0f) >> engine.audio_out(1);

    0.0f    >> env.in_attack();
    50.0f   >> env.in_decay();
    0.5f    >> env.in_sustain();
    500.0f  >> env.in_release();

    pitch_ctrl.set(72.0f); // we control the value of an pdsp::ValueControl directly with the set function
    
    // you can smooth out an pdsp::ValueControl changes, decomment this for less "grainy" pitch changes
    pitch_ctrl.enableSmoothing(50.0f); // 50ms smoothing


    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(5); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    // engine.setup( 44100, 512, 3); 
    engine.setup( 48000, 512, 3); 
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    if (kinect.isFrameNew())
    {
        size_t row = 0;
        size_t col = 0;
        for (size_t y = 0; y < kinect.height; y += kinectSettings.stride)
        {
            for (size_t x = 0; x < kinect.width; x += kinectSettings.stride)
            {
                size_t index = row * (kinect.width / kinectSettings.stride) + col;
                float dist  = ofMap(kinect.getDistanceAt(x, y), 0, 4095, 0, 255);
                if (index < squares.size())
                {
                    if (dist < kinectSettings.distThresh && dist != 0)
                    {
                        if (!squares[index].isOn())
                            squarePtrs.push_back(&squares[index]);
                        squares[index].grow(dist);
                    }
                    else if (dist != 0)
                    {
                        squares[index].shrink();
                    }
                }
                col++;
            }
            row++;
            col = 0;
        }
    }

    //If cube must shrink,  add it to array of references
    //After 5 frames, increment index;
    //Have index go through reference array, change color to white if _bON iss true
    //If _bOn is false, increment again and check again after 5 frames.
    if (squarePtrs.size() > 0 && highlightIndex < squarePtrs.size())
    {
        std::cout << "SquarePtrs size: " << squarePtrs.size() << std::endl;
        if (squarePtrs[highlightIndex]->isOn())
        {
            squarePtrs[highlightIndex]->setColor(ofColor(255));

            ofVec2f squarePos = squarePtrs[highlightIndex]->getPosition();

            octave = ofMap(squarePos.y, ofGetScreenHeight(), 0, -1, 3);
            pitch_ctrl.set(p + (octave * 12));
            gate_ctrl.trigger(0.8f);
        }
        else
        {
            squarePtrs.erase(squarePtrs.begin() + highlightIndex);
        }
    }
    if (ofGetFrameNum() % 20 == 0 && squarePtrs.size() != 0)
    {
        p = scale[(int)ofRandom(0, 7)]; // random note from scale
        highlightIndex++;
        if ((highlightIndex - 1) >= 0 && highlightIndex < squarePtrs.size())
        {
            squarePtrs[highlightIndex - 1]->resetColor();
            squarePtrs.erase(squarePtrs.begin() + highlightIndex - 1);
        }
        gate_ctrl.off();
    }
    if (highlightIndex > squarePtrs.size())
        highlightIndex = 0;
}

//--------------------------------------------------------------
void ofApp::draw(){

    cam.begin();
        ofTranslate(ofGetWidth() / 2.0, 0.0);
        ofScale(-1, -1, -1);
        light.setPosition(0, 0, 600);
        for (size_t i = 0; i < squares.size(); i++)
        {
            squares[i].draw();
        }
	ofSetColor(255);
    displayKinectSettings();
    cam.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key)
    {
        case 'f':
            ofToggleFullscreen();
            break;
        case '<':
        case ',':
            kinectSettings.farThresh--;
            if (kinectSettings.farThresh < 0)
                kinectSettings.farThresh = 0;
            break ;
        case '>':
        case '.':
            kinectSettings.farThresh++;
            if (kinectSettings.farThresh > 255)
                kinectSettings.farThresh = 255;
            break ;
        case '-':
            kinectSettings.nearThresh--;
            if (kinectSettings.nearThresh < 0)
                kinectSettings.nearThresh = 0;
            break ;
        case '+':
            kinectSettings.nearThresh++;
            if (kinectSettings.nearThresh > 255)
                kinectSettings.nearThresh = 255;
            break ;
		case OF_KEY_UP:
			kinectSettings.angle++;
			if(kinectSettings.angle>30) kinectSettings.angle=30;
			kinect.setCameraTiltAngle(kinectSettings.angle);
			break;
		case OF_KEY_DOWN:
			kinectSettings.angle--;
			if(kinectSettings.angle<-30) kinectSettings.angle=-30;
			kinect.setCameraTiltAngle(kinectSettings.angle);
			break;
        case OF_KEY_LEFT:
            kinectSettings.distThresh--;
            if (kinectSettings.distThresh < 0) kinectSettings.distThresh = 0;
            break;
        case OF_KEY_RIGHT:
            kinectSettings.distThresh++;
            if (kinectSettings.distThresh > 255) kinectSettings.distThresh = 255;
            break;
        case OF_KEY_ESC:
            kinect.close();
            squares.clear();
            ofExit();
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
void ofApp::displayKinectSettings()
{
	ofSetColor(255);
	stringstream reportStream;

	reportStream << "set near treshold " << kinectSettings.nearThresh << " (press: + -)" << endl
				 << "set far treshold " << kinectSettings.farThresh << " (press: < >)" << endl
				 << "set dist treshold " << kinectSettings.distThresh << " (press: <- ->)" << endl
				 << ", fps: " << ofGetFrameRate() << endl
				 << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
	if (kinect.hasCamTiltControl())
		reportStream << "press UP and DOWN to change the tilt angle: " << kinectSettings.angle << " degrees" << endl;

	ofDrawBitmapString(reportStream.str(), ofGetWidth() - 600, ofGetHeight() - 75);
}