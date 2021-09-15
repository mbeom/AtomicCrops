#pragma once
#include "gameNode.h"

enum class object_type
{
	WELL,
	FORGE,
	FLAG,
	PLANE,
	NPC,
	COPTER,

	TYPE_NUM
};

class Object : public gameNode
{
protected:
	float count;
	float _x, _y;
	float _playerX, _playerY;
	float width, height;
	int currentFrameX, currentFrameY;
	int loopX, loopY;

	image* _img;
	image* _effectImg;
	animation* ani;
	animation* _effect;
	object_type _type;
	D2D1_RECT_F _rc;

	bool is_touched;
	bool is_ready;

public:

	virtual HRESULT init();
	virtual HRESULT init(float x, float y);
	virtual HRESULT init(float x, float y, int num);
	virtual void release();
	virtual void update();
	virtual void render();

	inline void set_touched(bool val) { is_touched = val; }
	inline bool get_touched() { return is_touched; }

	inline void set_ready(bool val) { is_ready = val; }
	inline bool get_ready() { return is_ready; }

	inline void set_type(object_type type) { _type = type; }

	inline object_type get_type() { return _type; }
	inline D2D1_RECT_F get_rect() { return _rc; }

	inline void set_x(float val) { _playerX = val; }
	inline void set_y(float val) { _playerY = val; }

	inline float get_x() { return _x; }
	inline float get_y() { return _y; }
};

