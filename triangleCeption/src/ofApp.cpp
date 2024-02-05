#include "ofApp.h"

void ofApp::initKinectSettings()
{
	//Kinect
	ofSetLogLevel(OF_LOG_VERBOSE);
	kinect.setRegistration(true);
	kinect.init(false, false);
	kinect.open();
	grayImage.allocate(kinect.width, kinect.height);
	ofSetFrameRate(60);
	kinect.setCameraTiltAngle(angle);
}

void ofApp::findKinectContours()
{
	kinect.update();

	if (kinect.isFrameNew())
	{
		grayImage.setFromPixels(kinect.getDepthPixels());
		ofPixels & pix 			= grayImage.getPixels();
		int 	   numPixels 	= pix.size();
		for (size_t i = 0; i < numPixels; i++)
		{
			if (pix[i] < uiNearThresh && pix[i] > uiFarThresh)
				pix[i] = 255;
			else 
				pix[i] = 0;
		}
		grayImage.flagImageChanged();
		grayImage.mirror(false, true);
		contourFinder.findContours(grayImage, 10, (kinect.width * kinect.height) / 2, 20, false);
	}
}

void ofApp::drawKinectContours()
{
	for (size_t i = 0; i < contourFinder.nBlobs; i++)
	{
		int width = 0;
		int height = 0;
		ofRectangle r = contourFinder.blobs[i].boundingRect;
		contourFinder.blobs[i].draw(0, 0);
		width = r.width;
		height = r.height;
	}
}

ofApp::Node* ofApp::addLast(Node *head, Node *newNode)
{
	Node *current;

	current = head;
	while (current->next != NULL)
		current = current->next;
	current->next = newNode;
	newNode->next = NULL;
	return (head);
}


ofApp::Node* ofApp::addTriangleNode(Node *head)
{
	Node *newNode = NULL;

	if (contourFinder.nBlobs >= 3)
	{
		newNode = new Node();
		std::vector <ofVec3f> points;
		for (size_t i = 0; i < 3; i++)
		{
			ofRectangle r = contourFinder.blobs[i].boundingRect;
			points.push_back(ofVec3f(r.x + (r.width * 0.5), r.y + (r.height * 0.5), 0.0));
		}
		newNode->p = points;
		newNode->next = NULL;
		if (!head)
			return (newNode);
		else
			head = addLast(head, newNode);
		return (head);
	}
	else 
		return (NULL);
}

void ofApp::initGui()
{
	gui.setup();
	gui.add(uiAmount.set("amount", 20, 1, 30));
	gui.add(uiPower.set("power", ofVec3f(0.0, 0.0, 0.5), ofVec3f(0), ofVec3f(3.0)));
	gui.add(uiRadius.set("radius", 0.05, -1.0, 1.0));
	gui.add(uiPosition.set("position",
							ofVec2f(0),
							ofVec2f(-ofGetWidth(), -ofGetHeight()),
							ofVec2f(ofGetWidth(), ofGetHeight())));
	gui.add(uiNearThresh.set("near treshold", 255, 0, 255));
	gui.add(uiFarThresh.set("far treshold", 230, 0, 255));
	gui.add(uiTrianCol.set("triangle color", ofColor(0), ofColor(0), ofColor(255)));
	gui.add(uiBgCol.set("background color", ofColor(30, 30, 30), ofColor(0), ofColor(255)));
}

//--------------------------------------------------------------
void ofApp::setup(){
	// ofBackground(30, 30, 30);
	ofSetBackgroundAuto(false);

	//Kinect
	initGui();
	initKinectSettings();
}

//--------------------------------------------------------------
void ofApp::update(){
	findKinectContours();
	triangleList = addTriangleNode(triangleList);
}

void ofApp::drawTriangleCeption(ofVec2f p1, ofVec2f p2, ofVec2f p3){
	ofColor c = uiTrianCol;
	ofSetColor(c);
	// ofNoFill();
	ofPushMatrix();
	for (size_t i = 0; i < uiAmount; i++)
	{
		c.r += i * 1.1;
		c.g += i * 1.2;
		c.b += i * 1.5;
		ofSetColor(c);
		float noisex = ofNoise((ofGetElapsedTimef() * i) * uiPower->x);
		float noisey = ofNoise((ofGetElapsedTimef() * i) * uiPower->y);
		float noisez = sin((ofGetElapsedTimef() * 0.5) * i) * uiPower->z;
		float x = ofMap(noisex, 0, 1.0, -10.0, 10.0);
		float y = ofMap(noisey, 0, 1.0, -10.0, 10.0);
		float z = ofMap(noisez, -1.0, 1.0, -10.0, 10.0);
		ofTranslate(x, y, z);
		float dist = uiRadius;
		ofVec2f centroid((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3);
		ofVec2f inwardDir1 = centroid - p1;
		ofVec2f inwardDir2 = centroid - p2;
		ofVec2f inwardDir3 = centroid - p3;
		ofDrawTriangle(p1 + (inwardDir1 * dist) * i, p2 + (inwardDir2 * dist) * i, p3 + (inwardDir3 * dist) * i);
	}
	ofPopMatrix();
}

void ofApp::deleteTriangleList(Node **head)
{
	if (triangleList)
	{
		Node *delNode;
		while (triangleList != NULL)
		{
			delNode = triangleList;
			triangleList->p.clear();
			triangleList = triangleList->next;
			delete(delNode);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	// ofSetColor(30, 30, 30, 5);
	// ofDrawRectangle(0, 0, 1024, 768);
	Node *currentTriangle;
	ofBackground(uiBgCol);

	drawKinectContours();

	if (triangleList)
	{
		currentTriangle = triangleList;
		while (currentTriangle != NULL)
		{
			drawTriangleCeption(currentTriangle->p[0], currentTriangle->p[1], currentTriangle->p[2]);
			currentTriangle = currentTriangle->next;
		}
	}
	deleteTriangleList(&triangleList);

	if (bHideGui)
		gui.draw();
	drawKinectSettings();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case 'h':
			bHideGui = !bHideGui;
			break;
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;
			
		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;
			
		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;
			
		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;
			
		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
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
void ofApp::drawKinectSettings()
{
	ofSetColor(255);
	stringstream reportStream;

	reportStream << ", fps: " << ofGetFrameRate() << endl
				 << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
	if (kinect.hasCamTiltControl())
		reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl;

	ofDrawBitmapString(reportStream.str(), 20, 652);
}
