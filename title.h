#pragma once
#include "gameNode.h"

struct Button
{
	float x, y;
	float widht, height;
	float rgb_alpha;
	float loop_y;
	D2D1_RECT_F rc;
	image* img;
	wstring string;

	bool is_ready;
};

class title : public gameNode
{
private:
	Button* _intro;
	Button* _backLoopscreen;
	Button* _FrontLoopScreen;
	Button* _magentaBackground;
	Button* _logo;
	Button* _link;
	Button* _menuBackground;

	Button* _mouse;
	Button* _menuButton;
	Button* _menuButton2;
	Button* _menuButton3;

	ID2D1SolidColorBrush* orange_brush;
	ID2D1SolidColorBrush* green_brush;

	image* _cabbage;
	image* _cabbage2;

	animation* _cabbageOn;
	animation* _cabbageOn2;

	bool start;

public:

	virtual HRESULT init();
	virtual void release();
	virtual void render();
	virtual void update();
};

