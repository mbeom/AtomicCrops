#include "stdafx.h"
#include "Plane.h"

HRESULT Plane::init(float x, float y)
{
	IMAGEMANAGER->addFrameImage("plane_flying", L"image/Object/Copter.png", 1760, 704, 5, 2);
	IMAGEMANAGER->addFrameImage("plane_Rope", L"image/Object/CopterARopeDrop.png", 1760, 704, 5, 2);
	IMAGEMANAGER->addFrameImage("plane_Rope2", L"image/Object/CopterAPostRopeDrop2.png", 1760, 704, 5, 2);
	IMAGEMANAGER->addFrameImage("wind", L"image/Object/CopterDustFx.png", 1024, 1024, 4, 4);

	_img = IMAGEMANAGER->findImage("plane_flying");
	_effectImg = IMAGEMANAGER->findImage("wind");

	int plane_move[] = { 4, 3, 2, 1, 0, 9, 8, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("plane_flying", "plane_flying", plane_move, 8, 8.0, true);

	int plane_rope[] = { 4, 3, 2, 1, 0, 9, 8, 7};
	KEYANIMANAGER->addArrayFrameAnimation("plane_rope", "plane_Rope", plane_rope, 8, 8.0, false);

	int plane_rope_dangling[] = { 4, 3, 2, 1, 0, 9, 8, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("plane_rope_dangling", "plane_Rope2", plane_rope_dangling, 8, 8.0, true);

	int wind_effect[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	KEYANIMANAGER->addArrayFrameAnimation("plane_wind", "wind", wind_effect, 12, 8.0, true);

	ani = new animation;
	ani = KEYANIMANAGER->findAnimation("plane_flying");
	ani->start();

	_effect = new animation;
	_effect = KEYANIMANAGER->findAnimation("plane_wind");
	_effect->start();

	_x = start_X = x;
	_y = start_Y = y;
	width = _img->getFrameWidth();
	height = _img->getFrameHeight();

	_rc = D2DMANAGER->RectMakeCenter(_x, _y, width, height);

	_type = object_type::PLANE;

	count = 0;
	is_touched = false;
	is_ready = false;

	return S_OK;
}

void Plane::release()
{
}

void Plane::update()
{
	angle = getAngle(_x, _y, 
		(CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2, 
		(CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2);
	
	distance = getDistance(_x, _y,
		(CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2, 
		(CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2);
	
	if (!ani->isPlay())
	{
		_img = IMAGEMANAGER->findImage("plane_Rope2");
		ani = KEYANIMANAGER->findAnimation("plane_rope_dangling");
		ani->start();
	}
	if (distance < 300 && !is_ready)
	{
		is_touched = true;
		is_ready = true;

		_img = IMAGEMANAGER->findImage("plane_Rope");
		ani = KEYANIMANAGER->findAnimation("plane_rope");
		ani->start();
	}
	else if (distance < 300 && is_ready)
	{
		is_touched = true;
	}
	else
		is_touched = false;

	if (!is_touched)
	{
		_x += cosf(angle) * 3.0f;
		_y += -sinf(angle) * 3.0f;

		_rc = D2DMANAGER->RectMakeCenter(_x, _y, width, height);
	}
}

void Plane::render()
{
	float screen_x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
	float screen_y = (CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2;

	float x = _x - screen_x;
	float y = _y - screen_y;

	CAMERAMANAGER->trans_mat = D2D1::Matrix3x2F::Translation(x, y);
	CAMERAMANAGER->scale_mat = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1.5f, 1.5f), CAMERAMANAGER->center_pos);
	CAMERAMANAGER->back_trans_mat = D2D1::Matrix3x2F::Translation(-1.5 * x, -1.5 * y);

	D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->trans_mat * CAMERAMANAGER->scale_mat * CAMERAMANAGER->back_trans_mat);

	_effectImg->aniRender(_x - 100, _y + 100, _effect);

	_img->aniRender(_x - 130, _y - 120, ani);

	D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}
