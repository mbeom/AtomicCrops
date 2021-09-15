#pragma once
#include "gameNode.h"

class action : public gameNode
{
private:
	image* _image;				//사용할 이미지
	float _startX, _startY;		//시작 좌표
	float _endX, _endY;			//도착 좌표	

	float _angle;				//각도
	float _travelRange;			//여행거리 (도달할 거리)
	float _worldTimeCount;
	float _time;

	bool _isMoving;

public:
	action();
	~action();

	HRESULT init();
	void release();
	void update();
	void render();

	void moveTo(image* image, float endX, float endY, float time);

	void moving();

	bool getIsMoving() { return _isMoving; }
};

