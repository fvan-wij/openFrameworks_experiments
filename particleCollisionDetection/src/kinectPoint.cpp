#include "kinectPoint.hpp"
#include "ofApp.hpp"
#include "ofGraphics.h"

kinectPoint::kinectPoint(int x, int y, int radius) {
	m_Pos = ofVec2f(x, y);
	m_ogPos = m_Pos;
	m_ogCol = m_Color;
	m_Color = ofColor(0, 255, 0);
	m_Radius = radius;
	m_Visible = false;
	m_StrideX = 1024 / 640;
	m_StrideY = 768 / 480;
	m_VelVal = 1.0;
	m_Vel = ofVec2f(ofRandom(-m_VelVal, m_VelVal), ofRandom(-m_VelVal, m_VelVal));

	c = ofRandom(61, 92);
}

kinectPoint::~kinectPoint() {
}

// void kinectPoint::loadFont()
// {
// 	ascii.load("../bin/data/fonts/CRASEY.otf", 32);
// }

void kinectPoint::draw(ofTrueTypeFont & font)
{
	if (m_Visible)
	{
		ofSetColor(m_Color);
		ofDrawBitmapString(c, m_Pos.x, m_Pos.y);
	}
}

void kinectPoint::update(int width, int height, bool disturbed)
{
	if (m_Color.getBrightness() > 200 && disturbed)
		m_Pos += m_Vel;
	else if (m_Color.getBrightness() > 200 && !disturbed)
	 	resetPos();

}

bool kinectPoint::resetPos()
{
	ofVec2f dir;
	dir = m_ogPos - m_Pos;
	dir.normalize() *= 1.5;
	m_Pos += dir;
	if (m_Pos == m_ogPos)
		return (true);
	else
	 	return (false);
}
