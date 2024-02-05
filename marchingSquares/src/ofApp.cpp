#include "ofApp.hpp"
#include "ofEvents.h"
#include "ofGraphics.h"
#include "ofUtils.h"
#include <ostream>

//--------------------------------------------------------------
void ofApp::initializeFieldVector()
{
	kinect.update();
	RES = 5;
	COLS = kinect.getWidth() / RES + 1;
	ROWS = kinect.getHeight() / RES + 1;
	ZOFFSET = 0;
	INC = 0.1;
	FIELD.resize(COLS);
	int colIndex;

	colIndex = 0;
	for (int i = 0; i < kinect.getWidth(); i++)
	{
		for (int j = 0; j < kinect.getHeight(); j++)
		{
			if (j % RES == 0)
				FIELD[colIndex].push_back(0);
		}
		if (i % RES == 0)
			colIndex++;
	}
}
//--------------------------------------------------------------
void ofApp::setup(){

	kinect.setRegistration(true);
	kinect.init();
	kinect.open();
	kinect.setCameraTiltAngle(0.0);
	grayImage.allocate(kinect.width, kinect.height);
	nearTreshold = 230;
	farTreshold = 70;
	initializeFieldVector();
	ofSetBackgroundAuto(false);
	ofBackground(150, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();

	if (kinect.isFrameNew())
	{
		grayImage.setFromPixels(kinect.getDepthPixels());
		ofPixels & pix = grayImage.getPixels();
		int colIndex;
		int rowIndex;

		colIndex = 0;
		rowIndex = 0;
		XOFFSET = 0;
		for (int i = 0; i < kinect.getWidth(); i++)
		{
			XOFFSET += INC;
			for (int j = 0; j < kinect.getHeight(); j++)
			{
				int arrayIndex = j * kinect.getWidth() + i;
				// float val = ofSignedNoise(XOFFSET, YOFFSET, ZOFFSET);
				if (pix[arrayIndex] < nearTreshold && pix[arrayIndex] > farTreshold)
				{
				float val = ofSignedNoise(XOFFSET, YOFFSET, ZOFFSET);
					if (colIndex < FIELD.size() && rowIndex < FIELD[colIndex].size())
						FIELD[colIndex][rowIndex] = 1.0 * val;
					pix[arrayIndex] = 255;
				}
				else
				{
					float val = ofSignedNoise(XOFFSET, YOFFSET, ZOFFSET);
					if (colIndex < FIELD.size() && rowIndex < FIELD[colIndex].size())
						FIELD[colIndex][rowIndex] = 0.0 * val;
					pix[arrayIndex] = 0;
				}
				if (j % RES == 0)
					rowIndex++;
				YOFFSET += INC;
			}
			YOFFSET = 0;
			rowIndex = 0;
			if (i % RES == 0)
				colIndex++;
		}
		// grayImage.flagImageChanged();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(150, 4);
	marchingSquares();
	displayReport();
	ZOFFSET += 0.01;
}

//--------------------------------------------------------------
int	binaryToNumber(int b8, int b4, int b2, int b1)
{
	return (b8 * 8 + b4 * 4 + b2 * 2 + b1 * 1);
}

//--------------------------------------------------------------
void ofApp::marchingSquares()
{
	int state;

	state = 0;
	for (int i = 0; i < COLS; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			if (i + 1 < COLS && j + 1 < ROWS && i < FIELD.size() && j < FIELD[i].size())
			{
				// ofSetColor(FIELD[i][j] * 255);
				// ofSetColor(FIELD[i][j]);
				// ofDrawRectangle(i * RES, j * RES, RES, RES);
				state = binaryToNumber(ceil(FIELD[i][j]), ceil(FIELD[i + 1][j]), ceil(FIELD[i + 1][j + 1]), ceil(FIELD[i][j + 1]));
				ofVec2f a((i * RES + RES * 0.5) * widthMultiplier, (j * RES) * heightMultiplier);
				ofVec2f b((i * RES + RES) * widthMultiplier, (j * RES + RES * 0.5) * heightMultiplier);
				ofVec2f c((i * RES + RES * 0.5) * widthMultiplier, (j * RES + RES) * heightMultiplier);
				ofVec2f d((i * RES) * widthMultiplier, (j * RES + RES * 0.5) * heightMultiplier);
				ofSetColor(FIELD[i][j] * 510, FIELD[i][j] * 510 * 0.5, FIELD[i][j] * 510);
				ofSetColor(255);
				if (state == 1)
					ofDrawLine(c, d);
				else if (state == 2)
					ofDrawLine(b, c);
				else if (state == 3)
					ofDrawLine(b, d);
				else if (state == 4)
					ofDrawLine(a, b);
				else if (state == 5)
				{
					ofDrawLine(a, d);
					ofDrawLine(b, c);

				}
				else if (state == 6)
					ofDrawLine(a, c);
				else if (state == 7)
					ofDrawLine(a, d);
				else if (state == 8)
					ofDrawLine(a, d);
				else if (state == 9)
					ofDrawLine(a, c);
				else if (state == 10)
				{
					ofDrawLine(a, b);
					ofDrawLine(c, d);

				}
				else if (state == 11)
					ofDrawLine(a, b);
				else if (state == 12)
					ofDrawLine(b, d);
				else if (state == 13)
					ofDrawLine(b, c);
				else if (state == 14)
					ofDrawLine(c, d);
			}
		}
	}
}

void ofApp::displayReport()
{
	stringstream reportStream;

	reportStream << "NEAR TRESHOLD: " << nearTreshold << " (press: + -)" << endl
		<< "FAR TRESHOLD: " << farTreshold << " (press: < >)" << endl
		<< "FPS:  " << ofGetFrameRate() << endl
		<< "RES: " << RES << endl
		<< "COLS: " << COLS << endl
		<< "ROWS: " << ROWS << endl
		<< "WIDTH: " << kinect.getWidth() << endl
		<< "HEIGHT: " << kinect.getHeight() << endl;
	ofSetColor(0);
	ofDrawRectangle(0, HEIGHT * 0.80, 290, 300);
	ofSetColor(255);
	ofDrawBitmapString(reportStream.str(), 20, HEIGHT * 0.85);
}

void ofApp::exit()
{
	kinect.setCameraTiltAngle(0.0);
	kinect.close();
	FIELD.clear();
}

void ofApp::keyPressed(int key)
{
	switch (key)
	{
		case '>':
		case '.':
			farTreshold ++;
			if (farTreshold > 255) farTreshold = 255;
			break;

		case '<':
		case ',':
			farTreshold --;
			if (farTreshold < 0) farTreshold = 0;
			break;

		case '+':
		case '=':
			nearTreshold ++;
			if (nearTreshold > 255) nearTreshold = 255;
			break;

		case '-':
			nearTreshold --;
			if (nearTreshold < 0) nearTreshold = 0;
			break;
		case 'e':
			exit();
	}
}
