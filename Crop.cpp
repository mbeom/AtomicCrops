#include "stdafx.h"
#include "Crop.h"

HRESULT Crop::init()
{
	return S_OK;
}

HRESULT Crop::init(float x, float y)
{
	return S_OK;
}

void Crop::release()
{

}

void Crop::update()
{
	count += TIMEMANAGER->getElapsedTime();
	angle += 0.5 *TIMEMANAGER->getElapsedTime();

	if (count > 0.1f && !water_need)
	{
		current_gague += grow_speed;
	}

	if (current_gague >= max_gauge && _stage == Crop_stage::stageD)
	{
		water_need = false;
		harvest();
	}
	else if (current_gague >= max_gauge && _stage < Crop_stage::stageD && _stage != Crop_stage::HARVEST)
	{
		_stage = (Crop_stage)((int)_stage + 1);
		currentFrameX = (int)_stage % 2;
		currentFrameY = ((int)_stage / 2) % 2;
		current_gague = 0;
		max_gauge += 10;
		water_need = true;
		animation_setting();
	}

	if (current_gague >= max_gauge && _stage == Crop_stage::LargeStageD)
	{
		water_need = false;
		harvest();
	}
	else if (current_gague >= max_gauge && (_stage > Crop_stage::stageD && _stage < Crop_stage::LargeStageD) && _stage != Crop_stage::LARGE_HARVEST)
	{
		_stage = (Crop_stage)((int)_stage + 1);
		currentFrameX = (int)_stage % 2;
		currentFrameY = ((int)_stage / 2) % 2;
		current_gague = 0;
		max_gauge += 10;
		water_need = true;
		animation_setting();
	}

	for (int i = 0; i < _harvest->getVBullet().size(); i++)
	{
		_harvest->update();
	}

	_iconAction->frameUpdate(TIMEMANAGER->getElapsedTime() * 1.0f);
}

void Crop::render()
{
	
}

void Crop::soil_render()
{
	if (_soil != NULL)
	{
		_soil->FrameRender(_rc.left, _rc.top, currentFrameX, currentFrameY);
	}
}

void Crop::crop_render()
{
	// ���� �ʿ��� ��Ȳ�� �ƴҋ� ���۹� �ִϸ��̼� ����
	if (_crop != NULL && !water_need)
	{	// ������ �ܰ谡 �ƴϸ� �̹��� ����
		if (_stage != Crop_stage::LARGE_HARVEST && _stage != Crop_stage::LargeStageD)
		{
			// ����߸� �̹��� ���̰� �̻��ؼ�.... ����ó��
			if (_kind == Crop_Kind::CABBAGE && _stage == Crop_stage::LargeStageC)
			{
				_crop->aniRender(_rc.left, _rc.bottom - _crop->getFrameHeight() + 15, _cropAction);
			}
			else
				_crop->aniRender(_rc.left, _rc.bottom - _crop->getFrameHeight(), _cropAction);
		}
		else // ������ �ܰ�� �̹��� ��ġ ����
		{
			_crop->aniRender(_rc.left, _rc.bottom - _crop->getFrameHeight() + 50, _cropAction);
		}
	}

	// ���۹��� ���� �ʿ��ҋ� �����ִ� ������ �̹��� ����
	if (_crop != NULL && water_need)
	{
		// �Ǵٽ� �����.......�� ����ó�� ������ OK
		if (_stage < Crop_stage::LargeStageD)
		{
			if (_kind == Crop_Kind::CABBAGE && _stage == Crop_stage::LargeStageC)
			{
				_crop->FrameRender(_rc.left, _rc.bottom - _crop->getFrameHeight() + 15, water_idx, water_idy);
			}
			else
				_crop->FrameRender(_rc.left, _rc.bottom - _crop->getFrameHeight(), water_idx, water_idy);
		}
		else
			_crop->FrameRender(_rc.left, _rc.bottom - _crop->getFrameHeight() + 50, water_idx, water_idy);
	}
}

void Crop::icon_render()
{
	if (angle >= PI2) angle -= PI2;

	if (_icon != NULL && _stage < Crop_stage::LargeStageA)
	{
		if(_stage == Crop_stage::stageC)
			_icon->aniRender(x - _icon->getFrameWidth() / 2, (y - _crop->getFrameHeight() + 32) - 2 * sinf(angle * 180/PI), _iconAction);
		else if (_stage == Crop_stage::stageD)
			_icon->aniRender(x - _icon->getFrameWidth() / 2, (y - _crop->getFrameHeight() + 12) - 2 * sinf(angle * 180 / PI), _iconAction);
		else
			_icon->aniRender(x - _icon->getFrameWidth() / 2, (y - _crop->getFrameHeight()) + 2 * sinf(angle * 180 / PI), _iconAction);
	}

	if (_icon != NULL && _stage > Crop_stage::stageD && _stage < Crop_stage::HARVEST)
	{
		if (_stage == Crop_stage::LargeStageC)
			_icon->aniRender(x - 10, (y - _crop->getFrameHeight() - 10) - 2 * sinf(angle * 180 / PI), _iconAction);
		else if (_stage == Crop_stage::LargeStageD)
			_icon->aniRender(x - 10, (y - _crop->getFrameHeight() + 24) - 2 * sinf(angle * 180 / PI), _iconAction);
		else
			_icon->aniRender(x - 10, (y - _crop->getFrameHeight()) + 2 * sinf(angle * 180 / PI), _iconAction);
	}

	for (int i = 0; i < _harvest->getVBullet().size(); i++)
	{
		_harvest->render();
	}
}

void Crop::animation_init()
{
}

void Crop::animation_setting()
{
}

void Crop::harvest()
{
}
