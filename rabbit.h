#pragma once
#include "enemy.h"

class rabbit : public enemy
{
private:
	float _count;
	image* _shoot;

public:
	virtual HRESULT init(float x, float y);
	virtual HRESULT init(float x, float y, bool is_find);
	virtual void release();
	virtual void update();
	virtual void render();

	void idle();
	void idle_find();
	void idle_sleep();
	void sleep_hurt();
	void hurt();
	void move();
	void attack();
	void dead();

	void update_direction();
	
};

