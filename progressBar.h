#pragma once
#include "gameNode.h"

class progressBar : public gameNode
{
private:
	D2D1_RECT_F _rcProgress;
	float _x, _y;
	float _width;

	image* _progressBarTop;
	image* _progressBarBottom;
	wstring _barName;

public:
	progressBar() {};
	~progressBar() {};

	HRESULT init(float x, float y, int width, int height);
	void release();
	void update();
	void render();

	void setGauge(float currentGauge, float maxGauge);

	void setX(int x) { _x = x; }
	void setY(int y) { _y = y; }

	inline void set_barName(wstring val) { _barName = val; }
	inline wstring get_barName() { return _barName; }
};

