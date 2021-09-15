#pragma once
#include "Crop.h"

class Heart : public Crop
{
private:

public:

	virtual HRESULT init(float destX, float destY);
	virtual void animation_init();
	virtual void animation_setting();
	virtual void harvest();
};

