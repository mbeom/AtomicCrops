#pragma once
#include "Object.h"

class NPC : public Object
{
private:

public:

	virtual HRESULT init(float x, float y);
	virtual HRESULT init(float x, float y, int num);
	virtual void release();
	virtual void update();
	virtual void render();
};

