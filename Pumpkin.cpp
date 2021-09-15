#include "stdafx.h"
#include "Pumpkin.h"

HRESULT Pumpkin::init(float destX, float destY)
{
	IMAGEMANAGER->addFrameImage("pumpkin_small_A", L"image/crop/Pumpkin/PumpkinCropStageA.png", 88, 88, 2, 2);
	IMAGEMANAGER->addFrameImage("pumpkin_small_B", L"image/crop/Pumpkin/PumpkinCropStageB.png", 220, 88, 5, 2);
	IMAGEMANAGER->addFrameImage("pumpkin_small_C", L"image/crop/Pumpkin/PumpkinCropStageC.png", 220, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("pumpkin_small_D", L"image/crop/Pumpkin/PumpkinCropStageD.png", 484, 176, 11, 2);

	IMAGEMANAGER->addFrameImage("pumpkin_large_A", L"image/crop/Pumpkin/PumpkinLargeCropStageA.png", 176, 176, 2, 2);
	IMAGEMANAGER->addFrameImage("pumpkin_large_B", L"image/crop/Pumpkin/PumpkinLargeCropStageB.png", 440, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("pumpkin_large_C", L"image/crop/Pumpkin/PumpkinLargeCropStageC.png", 440, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("pumpkin_large_D", L"image/crop/Pumpkin/PumpkinLargeCropStageD.png", 968, 352, 11, 2);

	IMAGEMANAGER->addFrameImage("pumpkin_small_harvest", L"image/crop/Pumpkin/PumpkinYieldS.png", 176, 176, 2, 2);
	IMAGEMANAGER->addFrameImage("pumpkin_large_harvest", L"image/crop/Pumpkin/PumpkinYieldL.png", 396, 264, 3, 2);

	_soil = IMAGEMANAGER->findImage("cropSoil_small");
	_crop = IMAGEMANAGER->findImage("pumpkin_small_A");
	_icon = IMAGEMANAGER->findImage("water_need");

	animation_init();

	x = destX;
	y = destY;
	width = _crop->getFrameWidth();
	height = _crop->getFrameHeight();
	_rc = D2DMANAGER->RectMakeCenter(x, y, width, height);

	_stage = Crop_stage::stageA;
	_kind = Crop_Kind::PUMPKIN;

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

void Pumpkin::animation_init()
{
	int small_stage_A[] = { 0, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("pumpkin_small_A", "pumpkin_small_A", small_stage_A, 3, 8.0, true);

	int small_stage_B[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("pumpkin_small_B", "pumpkin_small_B", small_stage_B, 8, 8.0, true);

	int small_stage_C[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("pumpkin_small_C", "pumpkin_small_C", small_stage_C, 8, 8.0, true);

	int small_stage_D[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	KEYANIMANAGER->addArrayFrameAnimation("pumpkin_small_D", "pumpkin_small_D", small_stage_D, 12, 8.0, true);

	int large_stage_A[] = { 0, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("pumpkin_large_A", "pumpkin_large_A", large_stage_A, 3, 8.0, true);

	int large_stage_B[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("pumpkin_large_B", "pumpkin_large_B", large_stage_B, 8, 8.0, true);

	int large_stage_C[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("pumpkin_large_C", "pumpkin_large_C", large_stage_C, 8, 8.0, true);

	int large_stage_D[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	KEYANIMANAGER->addArrayFrameAnimation("pumpkin_large_D", "pumpkin_large_D", large_stage_D, 12, 8.0, true);

	int harvest_small[] = { 13, 14, 15, 16, 17, 18 };
	KEYANIMANAGER->addArrayFrameAnimation("pumpkin_small_harvest", "pumpkin_small_D", harvest_small, 6, 8.0, true);

	int harvest_large[] = { 14, 15, 16, 17, 18, 19 };
	KEYANIMANAGER->addArrayFrameAnimation("pumpkin_large_harvest", "pumpkin_large_D", harvest_large, 6, 8.0, true);

	int water_need[] = { 2 };
	//KEYANIMANAGER->addArrayFrameAnimation("water_need", "water_need", water_need, 1, 4.0, true);

	int water_not_need[] = { 3, 4, 5, 6, 7 };
	//KEYANIMANAGER->addArrayFrameAnimation("water_not_need", "water_need", water_not_need, 5, 8.0, false);

	_cropAction = new animation;
	_cropAction = KEYANIMANAGER->findAnimation("pumpkin_small_A");
	_cropAction->start();

	_iconAction = new animation;
	_iconAction->init(128, 64, 32, 32);
	_iconAction->setPlayFrame(water_need, 1, true);
	_iconAction->setFPS(4.0);
	_iconAction->start();

	_harvest = new harvest_effect;
}

void Pumpkin::animation_setting()
{
	switch (_stage)
	{
	case Crop_stage::stageA:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 1;
		water_idy = 0;

		_crop = IMAGEMANAGER->findImage("pumpkin_small_A");
		_cropAction = KEYANIMANAGER->findAnimation("pumpkin_small_A");
		_cropAction->start();
		break;
	case Crop_stage::stageB:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 3;
		water_idy = 1;

		_crop = IMAGEMANAGER->findImage("pumpkin_small_B");
		_cropAction = KEYANIMANAGER->findAnimation("pumpkin_small_B");
		_cropAction->start();
		break;
	case Crop_stage::stageC:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 3;
		water_idy = 1;

		_crop = IMAGEMANAGER->findImage("pumpkin_small_C");
		_cropAction = KEYANIMANAGER->findAnimation("pumpkin_small_C");
		_cropAction->start();
		break;
	case Crop_stage::stageD:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 1;
		water_idy = 1;

		_crop = IMAGEMANAGER->findImage("pumpkin_small_D");
		_cropAction = KEYANIMANAGER->findAnimation("pumpkin_small_D");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageA:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 1;
		water_idy = 0;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("pumpkin_large_A");
		_cropAction = KEYANIMANAGER->findAnimation("pumpkin_large_A");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageB:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 3;
		water_idy = 1;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("pumpkin_large_B");
		_cropAction = KEYANIMANAGER->findAnimation("pumpkin_large_B");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageC:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 3;
		water_idy = 1;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("pumpkin_large_C");
		_cropAction = KEYANIMANAGER->findAnimation("pumpkin_large_C");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageD:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 1;
		water_idy = 1;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("pumpkin_large_D");
		_cropAction = KEYANIMANAGER->findAnimation("pumpkin_large_D");
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

void Pumpkin::harvest()
{
	if (_stage < Crop_stage::LargeStageA)
	{
		_stage = Crop_stage::HARVEST;
		_harvest->init(IMAGEMANAGER->findImage("pumpkin_small_harvest"));
		_crop = IMAGEMANAGER->findImage("pumpkin_small_D");
		_cropAction = KEYANIMANAGER->findAnimation("pumpkin_small_harvest");
		_cropAction->start();
		price = 30;
	}
	else
	{
		_stage = Crop_stage::LARGE_HARVEST;
		_harvest->init(IMAGEMANAGER->findImage("pumpkin_large_harvest"));
		_crop = IMAGEMANAGER->findImage("pumpkin_large_D");
		_cropAction = KEYANIMANAGER->findAnimation("pumpkin_large_harvest");
		_cropAction->start();
		price = 240;
	}
}
