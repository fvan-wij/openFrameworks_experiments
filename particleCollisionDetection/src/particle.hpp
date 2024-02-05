#pragma once

#include "ofMain.h"

class particle {
public:

	//Particle position
	ofVec2f m_Position, m_InitPosition;

	//Particle dynamics
	ofVec2f m_Velocity, m_Acceleration;

	//Particle properties
	ofColor m_Color;
	float	m_Radius, m_InitSize;
	int m_Mass;

	void update(float dt);
	void draw();
	bool intersects(particle other);
	void bounce();
	void resolveCollision(particle p1, particle p2);
	particle(ofVec2f pos, float radius);
	~particle();
};
