#include "ofApp.hpp" 
#include "ofAppRunner.h"
#include <cstdint>

//--------------------------------------------------------------
void ofApp::setup(){

	//Window settings
	ofEnableAlphaBlending(); 
	ofSetBackgroundColor(10);
	toggle = false;

	// Audio input initialization
	int			bufferSize = 256;	
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);

	bufferCounter 	= 0;
	drawCounter 	= 0;
	smoothedVol 	= 0.0;
	scaledVol 		= 0.0;

	ofSoundStreamSettings settings;
	soundStream.printDeviceList();
	auto	devices = soundStream.getDeviceList();
	settings.setInDevice(devices[0]);
	settings.setInListener(this);
	settings.sampleRate = 48000;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 1;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);

	//Kinect initialization
	kinect.init(false, false);
	if (kinect.isConnected())
		kinect.close();

	kinect.open();
	kinect.setCameraTiltAngle(0.0);
	grayImage.allocate(kinect.width, kinect.height);
	nearTreshold = 230;
	farTreshold = 70;
	kinectWidth = kinect.getWidth();
	kinectHeight = kinect.getHeight();
	font.load("../bin/data/fonts/Bebas-Regular.ttf", TILES);
	float scalerX = (((float) WIDTH - kinectWidth) / kinectWidth) + 1;
	float scalerY = (((float) HEIGHT - kinectHeight) / kinectHeight) + 1;
	printf("kinectWidth: %d, kinectHeight: %d, WIDTH: %d, HEIGHT: %d\n", kinectWidth, kinectHeight, WIDTH, HEIGHT);
	int col;

	col = 0;
	kinectPoints.resize(HEIGHT / TILES);
	for (int i = 0; i < kinectWidth; i++)
	{
		for (int j = 0; j < kinectHeight; j++)
		{
			if (j % TILES == 0)
			{
				kinectPoint point (i * scalerX, j * scalerY, 2);
				kinectPoints[col].push_back(point);
			}
		}
		if (i % TILES == 0)
			col++;
	}
	ofResetElapsedTimeCounter();
}

//--------------------------------------------------------------
void ofApp::update(){
	//Audio timer
	// float vol = smoothedVol * 100.0f;
	// if (vol > 115.0f && ofGetElapsedTimef() > 3.0f)
	// 	ofResetElapsedTimeCounter();
	// if (ofGetElapsedTimef() < 3.0f)
	// 	updateMatrixRasterization(true);
	// else 
	// 	updateMatrixRasterization(false);

	//Audio toggle
	float vol = smoothedVol * 100.0f;
	if (vol > 115.0f && ofGetElapsedTimef() > 0.5)
	{
		if (toggle)
		{
			toggle = false;
			ofResetElapsedTimeCounter();
		}
		else
		{
			toggle = true;
			ofResetElapsedTimeCounter();
		}
	}
	updateMatrixRasterization(toggle);

	//Kinect
	// updateMatrixRasterization();
}

//--------------------------------------------------------------
void ofApp::draw(){
	//Audio
 
	//Kinect
	drawMatrixRasterization();
}

void	ofApp::keyPressed(int key)
{
	switch (key)
	{
		case 'i':
		case 'e':
		case 's':
			soundStream.stop();
			// move = false;
			break;
		case 'S':
			soundStream.start();
			// move = true;
			break;
		case '>':
			farTreshold++;
			break;
		case '<':
			farTreshold--;
			break;
		case '-':
			nearTreshold++;
			break;
		case '+':
			nearTreshold--;
			break;
		case 'f':
		case 'F':
			ofToggleFullscreen();
			break;
		default:
			break;
	}
}

//Audio---------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input)
{
	float	curVol 		= 0.0;
	int		numCounted 	= 0;
	for (size_t i = 0; i < input.getNumFrames(); i++)
	{
		left[i] = input[i * 2] * 0.5;
		right[i] = input[i * 2 + 1] * 0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted +=2;
	}
	curVol /= (float) numCounted;
	curVol  = sqrt(curVol);
	smoothedVol *= 0.93;
	smoothedVol += 0.07 + curVol;
	bufferCounter++;
}



