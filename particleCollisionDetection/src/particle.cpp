#include "particle.hpp"
#include "ofApp.hpp"
#include "ofAppRunner.h"
#include "ofUtils.h"

particle::particle(ofVec2f pos, float radius){
	m_Position = pos;
	m_Velocity = ofVec2f(ofRandom(-20, 20), ofRandom(-20, 20));
	m_Acceleration = ofVec2f(ofRandom(-10.0, 10.0), ofRandom(-10.0, 10.0));
	m_Radius = radius;
	m_Mass = radius * 2;
	m_Color = ofColor(0);
}
particle::~particle(){

}

bool particle::intersects(particle other)
{
	float distance = this->m_Position.distance(other.m_Position);
	if (distance < (this->m_Radius + other.m_Radius) * 2)
		return true;
	else
	 	return false;
}


void particle::bounce(){
	if (m_Position.x - m_Radius < 0)
	{
		m_Position.x = m_Radius;
		m_Velocity.x *= -1;
	}
	else if (m_Position.x + m_Radius > WIDTH)
	{
		m_Position.x = ofGetWidth() - m_Radius;
		m_Velocity.x *= -1;
	}

	if (m_Position.y - m_Radius < 0)
	{
		m_Position.y = m_Radius;
		m_Velocity.y *= -1;
	}
	else if (m_Position.y + m_Radius > HEIGHT)
	{
		m_Position.y = ofGetHeight() - m_Radius;
		m_Velocity.y *= -1;
	}
}

void particle::update(float dt){
	m_Velocity = m_Velocity + m_Acceleration * dt;
	m_Position = m_Position + m_Velocity * dt;
	bounce();
}
void particle::draw(){
	ofFill();
	ofSetColor(m_Color);
	ofDrawCircle(m_Position.x, m_Position.y, m_Radius * 2);
}
