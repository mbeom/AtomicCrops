#pragma once
#include "enemy.h"
#include "progressBar.h"

struct circle
{
	float x, y;
	float radius;
	float angle;
};


class boss : public enemy
{
private:
	int hit_count;
	float escape_angle;
	float _count, _escape_count;
	float hit_interval;
	float alpha;
	circle _map;
	progressBar* _hp;

	bool is_ready;
	bool is_fired;
	bool is_patrol;
	bool end;

public:

	virtual HRESULT init(float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	void idle();
	void idle_find();
	void move();
	void escape();
	void attack();
	void hurt();
	void dead();
	void update_direction();
};

