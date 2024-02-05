#pragma once

#include "ofMain.h"
#include "particle.hpp"
#include <vector>

//Initialize globals
#define WIDTH 1024
#define HEIGHT 768
#define TILES 20
#define PARTICLES 2000
#define STRIDE 16

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void resolveCollision(particle& p1, particle& p2);
		void spacePartitionParticles();
		vector<particle *> particleIsWithinCell(ofVec2f cell);
		void checkCollision(vector<particle *> particlePointers);

		ofVec2f tileSize;
		vector<particle> particles;
		vector<vector<ofVec2f>> cellVector;
};
