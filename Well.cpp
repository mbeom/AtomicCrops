#include "stdafx.h"
#include "Well.h"

HRESULT Well::init(float x, float y)
{
	IMAGEMANAGER->addFrameImage("well", L"image/Object/Well.png", 512, 512, 4, 4);
	IMAGEMANAGER->addFrameImage("well_splash", L"image/Object/WellSplash.png", 490, 170, 5, 2);

	_img = IMAGEMANAGER->findImage("well");
	_effectImg = IMAGEMANAGER->findImage("well_splash");

	int well_move[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0 };
	KEYANIMANAGER->addArrayFrameAnimation("well_move", "well", well_move, 14, 8.0, false);

	int water_move[] = {6, 0, 1, 2, 3, 4, 5, 6 };
	KEYANIMANAGER->addArrayFrameAnimation("water_move", "well_splash", water_move, 8, 8.0, false);

	ani = new animation;
	ani = KEYANIMANAGER->findAnimation("well_move");

	_effect = new animation;
	_effect = KEYANIMANAGER->findAnimation("water_move");

	_x = x;
	_y = y;
	width = _img->getFrameWidth();
	height = _img->getFrameHeight();
	_rc = D2DMANAGER->RectMakeCenter(_x, _y, width, height);

	_type = object_type::WELL;

	count = 0;
	is_touched = false;
	is_ready = false;

	return S_OK;
}

void Well::release()
{
}

void Well::update()
{
	if (is_touched && is_ready)
	{
		ani->start();
		_effect->start();
		is_touched = false;
		is_ready = false;
	}

	if (!is_ready)
	{
		count += TIMEMANAGER->getElapsedTime();
	}

	if (count > 1.0f)
	{
		is_ready = true;
		count = 0;
	}
}

void Well::render()
{
	_img->aniRender(_rc.left, _rc.top, ani);

	_effectImg->aniRender(_rc.left, _rc.top, _effect);
}
