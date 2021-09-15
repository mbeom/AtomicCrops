#pragma once
#include "Object.h"

class Plane : public Object
{
private:
	float start_X, start_Y;
	float angle;
	float distance;

public:

	virtual HRESULT init(float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();
};

