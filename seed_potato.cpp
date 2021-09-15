#include "stdafx.h"
#include "seed_potato.h"

HRESULT seed_potato::init()
{
	IMAGEMANAGER->addImage("seed_potato", L"image/crop/Potato/Potato.png", 48, 48);
	IMAGEMANAGER->addImage("UI_potato", L"image/crop/Potato/PotatoSelected.png", 64, 88);
	_img = IMAGEMANAGER->findImage("seed_cabbage");
	object = NULL;

	quantity = 0;
	type = ItemType::CONSUMPTION;
	seed_kind = Crop_Kind::POTATO;

	return S_OK;
}

HRESULT seed_potato::init(float x, float y, bool is_taken)
{
	IMAGEMANAGER->addImage("seed_potato", L"image/crop/Potato/Potato.png", 48, 48);
	IMAGEMANAGER->addImage("UI_potato", L"image/crop/Potato/PotatoSelected.png", 64, 88);
	IMAGEMANAGER->addImage("root_potato", L"image/crop/Potato/PotatoSelected.png", 20, 20);

	object = new ItemObject;

	object->x = x;
	object->y = y;
	object->img = IMAGEMANAGER->findImage("root_potato");
	object->rc = D2DMANAGER->RectMakeCenter(object->x, object->y, object->img->getWidth(), object->img->getHeight());
	object->is_taken = is_taken;
	object->speed = 2.0f;
	object->gravity = 0.2f;
	object->angle = RND->getFromFloatTo(0, PI);
	object->is_drop = false;
	object->bounce = 0;

	quantity = 1;
	type = ItemType::CONSUMPTION;
	seed_kind = Crop_Kind::POTATO;

	return S_OK;
}

void seed_potato::release()
{
}