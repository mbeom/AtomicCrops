#include "stdafx.h"
#include "seed_bean.h"

HRESULT seed_bean::init()
{
	IMAGEMANAGER->addImage("seed_bean", L"image/crop/Bean/BlackEyedBean.png", 48, 48);
	IMAGEMANAGER->addImage("UI_bean", L"image/crop/Bean/BlackEyedBeanSelected.png", 64, 88);
	_img = IMAGEMANAGER->findImage("seed_bean");
	object = NULL;

	quantity = 0;
	type = ItemType::CONSUMPTION;
	seed_kind = Crop_Kind::BEAN;

	return S_OK;
}

HRESULT seed_bean::init(float x, float y, bool is_taken)
{
	IMAGEMANAGER->addImage("seed_bean", L"image/crop/Bean/BlackEyedBean.png", 48, 48);
	IMAGEMANAGER->addImage("UI_bean", L"image/crop/Bean/BlackEyedBeanSelected.png", 64, 88);
	IMAGEMANAGER->addImage("root_bean", L"image/crop/Bean/BlackEyedBeanSeedLoot.png", 16, 16);

	object = new ItemObject;
	object->x = x;
	object->y = y;
	object->img = IMAGEMANAGER->findImage("root_bean");
	object->rc = D2DMANAGER->RectMakeCenter(object->x, object->y, object->img->getWidth(), object->img->getHeight());
	object->is_taken = is_taken;
	object->speed = 5.0f;
	object->gravity = 0.2f;
	object->angle = RND->getFromFloatTo(0, PI);
	object->is_drop = false;
	object->bounce = 0;

	quantity = 1;
	type = ItemType::CONSUMPTION;
	seed_kind = Crop_Kind::BEAN;

	return S_OK;
}

void seed_bean::release()
{
}
