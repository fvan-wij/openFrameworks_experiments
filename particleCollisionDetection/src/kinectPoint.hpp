#pragma once

#include "ofMain.h"
#include "ofColor.h"
#include "ofVec2f.h"

class kinectPoint{
public:
	ofVec2f m_Pos, m_ogPos;
	ofVec2f m_Vel;
	ofColor m_ogCol;
	ofColor m_Color;
	bool m_Visible;
	float m_VelVal;
	int m_StrideX, m_StrideY, m_Radius;
	char c;

	void update(int width, int height, bool disturbed);
	void draw(ofTrueTypeFont & font);
	bool resetPos();
	kinectPoint(int x, int y,  int radius);
	~kinectPoint();
};

