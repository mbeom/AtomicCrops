#pragma once
#include "enemy.h"

class maple : public enemy
{
private:
	float _count;
	float is_fired;

public:

	virtual HRESULT init(float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	void idle();
	void attack();
	void hurt();
	void update_direction();

};

