#include "stdafx.h"
#include "Potato.h"

HRESULT Potato::init(float destX, float destY)
{
	IMAGEMANAGER->addFrameImage("potato_small_A", L"image/crop/Potato/PotatoCropStageA.png", 176, 88, 4, 2);
	IMAGEMANAGER->addFrameImage("potato_small_B", L"image/crop/Potato/PotatoCropStageB.png", 220, 88, 5, 2);
	IMAGEMANAGER->addFrameImage("potato_small_C", L"image/crop/Potato/PotatoCropStageC.png", 220, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("potato_small_D", L"image/crop/Potato/PotatoCropStageD.png", 484, 264, 11, 3);

	IMAGEMANAGER->addFrameImage("potato_large_A", L"image/crop/Potato/PotatoLargeCropStageA.png", 176, 176, 2, 2);
	IMAGEMANAGER->addFrameImage("potato_large_B", L"image/crop/Potato/PotatoLargeCropStageB.png", 440, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("potato_large_C", L"image/crop/Potato/PotatoLargeCropStageC.png", 440, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("potato_large_D", L"image/crop/Potato/PotatoLargeCropStageD.png", 968, 528, 11, 3);

	IMAGEMANAGER->addFrameImage("potato_small_harvest", L"image/crop/Potato/PotatoYieldS.png", 440, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("potato_large_harvest", L"image/crop/Potato/PotatoYieldL.png", 880, 528, 5, 3);

	_soil = IMAGEMANAGER->findImage("cropSoil_small");
	_crop = IMAGEMANAGER->findImage("potato_small_A");
	_icon = IMAGEMANAGER->findImage("water_need");

	animation_init();

	x = destX;
	y = destY;
	width = _crop->getFrameWidth();
	height = _crop->getFrameHeight();
	_rc = D2DMANAGER->RectMakeCenter(x, y, width, height);

	_stage = Crop_stage::stageA;
	_kind = Crop_Kind::POTATO;

	currentFrameX = currentFrameY = 0;
	water_idx = water_idy = 0;

	current_gague = 0;
	max_gauge = 20;
	price = 0;
	water_bonus = 0;
	fertile_bonus = 0;

	grow_speed = 0.1f;

	water_need = true;

	return S_OK;
}

void Potato::animation_init()
{
	int small_stage_A[] = { 0, 1, 2, 3, 4 };
	KEYANIMANAGER->addArrayFrameAnimation("potato_small_A", "potato_small_A", small_stage_A, 5, 8.0, true);

	int small_stage_B[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("potato_small_B", "potato_small_B", small_stage_B, 8, 8.0, true);

	int small_stage_C[] = { 0, 1, 2, 3, 4, 5, 6, 7};
	KEYANIMANAGER->addArrayFrameAnimation("potato_small_C", "potato_small_C", small_stage_C, 8, 8.0, true);

	int small_stage_D[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17, 18, 19, 20};
	KEYANIMANAGER->addArrayFrameAnimation("potato_small_D", "potato_small_D", small_stage_D, 20, 8.0, true);

	int large_stage_A[] = { 0, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("potato_large_A", "potato_large_A", large_stage_A, 3, 8.0, true);

	int large_stage_B[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("potato_large_B", "potato_large_B", large_stage_B, 8, 8.0, true);

	int large_stage_C[] = { 0, 1, 2, 3, 4, 5, 6, 7};
	KEYANIMANAGER->addArrayFrameAnimation("potato_large_C", "potato_large_C", large_stage_C, 8, 8.0, true);

	int large_stage_D[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 18, 19, 20, 21};
	KEYANIMANAGER->addArrayFrameAnimation("potato_large_D", "potato_large_D", large_stage_D, 20, 8.0, true);

	int harvest_small[] = {21, 22, 23, 24, 25, 26 };
	KEYANIMANAGER->addArrayFrameAnimation("potato_small_harvest", "potato_small_D", harvest_small, 6, 8.0, true);

	int harvest_large[] = {22, 23, 24, 25, 26, 27 };
	KEYANIMANAGER->addArrayFrameAnimation("potato_large_harvest", "potato_large_D", harvest_large, 6, 8.0, true);

	int water_need[] = { 2 };
	//KEYANIMANAGER->addArrayFrameAnimation("water_need", "water_need", water_need, 1, 4.0, true);

	int water_not_need[] = { 3, 4, 5, 6, 7 };
	//KEYANIMANAGER->addArrayFrameAnimation("water_not_need", "water_need", water_not_need, 5, 8.0, false);

	_cropAction = new animation;
	_cropAction = KEYANIMANAGER->findAnimation("potato_small_A");
	_cropAction->start();

	_iconAction = new animation;
	_iconAction->init(128, 64, 32, 32);
	_iconAction->setPlayFrame(water_need, 1, true);
	_iconAction->setFPS(4.0);
	_iconAction->start();

	_harvest = new harvest_effect;
}

void Potato::animation_setting()
{
	switch (_stage)
	{
	case Crop_stage::stageA:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 1;
		water_idy = 1;

		_crop = IMAGEMANAGER->findImage("potato_small_A");
		_cropAction = KEYANIMANAGER->findAnimation("potato_small_A");
		_cropAction->start();
		break;
	case Crop_stage::stageB:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 3;
		water_idy = 1;

		_crop = IMAGEMANAGER->findImage("potato_small_B");
		_cropAction = KEYANIMANAGER->findAnimation("potato_small_B");
		_cropAction->start();
		break;
	case Crop_stage::stageC:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 3;
		water_idy = 1;

		_crop = IMAGEMANAGER->findImage("potato_small_C");
		_cropAction = KEYANIMANAGER->findAnimation("potato_small_C");
		_cropAction->start();
		break;
	case Crop_stage::stageD:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 5;
		water_idy = 1;

		_crop = IMAGEMANAGER->findImage("potato_small_D");
		_cropAction = KEYANIMANAGER->findAnimation("potato_small_D");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageA:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 1;
		water_idy = 0;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("potato_large_A");
		_cropAction = KEYANIMANAGER->findAnimation("potato_large_A");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageB:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 3;
		water_idy = 1;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("potato_large_B");
		_cropAction = KEYANIMANAGER->findAnimation("potato_large_B");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageC:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 3;
		water_idy = 1;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("potato_large_C");
		_cropAction = KEYANIMANAGER->findAnimation("potato_large_C");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageD:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 5;
		water_idy = 1;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("potato_large_D");
		_cropAction = KEYANIMANAGER->findAnimation("potato_large_D");
		_cropAction->start();
		break;

	default:
		harvest();
		break;
	}

	if (water_need)
	{
		int water_need[] = { 2 };

		_iconAction->setPlayFrame(water_need, 1, true);
		_iconAction->setFPS(4.0);
		_iconAction->start();
	}
	else
	{
		int water_not_need[] = { 3, 4, 5, 6, 7 };

		_iconAction->setPlayFrame(water_not_need, 5, false);
		_iconAction->setFPS(8.0);
		_iconAction->start();
	}
}

void Potato::harvest()
{
	if (_stage < Crop_stage::LargeStageA)
	{
		_stage = Crop_stage::HARVEST;
		_harvest->init(IMAGEMANAGER->findImage("potato_small_harvest"));
		_crop = IMAGEMANAGER->findImage("potato_small_D");
		_cropAction = KEYANIMANAGER->findAnimation("potato_small_harvest");
		_cropAction->start();
		price = 30;
	}
	else
	{
		_stage = Crop_stage::LARGE_HARVEST;
		_harvest->init(IMAGEMANAGER->findImage("potato_large_harvest"));
		_crop = IMAGEMANAGER->findImage("potato_large_D");
		_cropAction = KEYANIMANAGER->findAnimation("potato_large_harvest");
		_cropAction->start();
		price = 240;
	}
}
