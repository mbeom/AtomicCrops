#pragma once
#include "Object.h"

class Well : public Object
{
private:

public:

	virtual HRESULT init(float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

};

