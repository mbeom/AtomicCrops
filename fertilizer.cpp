#include "stdafx.h"
#include "fertilizer.h"

HRESULT fertilizer::init()
{
	IMAGEMANAGER->addFrameImage("fertilizer_item", L"image/crop/FertilizerLoot.png", 64, 64, 2, 2);
	_img = IMAGEMANAGER->findImage("fertilizer_item");

	object = NULL;

	quantity = 0;
	type = ItemType::PICKAXE;

	return S_OK;
}

HRESULT fertilizer::init(float x, float y, bool is_taken)
{
	IMAGEMANAGER->addFrameImage("fertilizer_item", L"image/crop/FertilizerLoot.png", 64, 64, 2, 2);

	object = new ItemObject;
	object->x = x;
	object->y = y;
	object->img = IMAGEMANAGER->findImage("fertilizer_item");
	object->rc = D2DMANAGER->RectMakeCenter(object->x, object->y, object->img->getFrameWidth(), object->img->getFrameHeight());
	object->is_taken = is_taken;
	object->speed = 5.0f;
	object->gravity = 0.2f;
	object->angle = RND->getFromFloatTo(0, PI);
	object->is_drop = false;
	object->bounce = 0;

	int move[] = { 0, 1, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("fertilizer_move", "fertilizer_item", move, 4, 4.0f, true);

	_ani = new animation;
	_ani = KEYANIMANAGER->findAnimation("fertilizer_move");

	quantity = 1;
	type = ItemType::FERTILIZER;
	seed_kind = Crop_Kind::KIND_NUM;

	return S_OK;
}

void fertilizer::release()
{
}
