#include "stdafx.h"
#include "Cabbage.h"

HRESULT Cabbage::init(float destX, float destY)
{
	IMAGEMANAGER->addFrameImage("cabbage_small_A", L"image/crop/Cabbage/CabbageCropStageA.png", 88, 88, 2, 2);
	IMAGEMANAGER->addFrameImage("cabbage_small_B", L"image/crop/Cabbage/CabbageCropStageB.png", 220, 88, 5, 2);
	IMAGEMANAGER->addFrameImage("cabbage_small_C", L"image/crop/Cabbage/CabbageCropStageC.png", 220, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("cabbage_small_D", L"image/crop/Cabbage/CabbageCropStageD.png", 484, 176, 11, 2);

	IMAGEMANAGER->addFrameImage("cabbage_large_A", L"image/crop/Cabbage/CabbageLargeCropStageA.png", 176, 176, 2, 2);
	IMAGEMANAGER->addFrameImage("cabbage_large_B", L"image/crop/Cabbage/CabbageLargeCropStageB.png", 440, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("cabbage_large_C", L"image/crop/Cabbage/CabbageLargeCropStageC.png", 440, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("cabbage_large_D", L"image/crop/Cabbage/CabbageLargeCropStageD.png", 968, 352, 11, 2);

	IMAGEMANAGER->addFrameImage("cabbage_small_harvest", L"image/crop/Cabbage/CabbageYieldS.png", 440, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("cabbage_large_harvest", L"image/crop/Cabbage/CabbageYieldL.png", 880, 352, 5, 2);

	_soil = IMAGEMANAGER->findImage("cropSoil_small");
	_crop = IMAGEMANAGER->findImage("cabbage_small_A");
	_icon = IMAGEMANAGER->findImage("water_need");

	animation_init();

	x = destX;
	y = destY;
	width = _crop->getFrameWidth();
	height = _crop->getFrameHeight();
	_rc = D2DMANAGER->RectMakeCenter(x, y, width, height);

	_stage = Crop_stage::stageA;
	_kind = Crop_Kind::CABBAGE;

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

void Cabbage::animation_init()
{
	int small_stage_A[] = { 0, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("cabbage_small_A", "cabbage_small_A", small_stage_A, 3, 8.0, true);

	int small_stage_B[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("cabbage_small_B", "cabbage_small_B", small_stage_B, 8, 8.0, true);

	int small_stage_C[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("cabbage_small_C", "cabbage_small_C", small_stage_C, 8, 8.0, true);

	int small_stage_D[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	KEYANIMANAGER->addArrayFrameAnimation("cabbage_small_D", "cabbage_small_D", small_stage_D, 12, 8.0, true);

	int large_stage_A[] = { 0, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("cabbage_large_A", "cabbage_large_A", large_stage_A, 3, 8.0, true);

	int large_stage_B[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("cabbage_large_B", "cabbage_large_B", large_stage_B, 8, 8.0, true);

	int large_stage_C[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("cabbage_large_C", "cabbage_large_C", large_stage_C, 8, 8.0, true);

	int large_stage_D[] = { 0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13};
	KEYANIMANAGER->addArrayFrameAnimation("cabbage_large_D", "cabbage_large_D", large_stage_D, 12, 8.0, true);

	int harvest_small[] = { 13, 14, 15, 16, 17, 18 };
	KEYANIMANAGER->addArrayFrameAnimation("cabbage_small_harvest", "cabbage_small_D", harvest_small, 6, 8.0, true);

	int harvest_large[] = { 14, 15, 16, 17, 18, 19 };
	KEYANIMANAGER->addArrayFrameAnimation("cabbage_large_harvest", "cabbage_large_D", harvest_large, 6, 8.0, true);

	int water_need[] = { 2 };
	//KEYANIMANAGER->addArrayFrameAnimation("water_need", "water_need", water_need, 1, 4.0, true);

	int water_not_need[] = { 3, 4, 5, 6, 7 };
	//KEYANIMANAGER->addArrayFrameAnimation("water_not_need", "water_need", water_not_need, 5, 8.0, false);

	_cropAction = new animation;
	_cropAction = KEYANIMANAGER->findAnimation("cabbage_small_A");
	_cropAction->start();

	_iconAction = new animation;
	_iconAction->init(128, 64, 32, 32);
	_iconAction->setPlayFrame(water_need, 1, true);
	_iconAction->setFPS(4.0);
	_iconAction->start();

	_harvest = new harvest_effect;
}

void Cabbage::animation_setting()
{
	switch (_stage)
	{
	case Crop_stage::stageA:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 1;
		water_idy = 0;

		_crop = IMAGEMANAGER->findImage("cabbage_small_A");
		_cropAction = KEYANIMANAGER->findAnimation("cabbage_small_A");
		_cropAction->start();
		break;
	case Crop_stage::stageB:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 3;
		water_idy = 1;

		_crop = IMAGEMANAGER->findImage("cabbage_small_B");
		_cropAction = KEYANIMANAGER->findAnimation("cabbage_small_B");
		_cropAction->start();
		break;
	case Crop_stage::stageC:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 3;
		water_idy = 1;

		_crop = IMAGEMANAGER->findImage("cabbage_small_C");
		_cropAction = KEYANIMANAGER->findAnimation("cabbage_small_C");
		_cropAction->start();
		break;
	case Crop_stage::stageD:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 1;
		water_idy = 1;

		_crop = IMAGEMANAGER->findImage("cabbage_small_D");
		_cropAction = KEYANIMANAGER->findAnimation("cabbage_small_D");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageA:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 1;
		water_idy = 0;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("cabbage_large_A");
		_cropAction = KEYANIMANAGER->findAnimation("cabbage_large_A");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageB:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 3;
		water_idy = 1;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("cabbage_large_B");
		_cropAction = KEYANIMANAGER->findAnimation("cabbage_large_B");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageC:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 3;
		water_idy = 1;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("cabbage_large_C");
		_cropAction = KEYANIMANAGER->findAnimation("cabbage_large_C");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageD:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 8;
		water_idy = 0;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("cabbage_large_D");
		_cropAction = KEYANIMANAGER->findAnimation("cabbage_large_D");
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

void Cabbage::harvest()
{
	if (_stage < Crop_stage::LargeStageA)
	{
		_stage = Crop_stage::HARVEST;
		_harvest->init(IMAGEMANAGER->findImage("cabbage_small_harvest"));
		_crop = IMAGEMANAGER->findImage("cabbage_small_D");
		_cropAction = KEYANIMANAGER->findAnimation("cabbage_small_harvest");
		_cropAction->start();
		price = 30;
	}
	else
	{
		_stage = Crop_stage::LARGE_HARVEST;
		_harvest->init(IMAGEMANAGER->findImage("cabbage_large_harvest"));
		_crop = IMAGEMANAGER->findImage("cabbage_large_D");
		_cropAction = KEYANIMANAGER->findAnimation("cabbage_large_harvest");
		_cropAction->start();
		price = 240;
	}
}
