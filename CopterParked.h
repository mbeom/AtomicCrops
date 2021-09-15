#pragma once
#include "Object.h"

class CopterParked : public Object
{
private:
	wstring notice;

public:

	virtual HRESULT init(float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();
};

