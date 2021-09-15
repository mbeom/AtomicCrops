#include "stdafx.h"
#include "seed_cabbage.h"

HRESULT seed_cabbage::init()
{
	IMAGEMANAGER->addImage("seed_cabbage", L"image/crop/Cabbage/Cabbage.png", 48, 48);
	IMAGEMANAGER->addImage("UI_cabbage", L"image/crop/Cabbage/CabbageSelected.png", 64, 88);

	_img = IMAGEMANAGER->findImage("seed_cabbage");
	object = NULL;

	quantity = 0;
	type = ItemType::CONSUMPTION;
	seed_kind = Crop_Kind::CABBAGE;

	return S_OK;
}

HRESULT seed_cabbage::init(float x, float y, bool taken)
{
	IMAGEMANAGER->addImage("seed_cabbage", L"image/crop/Cabbage/Cabbage.png", 48, 48);
	IMAGEMANAGER->addImage("UI_cabbage", L"image/crop/Cabbage/CabbageSelected.png", 64, 88);
	IMAGEMANAGER->addImage("root_cabbage", L"image/crop/Cabbage/CabbageSeedLoot.png", 16, 16);

	object = new ItemObject;
	object->x = x;
	object->y = y;
	object->img = IMAGEMANAGER->findImage("root_cabbage");
	object->rc = D2DMANAGER->RectMakeCenter(object->x, object->y, object->img->getWidth(), object->img->getHeight());
	object->is_taken = taken;
	object->speed = 2.0f;
	object->gravity = 0.2f;
	object->angle = RND->getFromFloatTo(0, PI);
	object->is_drop = false;
	object->bounce = 0;

	quantity = 1;
	type = ItemType::CONSUMPTION;
	seed_kind = Crop_Kind::CABBAGE;
	is_selling = false;

	return S_OK;
}

void seed_cabbage::release()
{
}