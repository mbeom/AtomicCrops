#pragma once
#include "singletonBase.h"
#include "timer.h"

class timeManager : public singletonBase<timeManager>
{
private:
	timer* _timer;

public:
	timeManager();
	~timeManager();

	int world_time;
	int day;
	float world_count;

	HRESULT init();
	void release();
	void update(float lock = 0.0f);
	void render();
	void render(float x, float y);

	inline float getElapsedTime() const { return _timer->getElapsedTime(); }
	inline float getWorldTime() const { return _timer->getWorldTime(); }
	inline int getFrameRate(void) const { return _timer->getFrameRate(); }
};

