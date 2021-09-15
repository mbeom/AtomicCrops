#include "stdafx.h"
#include "Bean.h"

HRESULT Bean::init(float destX, float destY)
{
	IMAGEMANAGER->addFrameImage("bean_small_A", L"image/crop/Bean/BlackEyedBeanCropStageA.png", 88, 88, 2, 2);
	IMAGEMANAGER->addFrameImage("bean_small_B", L"image/crop/Bean/BlackEyedBeanCropStageB.png", 220, 88, 5, 2);
	IMAGEMANAGER->addFrameImage("bean_small_C", L"image/crop/Bean/BlackEyedBeanCropStageC.png", 220, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("bean_small_D", L"image/crop/Bean/BlackEyedBeanCropStageD.png", 484, 176, 11, 2);

	IMAGEMANAGER->addFrameImage("bean_large_A", L"image/crop/Bean/BlackEyedBeanLargeCropStageA.png", 176, 176, 2, 2);
	IMAGEMANAGER->addFrameImage("bean_large_B", L"image/crop/Bean/BlackEyedBeanLargeCropStageB.png", 440, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("bean_large_C", L"image/crop/Bean/BlackEyedBeanLargeCropStageC.png", 440, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("bean_large_D", L"image/crop/Bean/BlackEyedBeanLargeCropStageD.png", 968, 528, 11, 3);

	IMAGEMANAGER->addFrameImage("bean_small_harvest", L"image/crop/Bean/BlackEyedBeanYieldSmall.png", 440, 176, 5, 2);
	IMAGEMANAGER->addFrameImage("bean_large_harvest", L"image/crop/Bean/BlackEyedBeanYieldL.png", 440, 176, 5, 2);

	_soil = IMAGEMANAGER->findImage("cropSoil_small");
	_crop = IMAGEMANAGER->findImage("bean_small_A");
	_icon = IMAGEMANAGER->findImage("water_need");

	animation_init();

	x = destX;
	y = destY;
	width = _crop->getFrameWidth();
	height = _crop->getFrameHeight();
	_rc = D2DMANAGER->RectMakeCenter(x, y, width, height);

	_stage = Crop_stage::stageA;
	_kind = Crop_Kind::BEAN;

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

void Bean::animation_init()
{
	int small_stage_A[] = { 0, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("bean_small_A", "bean_small_A", small_stage_A, 3, 8.0, true);

	int small_stage_B[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("bean_small_B", "bean_small_B", small_stage_B, 8, 8.0, true);

	int small_stage_C[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("bean_small_C", "bean_small_C", small_stage_C, 8, 8.0, true);

	int small_stage_D[] = { 0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12};
	KEYANIMANAGER->addArrayFrameAnimation("bean_small_D", "bean_small_D", small_stage_D, 12, 8.0, true);

	int large_stage_A[] = { 0, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("bean_large_A", "bean_large_A", large_stage_A, 3, 8.0, true);

	int large_stage_B[] = { 0, 1, 2, 3, 4, 5, 6, 7};
	KEYANIMANAGER->addArrayFrameAnimation("bean_large_B", "bean_large_B", large_stage_B, 8, 8.0, true);

	int large_stage_C[] = { 0, 1, 2, 3, 4, 5, 6, 7};
	KEYANIMANAGER->addArrayFrameAnimation("bean_large_C", "bean_large_C", large_stage_C, 8, 8.0, true);

	int large_stage_D[] = { 0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 16, 17};
	KEYANIMANAGER->addArrayFrameAnimation("bean_large_D", "bean_large_D", large_stage_D, 16, 8.0, true);

	int harvest_small[] = { 13, 14, 15, 16, 17, 18 };
	KEYANIMANAGER->addArrayFrameAnimation("harvest_small", "bean_small_D", harvest_small, 6, 8.0, true);

	int harvest_large[] = { 18, 19, 20, 21, 22, 23 };
	KEYANIMANAGER->addArrayFrameAnimation("harvest_large", "bean_large_D", harvest_large, 6, 8.0, true);

	int water_need[] = { 2 };
	//KEYANIMANAGER->addArrayFrameAnimation("water_need", "water_need", water_need, 1, 4.0, true);

	int water_not_need[] = {3, 4, 5, 6, 7 };
	//KEYANIMANAGER->addArrayFrameAnimation("water_not_need", "water_need", water_not_need, 5, 8.0, false);

	_cropAction = new animation;
	_cropAction = KEYANIMANAGER->findAnimation("bean_small_A");
	_cropAction->start();

	_iconAction = new animation;
	_iconAction->init(128, 64, 32, 32);
	_iconAction->setPlayFrame(water_need, 1, true);
	_iconAction->setFPS(4.0);
	_iconAction->start();

	_harvest = new harvest_effect;
}

void Bean::animation_setting()
{
	switch (_stage)
	{
	case Crop_stage::stageA:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 1;
		water_idy = 0;

		_crop = IMAGEMANAGER->findImage("bean_small_A");
		_cropAction = KEYANIMANAGER->findAnimation("bean_small_A");
		_cropAction->start();
		break;
	case Crop_stage::stageB:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 3;
		water_idy = 1;

		_crop = IMAGEMANAGER->findImage("bean_small_B");
		_cropAction = KEYANIMANAGER->findAnimation("bean_small_B");
		_cropAction->start();
		break;
	case Crop_stage::stageC:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 3;
		water_idy = 1;

		_crop = IMAGEMANAGER->findImage("bean_small_C");
		_cropAction = KEYANIMANAGER->findAnimation("bean_small_C");
		_cropAction->start();
		break;
	case Crop_stage::stageD:
		_icon = IMAGEMANAGER->findImage("water_need");
		_iconAction->init(128, 64, 32, 32);
		water_idx = 4;
		water_idy = 0;

		_crop = IMAGEMANAGER->findImage("bean_small_D");
		_cropAction = KEYANIMANAGER->findAnimation("bean_small_D");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageA:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 1;
		water_idy = 0;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("bean_large_A");
		_cropAction = KEYANIMANAGER->findAnimation("bean_large_A");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageB:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 4;
		water_idy = 1;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("bean_large_B");
		_cropAction = KEYANIMANAGER->findAnimation("bean_large_B");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageC:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 4;
		water_idy = 1;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("bean_large_C");
		_cropAction = KEYANIMANAGER->findAnimation("bean_large_C");
		_cropAction->start();
		break;
	case Crop_stage::LargeStageD:
		_icon = IMAGEMANAGER->findImage("water_need_large");
		_iconAction->init(256, 128, 64, 64);
		water_idx = 9;
		water_idy = 0;

		_soil = IMAGEMANAGER->findImage("cropSoil_large");
		_crop = IMAGEMANAGER->findImage("bean_large_D");
		_cropAction = KEYANIMANAGER->findAnimation("bean_large_D");
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

void Bean::harvest()
{
	if (_stage < Crop_stage::LargeStageA)
	{
		_stage = Crop_stage::HARVEST;
		_harvest->init(IMAGEMANAGER->findImage("bean_small_harvest"));
		_crop = IMAGEMANAGER->findImage("bean_small_D");
		_cropAction = KEYANIMANAGER->findAnimation("harvest_small");
		_cropAction->start();
		price = 30;
	}
	else
	{
		_stage = Crop_stage::LARGE_HARVEST;
		_harvest->init(IMAGEMANAGER->findImage("bean_large_harvest"));
		_crop = IMAGEMANAGER->findImage("bean_large_D");
		_cropAction = KEYANIMANAGER->findAnimation("harvest_large");
		_cropAction->start();
		price = 240;
	}
}
