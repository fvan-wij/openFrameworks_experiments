#include "ofApp.hpp" 

//--------------------------------------------------------------
void ofApp::setup(){
	tileSize.x = (float) WIDTH / TILES;
	tileSize.y = (float) HEIGHT / TILES;

	for (size_t i = 0; i < PARTICLES; i++)
	{
		ofVec2f p(ofRandom(WIDTH), ofRandom(HEIGHT));
		particle point(p, ofRandom(2, 5));
		particles.push_back(point);
	}

	cellVector.resize(TILES);
	for (int x = 0; x < TILES; x++)
	{
		for (int y = 0; y < TILES; y++)
		{
			ofVec2f cell(x * tileSize.x, y * tileSize.y);
			cellVector[y].push_back(cell);
		}
	}
	std::cout << "Size of cellDimensions: " << cellVector.size() << endl;
	std::cout << "Size of tile.x: " << tileSize.x << endl;
	std::cout << "Size of tile.y: " << tileSize.y << endl;
	ofEnableAlphaBlending(); 
	ofSetBackgroundColor(255);
}

//--------------------------------------------------------------
void ofApp::update(){
	float dt;

	dt = ofGetLastFrameTime();
	spacePartitionParticles();
	for (size_t i = 0; i < particles.size(); i++)
		particles[i].update(dt);
}

//--------------------------------------------------------------
void ofApp::draw(){
	for (size_t i = 0; i < particles.size(); i++)
			particles[i].draw();
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