//Kinect--------------------------------------------------------
void	ofApp::updateMatrixRasterization(bool move)
{
	kinect.update();

	int col;
	int row;

	col = 0;
	row = 0;
	if (kinect.isFrameNew())
	{
		grayImage.setFromPixels(kinect.getDepthPixels());
		ofPixels & pix = grayImage.getPixels();
		for (int i = 0; i < kinectWidth; i++)
		{
			for (int j = 0; j < kinectHeight; j++)
			{
				int arrayIndex = j * kinectWidth + i;
				if (j % TILES == 0)
				{
					kinectPoints[col][row].m_Color.g = pix[arrayIndex];
					if (pix[arrayIndex] < nearTreshold && pix[arrayIndex] > farTreshold)
						kinectPoints[col][row].m_Visible = true;
					else
						kinectPoints[col][row].m_Visible = false;
					kinectPoints[col][row].update(kinectWidth, kinectHeight, move);
					row++;
				}
			}
			row = 0;
			if (i % TILES == 0)
				col++;
		}
	}
}

void ofApp::drawMatrixRasterization()
{
	int col;
	int row;

	col = 0;
	row = 0;
	for (int i = 0; i < kinectWidth; i++)
	{
		for (int j = 0; j < kinectHeight; j++)
		{
			if (j % TILES == 0)
			{
				kinectPoints[col][row].draw(font);
				row++;
			}
		}
		row = 0;
		if (i % TILES == 0)
			col++;
	}
}

vector<particle *> ofApp::particleIsWithinCell(ofVec2f cell)
{
	vector<particle *> particlePointers;

	for (size_t i = 0; i < particles.size(); i++)
	{
		if (particles[i].m_Position.x > cell.x && particles[i].m_Position.x < (cell.x + tileSize.x) && particles[i].m_Position.y > cell.y && particles[i].m_Position.y < (cell.y + tileSize.y))
			particlePointers.push_back(&particles[i]);
	}
	return particlePointers;
}

//--------------------------------------------------------------
void	ofApp::checkCollision(vector<particle *> particlePointers)
{
	int n = particlePointers.size();
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (particlePointers[i]->intersects(*particlePointers[j]))
				resolveCollision(*particlePointers[i], *particlePointers[j]);
		}
	}
	particlePointers.clear();
}

//--------------------------------------------------------------
void ofApp::spacePartitionParticles()
{
	vector<particle *> particlePointers;

	for (int i = 0; i < TILES; i++)
	{
		for (int j = 0; j < TILES; j++)
		{
			particlePointers = particleIsWithinCell(cellVector[i][j]);
			if (particlePointers.size() > 0)
				checkCollision(particlePointers);
		}
	}
}

//--------------------------------------------------------------
void ofApp::resolveCollision(particle& p1, particle& p2)
{
		float relativeVelocityX = p2.m_Velocity.x - p1.m_Velocity.x;
		float relativeVelocityY = p2.m_Velocity.y - p1.m_Velocity.y;

		// Calculate the normal vector.
		float normalX = p2.m_Position.x - p1.m_Position.x;
		float normalY = p2.m_Position.y - p1.m_Position.y;
		float normalLength = sqrt(normalX * normalX + normalY * normalY);
		normalX /= normalLength;
		normalY /= normalLength;

		// Calculate the relative velocity along the normal.
		float relativeVelocityAlongNormal = relativeVelocityX * normalX + relativeVelocityY * normalY;

		// Calculate the impulse (change in velocity) along the normal.
		float impulse = (2.0f * relativeVelocityAlongNormal) / (p1.m_Mass + p2.m_Mass);

		// Update the velocities of the particles.
		p1.m_Velocity.x += impulse * p2.m_Mass * normalX;
		p1.m_Velocity.y += impulse * p2.m_Mass * normalY;
		p2.m_Velocity.x -= impulse * p1.m_Mass * normalX;
		p2.m_Velocity.y -= impulse * p1.m_Mass * normalY;
}

void ofApp::exit()
{
	kinect.setCameraTiltAngle(0.0);
	kinect.close();
}


//--------------------------------------------------------------
void ofApp::drawContour()
{
	for (size_t x = 0; x < kinectPoints.size(); x++) {
		for (size_t y = 0; y < kinectPoints[x].size(); y++) {
			float distance = kinect.getDistanceAt(x, y);
			if (distance > nearTreshold && distance < farTreshold)
				kinectPoints[x][y].draw(font);
		}
	}
}

