#pragma once

class AnimOb;

#include "GUIObject.h"

#define DEFAULT_SPEED 20

class AnimOb : public CGUIObject
{
public:
	enum AnimType { NONE, BUTTON, WINDOW };

protected:
	AnimType animType;
	int endX, endY;
	int	m_speed;		// How fast the object scrolls

public:
	AnimOb() : CGUIObject() {
		endX = 0;
		endY = 0;
		animType = NONE;
		m_speed = DEFAULT_SPEED;
	}
	AnimOb(int x, int y, int h, int w) : CGUIObject(x,y,h,w) {
		endX = 0;
		endY = 0;
		animType = NONE;
		m_speed = DEFAULT_SPEED;
	}

	virtual void Animate() = 0;
	void SetEndX(int x) { endX = x; }
	void SetEndY(int y) { endY = y; }
	void SetAnimType(AnimType t) { animType = t; }
	int GetEndX() { return endX; }
	int GetEndY() { return endY; }
	AnimType GetAnimType() { return animType; }
};
