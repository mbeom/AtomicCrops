#include "stdafx.h"
#include "CropManager.h"

HRESULT CropManager::init()
{
	IMAGEMANAGER->addFrameImage("cropSoil_small", L"image/tile/CropMoundSmall.png", 88, 88, 2, 2);
	IMAGEMANAGER->addFrameImage("cropSoil_large", L"image/tile/CropMoundLarge.png", 176, 176, 2, 2);
	IMAGEMANAGER->addFrameImage("water_need", L"image/crop/water_need_small.png", 128, 64, 4, 2);
	IMAGEMANAGER->addFrameImage("water_need_large", L"image/crop/water_need.png", 256, 128, 4, 2);

	return S_OK;
}

void CropManager::release()
{
	for (int i = 0; i < _cropList.size(); i++)
	{
		_cropList[i]->release();
	}
}

void CropManager::update()
{
	for (int i = 0; i < _cropList.size(); i++)
	{
		_cropList[i]->update();
	}

	for (int i = 0; i < _harvestList.size(); i++)
	{
		_harvestList[i]->update();
	}

	check_square();
}

void CropManager::render()
{
	for (int i = 0; i < _cropList.size(); i++)
	{
		_cropList[i]->soil_render();
	}

	for (int i = 0; i < _cropList.size(); i++)
	{
		_cropList[i]->crop_render();
	}

	for (int i = 0; i < _cropList.size(); i++)
	{
		_cropList[i]->icon_render();
	}

	for (int i = 0; i < _harvestList.size(); i++)
	{
		_harvestList[i]->icon_render();
	}

}

void CropManager::set_bean(float x, float y, int idx, int idy)
{
	Crop* _bean = new Bean;
	_bean->init(x, y);
	_bean->set_idX(idx);
	_bean->set_idY(idy);

	_cropList.push_back(_bean);

}

void CropManager::set_potato(float x, float y, int idx, int idy)
{
	Crop* _potato = new Potato;
	_potato->init(x, y);
	_potato->set_idX(idx);
	_potato->set_idY(idy);

	_cropList.push_back(_potato);
}

void CropManager::set_cabbage(float x, float y, int idx, int idy)
{
	Crop* _cabbage = new Cabbage;
	_cabbage->init(x, y);
	_cabbage->set_idX(idx);
	_cabbage->set_idY(idy);

	_cropList.push_back(_cabbage);
}

void CropManager::set_heart(float x, float y, int idx, int idy)
{
	Crop* _heart = new Heart;
	_heart->init(x, y);
	_heart->set_idX(idx);
	_heart->set_idY(idy);

	_cropList.push_back(_heart);
}

void CropManager::set_pumpkin(float x, float y, int idx, int idy)
{
	Crop* _pumpkin = new Pumpkin;
	_pumpkin->init(x, y);
	_pumpkin->set_idX(idx);
	_pumpkin->set_idY(idy);

	_cropList.push_back(_pumpkin);
}

void CropManager::check_square()
{
	for (int i = 0; i < _cropList.size(); i++)
	{
		int tile_idx = _cropList[i]->get_idX();
		int tile_idy = _cropList[i]->get_idY();
		int match_count = 1;
		list<int> delete_list;
		Crop_Kind kind = _cropList[i]->get_kind();
		Crop_stage stage = _cropList[i]->get_stage();

		for (int j = 0; j < _cropList.size(); j++)
		{
			if (i == j) continue;

			int next_idx = _cropList[j]->get_idX();
			int next_idy = _cropList[j]->get_idY();
			Crop_Kind next_kind = _cropList[j]->get_kind();
			Crop_stage next_stage = _cropList[j]->get_stage();

			if (kind == next_kind && stage == next_stage)
			{
				if ((tile_idx == next_idx && tile_idy + 1 == next_idy) ||
					(tile_idx + 1 == next_idx && tile_idy == next_idy) ||
					(tile_idx + 1 == next_idx && tile_idy + 1 == next_idy))
				{
					match_count += 1;
					delete_list.push_back(j);
				}
			}
		}

		if (match_count == 4)
		{
			int stage = (int)_cropList[i]->get_stage() + 4;

			if (stage > (int)Crop_stage::LARGE_HARVEST)	stage = (int)Crop_stage::LARGE_HARVEST;

			_cropList[i]->set_stage((Crop_stage)(stage));
			_cropList[i]->animation_setting();

			int interval = 0;
			for (auto iter = delete_list.begin(); iter != delete_list.end(); iter++)
			{
				_cropList.erase(_cropList.begin() + (*iter - interval));
				interval += 1;
			}
			break;
		}
	}
}
