#include "lineSegment.hpp"

lineSegment::lineSegment(ofVec2f p1, ofVec2f p2)
{
	currentPos = p1;
	newPos = p2;
	direction = ofVec2f(0, ofRandom(3.0, 5.0));
	force = ofVec2f(0, 1);
	fall = false;
	timer = 100;
}

lineSegment::~lineSegment()
{

};

void	lineSegment::update(vector<contourData> contourVector)
{
	ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());
	// float distance = currentPos.distance(mousePos);
	for (int i = 0; i < contourVector.size(); i++)
	{
		float distance = currentPos.distance(contourVector[i].currentPos);
		if (distance < (tileSize.x) && contourVector[i].visible == true)
		{
			fall = true;
			direction = currentPos - contourVector[i].currentPos;
			direction.normalize();
		}
	}
	if (fall == true)
	{
		currentPos += direction;
		newPos += direction;
		direction += force;
		timer--;
	}
}

void	lineSegment::draw()
{
	ofSetColor(color);
	ofDrawLine(currentPos.x, currentPos.y, newPos.x, newPos.y);
}

void 	lineSegment::setTileSize(ofVec2f tile)
{
	tileSize.x = tile.x;
	tileSize.y = tile.y;
}



