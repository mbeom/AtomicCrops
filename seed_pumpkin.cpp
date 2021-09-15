#include "stdafx.h"
#include "seed_pumpkin.h"

HRESULT seed_pumpkin::init()
{
	IMAGEMANAGER->addImage("seed_pumpkin", L"image/crop/Pumpkin/Pumpkin.png", 48, 48);
	IMAGEMANAGER->addImage("UI_pumpkin", L"image/crop/Pumpkin/PumpkinSelected.png", 64, 88);
	_img = IMAGEMANAGER->findImage("seed_cabbage");
	object = NULL;

	quantity = 0;
	type = ItemType::CONSUMPTION;
	seed_kind = Crop_Kind::PUMPKIN;

	return S_OK;
}

HRESULT seed_pumpkin::init(float x, float y, bool is_taken)
{
	IMAGEMANAGER->addImage("seed_pumpkin", L"image/crop/Pumpkin/Pumpkin.png", 48, 48);
	IMAGEMANAGER->addImage("UI_pumpkin", L"image/crop/Pumpkin/PumpkinSelected.png", 64, 88);
	IMAGEMANAGER->addImage("root_pumpkin", L"image/crop/Pumpkin/PumpkinSeedLoot.png", 16, 16);

	object = new ItemObject;

	object->x = x;
	object->y = y;
	object->img = IMAGEMANAGER->findImage("root_pumpkin");
	object->rc = D2DMANAGER->RectMakeCenter(object->x, object->y, object->img->getWidth(), object->img->getHeight());
	object->is_taken = is_taken;
	object->speed = 2.0f;
	object->gravity = 0.2f;
	object->angle = RND->getFromFloatTo(0, PI);
	object->is_drop = false;
	object->bounce = 0;

	quantity = 1;
	type = ItemType::CONSUMPTION;
	seed_kind = Crop_Kind::PUMPKIN;

	return S_OK;
}

void seed_pumpkin::release()
{
}
