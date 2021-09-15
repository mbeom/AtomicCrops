#include "stdafx.h"
#include "Pickaxe.h"

HRESULT Pickaxe::init()
{
	IMAGEMANAGER->addImage("pickaxe_item", L"image/crop/Pickaxe.png", 36, 36);
	_img = IMAGEMANAGER->findImage("pickaxe");

	object = NULL;

	quantity = 0;
	type = ItemType::PICKAXE;

	return S_OK;
}

HRESULT Pickaxe::init(float x, float y, bool is_taken)
{
	IMAGEMANAGER->addImage("pickaxe_item", L"image/crop/Pickaxe.png", 36, 36);

	object = new ItemObject;
	object->x = x;
	object->y = y;
	object->img = IMAGEMANAGER->findImage("pickaxe_item");
	object->rc = D2DMANAGER->RectMakeCenter(object->x, object->y, object->img->getWidth(), object->img->getHeight());
	object->is_taken = is_taken;
	object->speed = 5.0f;
	object->gravity = 0.2f;
	object->angle = RND->getFromFloatTo(0, PI);
	object->is_drop = false;
	object->bounce = 0;

	quantity = 1;
	type = ItemType::PICKAXE;
	seed_kind = Crop_Kind::KIND_NUM;

	return S_OK;
}

void Pickaxe::release()
{
}
