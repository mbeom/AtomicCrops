#include "stdafx.h"
#include "NPC.h"

HRESULT NPC::init(float x, float y)
{
	IMAGEMANAGER->addFrameImage("blacksmith", L"image/Object/ExplorerShopkeeper.png", 255, 255, 3, 3);
	IMAGEMANAGER->addFrameImage("doctor", L"image/Object/MedicalDoctor.png", 176, 176, 2, 2);
	IMAGEMANAGER->addFrameImage("localA", L"image/Object/LocalA.png", 476, 176, 7, 2);
	IMAGEMANAGER->addFrameImage("localB", L"image/Object/LocalB.png", 240, 256, 5, 3);
	IMAGEMANAGER->addFrameImage("localC", L"image/Object/LocalC.png", 440, 240, 5, 3);
	IMAGEMANAGER->addFrameImage("localD", L"image/Object/LocalD.png", 440, 264, 5, 3);
	IMAGEMANAGER->addFrameImage("pilot", L"image/Object/LocalD.png", 88, 88, 1, 1);

	_img = IMAGEMANAGER->findImage("blacksmith");

	int move[] = { 0, 1, 2, 3, 4, 5, 6, 7};
	KEYANIMANAGER->addArrayFrameAnimation("blacksmith", "blacksmith", move, 8, 8.0, true);

	ani = new animation;
	ani = KEYANIMANAGER->findAnimation("blacksmith");
	ani->start();

	_x = x;
	_y = y;
	width = _img->getFrameWidth();
	height = _img->getFrameHeight();
	_rc = D2DMANAGER->RectMakeCenter(_x, _y, width, height);

	_type = object_type::NPC;

	return S_OK;
}

HRESULT NPC::init(float x, float y, int num)
{
	IMAGEMANAGER->addFrameImage("bob", L"image/Object/Bob.png", 224, 88, 4, 1);
	IMAGEMANAGER->addFrameImage("blacksmith", L"image/Object/ExplorerShopkeeper.png", 255, 255, 3, 3);
	IMAGEMANAGER->addFrameImage("doctor", L"image/Object/MedicalDoctor.png", 176, 176, 2, 2);
	IMAGEMANAGER->addFrameImage("localA", L"image/Object/LocalA.png", 476, 176, 7, 2);
	IMAGEMANAGER->addFrameImage("localB", L"image/Object/LocalB.png", 240, 256, 5, 3);
	IMAGEMANAGER->addFrameImage("localC", L"image/Object/LocalC.png", 440, 240, 5, 3);
	IMAGEMANAGER->addFrameImage("localD", L"image/Object/LocalD.png", 440, 264, 5, 3);
	IMAGEMANAGER->addFrameImage("pilot", L"image/Object/copterPilot.png", 88, 88, 1, 1);

	if (num == 0)
	{
		_img = IMAGEMANAGER->findImage("bob");

		int move[] = { 0, 1, 2, 3};
		KEYANIMANAGER->addArrayFrameAnimation("bob", "bob", move, 4, 8.0, true);

		ani = new animation;
		ani = KEYANIMANAGER->findAnimation("bob");
		ani->start();
	}
	if (num == 1)
	{
		_img = IMAGEMANAGER->findImage("blacksmith");

		int move[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
		KEYANIMANAGER->addArrayFrameAnimation("blacksmith", "blacksmith", move, 8, 8.0, true);

		ani = new animation;
		ani = KEYANIMANAGER->findAnimation("blacksmith");
		ani->start();
	}
	if (num == 2)
	{
		_img = IMAGEMANAGER->findImage("doctor");

		int move[] = { 0, 1, 2, 3 };
		KEYANIMANAGER->addArrayFrameAnimation("doctor", "doctor", move, 4, 8.0, true);

		ani = new animation;
		ani = KEYANIMANAGER->findAnimation("doctor");
		ani->start();
	}
	if (num == 3)
	{
		_img = IMAGEMANAGER->findImage("localA");

		int move[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
		KEYANIMANAGER->addArrayFrameAnimation("localA", "localA", move, 12, 8.0, true);

		ani = new animation;
		ani = KEYANIMANAGER->findAnimation("localA");
		ani->start();
	}
	if (num == 4)
	{
		_img = IMAGEMANAGER->findImage("localB");

		int move[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
		KEYANIMANAGER->addArrayFrameAnimation("localB", "localB", move, 12, 8.0, true);

		ani = new animation;
		ani = KEYANIMANAGER->findAnimation("localB");
		ani->start();
	}
	if (num == 5)
	{
		_img = IMAGEMANAGER->findImage("localC");

		int move[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
		KEYANIMANAGER->addArrayFrameAnimation("localC", "localC", move, 12, 8.0, true);

		ani = new animation;
		ani = KEYANIMANAGER->findAnimation("localC");
		ani->start();
	}
	if (num == 6)
	{
		_img = IMAGEMANAGER->findImage("localD");

		int move[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
		KEYANIMANAGER->addArrayFrameAnimation("localD", "localD", move, 12, 8.0, true);

		ani = new animation;
		ani = KEYANIMANAGER->findAnimation("localD");
		ani->start();
	}
	if (num == 7)
	{
		_img = IMAGEMANAGER->findImage("pilot");

		int move[] = { 0 };
		KEYANIMANAGER->addArrayFrameAnimation("pilot", "pilot", move, 1, 8.0, false);

		ani = new animation;
		ani = KEYANIMANAGER->findAnimation("pilot");
		ani->start();
	}

	_x = x;
	_y = y;
	width = _img->getFrameWidth();
	height = _img->getFrameHeight();
	_rc = D2DMANAGER->RectMakeCenter(_x, _y, width, height);

	_type = object_type::NPC;

	return S_OK;
}

void NPC::release()
{
}

void NPC::update()
{
}

void NPC::render()
{
	_img->aniRender(_rc.left, _rc.top, ani);

	//D2DMANAGER->Rectangle(_rc);
}
