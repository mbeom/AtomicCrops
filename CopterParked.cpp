#include "stdafx.h"
#include "CopterParked.h"

HRESULT CopterParked::init(float x, float y)
{
	IMAGEMANAGER->addFrameImage("Copterparked", L"image/Object/Copterparked.png", 256, 144, 1, 1);

	_img = IMAGEMANAGER->findImage("Copterparked");

	_x = x;
	_y = y;
	width = _img->getFrameWidth();
	height = _img->getFrameHeight();
	_rc = D2DMANAGER->RectMakeCenter(_x, _y, width, height);

	_type = object_type::COPTER;

	count = 0;
	is_touched = false;
	is_ready = false;

	notice = L"농장으로 되돌아가기 'F' ";

	return S_OK;
}

void CopterParked::release()
{
}

void CopterParked::update()
{
}

void CopterParked::render()
{
	_img->FrameRender(_rc.left, _rc.top, 0, 0);

	D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 20, notice.c_str(), _rc.left, _rc.top, _rc.right, _rc.bottom);
	//D2DMANAGER->Rectangle(_rc);
}
