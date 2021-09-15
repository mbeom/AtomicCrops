#include "stdafx.h"
#include "gun.h"

HRESULT gun::init(float x, float y, int num)
{
	IMAGEMANAGER->addImage("squirrel_root", L"image/gun/SeedGunLoot.png", 52, 34);
	IMAGEMANAGER->addImage("squirrel_gun", L"image/gun/Squirrel_gun.png", 52, 34);
	IMAGEMANAGER->addImage("gatling_root", L"image/gun/GatlingGunIconRoot.png", 54, 26);
	IMAGEMANAGER->addImage("description_box_brown", L"image/UI/decription box.png", 600, 200);
	IMAGEMANAGER->addImage("description_box_black", L"image/UI/decription box2.png", 600, 200);
	IMAGEMANAGER->addImage("description_box_green", L"image/UI/decription box3.png", 600, 200);
	IMAGEMANAGER->addImage("cost_icon", L"image/UI/MoneyCostIcon.png", 16, 16);

	object = new ItemObject;

	if (num == 0)
	{
		price = 500;
		object->img = IMAGEMANAGER->findImage("squirrel_root");
		gun_type = GunType::SQUIRREL_GUN;
		bottom_title.description = L"다람쥐 총";
		bottom_description.description = L"다람쥐 탄환 두개를 발사하는 긴 사거리 총입니다.\n 귀엽지만 성능은 과연??....";
	}
	else if (num == 1)
	{
		price = 300;
		object->img = IMAGEMANAGER->findImage("gatling_root");
		gun_type = GunType::GATLING_GUN;
		bottom_title.description = L"개틀링 건";
		bottom_description.description = L"기본 총보다 성능이 강화된 무난한 총입니다.";
	}

	object->x = x;
	object->y = y;
	object->rc = D2DMANAGER->RectMakeCenter(object->x, object->y, object->img->getWidth(), object->img->getHeight());
	object->is_drop = true;
	object->bounce = 0;

	int font_size = 20;
	bottom_title.x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
	bottom_title.y = CAMERAMANAGER->screen.bottom * 0.8 + 24;
	bottom_title.rc = D2DMANAGER->RectMakeCenter(bottom_title.x, bottom_title.y,
		bottom_title.description.length() * font_size * 1.2, font_size * 1.2);
	bottom_title.width = bottom_title.rc.right - bottom_title.rc.left;
	bottom_title.height = bottom_title.rc.bottom - bottom_title.rc.top;
	bottom_title.img = new image;
	bottom_title.img->D_Init(L"image/UI/decription box3.png", bottom_title.width, bottom_title.height);

	bottom_rect.x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
	bottom_rect.y = CAMERAMANAGER->screen.bottom * 0.85;
	bottom_rect.rc = D2DMANAGER->RectMakeCenter(bottom_rect.x, bottom_rect.y,
		WINSIZEX * 0.25, WINSIZEY * 0.1);
	bottom_rect.width = bottom_rect.rc.right - bottom_rect.rc.left;
	bottom_rect.height = bottom_rect.rc.bottom - bottom_rect.rc.top;
	bottom_rect.img = new image;
	bottom_rect.img->D_Init(L"image/UI/decription box3.png", bottom_rect.width, bottom_rect.height);

	bottom_description.x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
	bottom_description.y = CAMERAMANAGER->screen.bottom * 0.845;
	bottom_description.rc = D2DMANAGER->RectMakeCenter(bottom_description.x, bottom_description.y,
		WINSIZEX * 0.245, WINSIZEY * 0.09);
	bottom_description.width = bottom_description.rc.right - bottom_description.rc.left;
	bottom_description.height = bottom_description.rc.bottom - bottom_description.rc.top;
	bottom_description.img = new image;
	bottom_description.img->D_Init(L"image/UI/decription box.png", bottom_description.width, bottom_description.height);

	top_box.description = bottom_title.description;
	top_box.x = (object->rc.left + object->rc.right) / 2;
	top_box.y = object->rc.top * 0.95;
	top_box.rc = D2DMANAGER->RectMakeCenter(top_box.x, top_box.y,
		(bottom_title.description.length() + 2) * font_size * 1.1, font_size * 1.3);
	top_box.width = top_box.rc.right - top_box.rc.left;
	top_box.height = top_box.rc.bottom - top_box.rc.top;
	top_box.img = new image;
	top_box.img->D_Init(L"image/UI/decription box2.png", top_box.width, top_box.height);

	top_description.img = IMAGEMANAGER->findImage("cost_icon");
	top_description.description = to_wstring(price);
	top_description.x = (object->rc.left + object->rc.right) / 2;
	top_description.y = object->rc.top * 0.9;
	top_description.rc = D2DMANAGER->RectMake(top_description.x, top_description.y,
		top_box.width * 0.5, top_box.height);
	top_description.width = top_description.rc.right - top_description.rc.left;
	top_description.height = top_description.rc.bottom - top_description.rc.top;

	quantity = 2;

	type = ItemType::EQUIPMENT;

	int randNum = RND->getFromIntTo(0, (int)Crop_Kind::KIND_NUM);

	seed_kind = (Crop_Kind)randNum;

	is_selling = true;
	return S_OK;
}
