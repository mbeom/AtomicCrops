#include "stdafx.h"
#include "seed_heart.h"

HRESULT seed_heart::init()
{
	IMAGEMANAGER->addImage("seed_heart", L"image/crop/Heart/Heartbeet.png", 48, 48);
	IMAGEMANAGER->addImage("UI_heart", L"image/crop/Heart/HeartbeetSelected.png", 64, 88);
	_img = IMAGEMANAGER->findImage("seed_heart");
	object = NULL;

	quantity = 0;
	type = ItemType::CONSUMPTION;
	seed_kind = Crop_Kind::HEART;

	return S_OK;
}

HRESULT seed_heart::init(float x, float y, bool is_taken)
{
	IMAGEMANAGER->addImage("seed_heart", L"image/crop/Heart/Heartbeet.png", 48, 48);
	IMAGEMANAGER->addImage("UI_heart", L"image/crop/Heart/HeartbeetSelected.png", 64, 88);
	IMAGEMANAGER->addImage("root_heart", L"image/crop/Heart/HeartBeatLoot.png", 44, 44);

	object = new ItemObject;

	object->x = x;
	object->y = y;
	object->img = IMAGEMANAGER->findImage("root_heart");
	object->rc = D2DMANAGER->RectMakeCenter(object->x, object->y, object->img->getWidth(), object->img->getHeight());
	object->is_taken = is_taken;
	object->speed = 2.0f;
	object->gravity = 0.2f;
	object->angle = RND->getFromFloatTo(0, PI);
	object->is_drop = false;
	object->bounce = 0;

	quantity = 0;
	type = ItemType::CONSUMPTION;
	seed_kind = Crop_Kind::HEART;

	return S_OK;
}

void seed_heart::release()
{
}
