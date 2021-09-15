#pragma once
#include "enemy.h"

class scorpion : public enemy
{
private:
	float _count;
	float idle_count;
	bool is_fired;

public:

	virtual HRESULT init(float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	void idle();
	void move();
	void attack();
	void hurt();
	void update_direction();
};

