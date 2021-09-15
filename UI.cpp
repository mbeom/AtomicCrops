#include "stdafx.h"
#include "UI.h"
#include "player.h"

HRESULT UI::init()
{
	clear();

	IMAGEMANAGER->addFrameImage("heart_icon", L"image/UI/HeartFull.png", 32, 32, 1, 1);
	IMAGEMANAGER->addFrameImage("heart_empty", L"image/UI/HeartEmpty.png", 32, 32, 1, 1);
	IMAGEMANAGER->addFrameImage("money_icon", L"image/UI/MoneyIcon.png", 48, 48, 1, 1);
	IMAGEMANAGER->addFrameImage("pickaxe_icon", L"image/UI/PickaxeIcon.png", 48, 48, 1, 1);
	IMAGEMANAGER->addFrameImage("fertilizer_icon", L"image/UI/FertilizerIcon.png", 48, 48, 1, 1);
	IMAGEMANAGER->addFrameImage("water_icon", L"image/UI/WaterIcon.png", 48, 48, 1, 1);
	IMAGEMANAGER->addFrameImage("time_background", L"image/UI/UiHudTimeBgSpring.png", 264, 264, 1, 1);
	IMAGEMANAGER->addFrameImage("daytime", L"image/UI/daytime.png", 504, 216, 7, 3);
	IMAGEMANAGER->addFrameImage("calendar", L"image/UI/CalendarSpring.png", 64, 64, 1, 1);
	IMAGEMANAGER->addFrameImage("day1", L"image/UI/Day1.png", 64, 64, 1, 1);
	IMAGEMANAGER->addFrameImage("day2", L"image/UI/Day2.png", 64, 64, 1, 1);
	IMAGEMANAGER->addFrameImage("day3", L"image/UI/Day3.png", 64, 64, 1, 1);
	IMAGEMANAGER->addFrameImage("UI_number", L"image/UI/UI_number.png", 198, 30, 11, 1);
	IMAGEMANAGER->addFrameImage("night", L"image/UI/UiHudNight.png", 72, 72, 1, 1);
	IMAGEMANAGER->addFrameImage("dusk", L"image/UI/UiHudDawnDusk.png", 72, 72, 1, 1);
	IMAGEMANAGER->addImage("score_bg", L"image/UI/ScoreBackground.png", 108, 108);

	for (int i = 0; i < _player->get_playerStatus().max_hp; i++)
	{
		UI_info* _heart = new UI_info;
		if(i <_player->get_playerStatus().current_hp)
			_heart->img = IMAGEMANAGER->findImage("heart_icon");
		else
			_heart->img = IMAGEMANAGER->findImage("heart_empty");
		_heart->x = CAMERAMANAGER->screen.left + 280 + _heart->img->getFrameWidth() * i;
		_heart->y = CAMERAMANAGER->screen.top + 160;
		_heart->rc = D2DMANAGER->RectMake(_heart->x, _heart->y, _heart->img->getFrameWidth(), _heart->img->getFrameHeight());

		_heartList.push_back(_heart);
	}

	past_heart = _player->get_playerStatus().current_hp;

	_timeBackground.img = IMAGEMANAGER->findImage("time_background");
	_timeBackground.x = CAMERAMANAGER->screen.right - _timeBackground.img->getFrameWidth();
	_timeBackground.y = CAMERAMANAGER->screen.top + 160;
	_timeBackground.rc = D2DMANAGER->RectMake(_timeBackground.x, _timeBackground.y, _timeBackground.img->getFrameWidth(), _timeBackground.img->getFrameHeight());

	_calender.img = IMAGEMANAGER->findImage("calendar");
	_day.img = IMAGEMANAGER->findImage("day1");

	_gold_icon.img			= IMAGEMANAGER->findImage("money_icon");
	_water_icon.img			= IMAGEMANAGER->findImage("water_icon");
	_pickaxe_icon.img		= IMAGEMANAGER->findImage("pickaxe_icon");
	_fertilizer_icon.img	= IMAGEMANAGER->findImage("fertilizer_icon");

	int current_water = _player->get_playerResource().water;
	int max_water = _player->get_playerStatus().max_water;

	_water_indicator[0].img = IMAGEMANAGER->findImage("UI_number");
	_water_indicator[0].currentFrameX = current_water;
	_water_indicator[0].currentFrameY = 0;

	_water_indicator[1].img = IMAGEMANAGER->findImage("UI_number");
	_water_indicator[1].currentFrameX = 10;
	_water_indicator[1].currentFrameY = 0;

	_water_indicator[2].img = IMAGEMANAGER->findImage("UI_number");
	_water_indicator[2].currentFrameX = max_water;
	_water_indicator[2].currentFrameY = 0;

	_currentItem.x = CAMERAMANAGER->screen.left + 260 + 44;
	_currentItem.y = (CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2;
	_currentItem.rc = D2DMANAGER->RectMakeCenter(_currentItem.x, _currentItem.y, 88, 88);

	_currentItem.img = NULL;

	_mouse.x = _ptMouse.x;
	_mouse.y = _ptMouse.y;
	_mouse.img = IMAGEMANAGER->findImage("cursor");
	_mouse.rc = D2DMANAGER->RectMakeCenter(_mouse.x, _mouse.y, _mouse.img->getWidth(), _mouse.img->getHeight());

	if(TIMEMANAGER->world_time < 330)
		_daytime.img = IMAGEMANAGER->findImage("daytime");
	else
		_daytime.img = IMAGEMANAGER->findImage("dusk");

	index = 0;
	currentItemIndex = _player->get_playerResource().inventory.size() / 2;
	currentFrameX = currentFrameY = 0;

	past_gold = past_axe = past_fertilizer = -1;
	past_item = 0;
	past_size = 0;

	start = false;

	inventory_init();

	return S_OK;
}

void UI::release()
{
}

void UI::update()
{
	max_heart_count();

	if(past_heart != _player->get_playerStatus().current_hp)
		heart_count();

	if (past_gold != _player->get_playerResource().gold);
		gold_count();

	if (past_axe != _player->get_playerResource().pickaxe);
		axe_count();

	if (past_fertilizer != _player->get_playerResource().fertilizer);
		fertilizer_count();
	
	if (past_size != _player->get_playerResource().inventory.size())
	{
		inventory_size_check();
	}
	else if (past_item != _player->get_inventorySize())
	{
		inventory_Quantity_check();
	}

	_currentItem.x = CAMERAMANAGER->screen.left + 260 + 44;
	_currentItem.y = (CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2;
	_currentItem.rc = D2DMANAGER->RectMakeCenter(_currentItem.x, _currentItem.y, 88, 88);

	for (int i = 0; i < _heartList.size(); i++)
	{
		_heartList[i]->x = CAMERAMANAGER->screen.left + 280 + _heartList[i]->img->getFrameWidth() * i;
		_heartList[i]->y = CAMERAMANAGER->screen.top + 160;
		_heartList[i]->rc = D2DMANAGER->RectMake(_heartList[i]->x, _heartList[i]->y, _heartList[i]->img->getFrameWidth(), _heartList[i]->img->getFrameHeight());
	}

	for (int i = 0; i < _gold.size(); i++)
	{
		_gold[i]->x = CAMERAMANAGER->screen.left + 320 + _gold[i]->img->getFrameWidth() * i;
		_gold[i]->y = CAMERAMANAGER->screen.top + 220;
		_gold[i]->rc = D2DMANAGER->RectMake(_gold[i]->x, _gold[i]->y, _gold[i]->img->getFrameWidth(), _gold[i]->img->getFrameHeight());
	}
	
	for (int i = 0; i < _axeList.size(); i++)
	{
		_axeList[i]->x = CAMERAMANAGER->screen.left + 440 + _axeList[i]->img->getFrameWidth() * i;
		_axeList[i]->y = CAMERAMANAGER->screen.bottom - 192;
		_axeList[i]->rc = D2DMANAGER->RectMake(_axeList[i]->x, _axeList[i]->y, _axeList[i]->img->getFrameWidth(), _axeList[i]->img->getFrameHeight());
	}

	for (int i = 0; i < _fertilizerList.size(); i++)
	{
		_fertilizerList[i]->x = CAMERAMANAGER->screen.left + 560 + _fertilizerList[i]->img->getFrameWidth() * i;
		_fertilizerList[i]->y = CAMERAMANAGER->screen.bottom - 192;
		_fertilizerList[i]->rc = D2DMANAGER->RectMake(_fertilizerList[i]->x, _fertilizerList[i]->y, _fertilizerList[i]->img->getFrameWidth(), _fertilizerList[i]->img->getFrameHeight());
	}

	for (int i = 0; i < _itemList.size(); i++)
	{
		D2D1_RECT_F temp;

		_itemList[i]->x = _currentItem.x - _itemList[i]->img->getWidth();
	
		if ((int)(_itemList.size() / 2) - i >= 0)
			_itemList[i]->y = _currentItem.y +
			((int)(_itemList.size() / 2) - i) * _itemList[i]->img->getHeight();
		else
			_itemList[i]->y = _currentItem.y - _itemList[i]->img->getHeight() +
			((int)(_itemList.size() / 2) - i) * _itemList[i]->img->getHeight();
	
		_itemList[i]->rc = D2DMANAGER->RectMake(_itemList[i]->x, _itemList[i]->y, _itemList[i]->img->getWidth(), _itemList[i]->img->getHeight());
		
	}

	int current_water = _player->get_playerResource().water;
	int max_water = _player->get_playerStatus().max_water;

	_water_indicator[0].currentFrameX = current_water;
	_water_indicator[2].currentFrameX = max_water;

	_timeBackground.x = CAMERAMANAGER->screen.right - _timeBackground.img->getFrameWidth() - 132;
	_timeBackground.y = CAMERAMANAGER->screen.top + 20;
	_timeBackground.rc = D2DMANAGER->RectMake(_timeBackground.x, _timeBackground.y, _timeBackground.img->getFrameWidth(), _timeBackground.img->getFrameHeight());

	if (TIMEMANAGER->world_time < 180)
		_daytime.img = IMAGEMANAGER->findImage("daytime");
	else if (TIMEMANAGER->world_time > 330)
	{
		_daytime.img = IMAGEMANAGER->findImage("dusk");
		currentFrameX = 0;
		currentFrameY = 0;
	}

	if (TIMEMANAGER->world_time != 0 && TIMEMANAGER->world_time <= 180)
	{
		_count += TIMEMANAGER->getElapsedTime();
	}

	if (TIMEMANAGER->day == 2)
	{
		_day.img = IMAGEMANAGER->findImage("day2");
	}
	else if (TIMEMANAGER->day == 3)
	{
		_day.img = IMAGEMANAGER->findImage("day3");
	}

	if (_count > 10)
	{
		_count = 0;
		currentFrameX += 1;

		if (currentFrameX > _daytime.img->getMaxFrameX())
		{
			currentFrameX = 0;
			currentFrameY += 1;
		}

		if (currentFrameY > _daytime.img->getMaxFrameY())
		{
			_daytime.img = IMAGEMANAGER->findImage("night");
			currentFrameX = 0;
			currentFrameY = 0;
		}

		_daytime.img->setFrameX(currentFrameX);
		_daytime.img->setFrameY(currentFrameY);
	}

	_mouse.x = _ptMouse.x;
	_mouse.y = _ptMouse.y;
	_mouse.rc = D2DMANAGER->RectMakeCenter(_mouse.x, _mouse.y, _mouse.img->getWidth(), _mouse.img->getHeight());

	inventory_move();
}

void UI::render()
{
	// ÁÂÃø »ó´Ü ÇÏÆ® °¹¼ö ·»´õ
	for (int i = 0; i < _heartList.size(); i++)
	{
		_heartList[i]->img->render(_heartList[i]->rc.left, _heartList[i]->rc.top);
	}

	// ¿ìÃø »ó´Ü ½Ã°£ ¹× ´Þ·Â ·»´õ
	
	_timeBackground.img->FrameRender(_timeBackground.rc.left, _timeBackground.rc.top, 0, 0);

	_daytime.img->FrameRender(	(_timeBackground.rc.left + _timeBackground.rc.right) / 2 - _daytime.img->getFrameWidth(), 
								(_timeBackground.rc.top + _timeBackground.rc.bottom) / 2, 
								currentFrameX, currentFrameY);

	_calender.img->FrameRender((_timeBackground.rc.left + _timeBackground.rc.right) / 2 - _daytime.img->getFrameWidth(), 
								_timeBackground.rc.bottom - _calender.img->getFrameHeight() / 2, 
								0, 0);

	_day.img->FrameRender((	_timeBackground.rc.left + _timeBackground.rc.right) / 2 - _daytime.img->getFrameWidth(),
							_timeBackground.rc.bottom - _day.img->getFrameHeight() / 2, 
							0, 0);

	// UI È­¸éÀÇ °¢ ¾ÆÀÌÄÜµé(µ·, ¹°, °î±ªÀÌ, ºñ·á)

	_gold_icon.img->		FrameRender(CAMERAMANAGER->screen.left + 272, CAMERAMANAGER->screen.top + 200, 0, 0);
	_water_icon.img->		FrameRender(CAMERAMANAGER->screen.left + 280, CAMERAMANAGER->screen.bottom - 200, 0, 0);
	_pickaxe_icon.img->		FrameRender(CAMERAMANAGER->screen.left + 400, CAMERAMANAGER->screen.bottom - 200, 0, 0);
	_fertilizer_icon.img->	FrameRender(CAMERAMANAGER->screen.left + 520, CAMERAMANAGER->screen.bottom - 200, 0, 0);

	// ¼ÒÁöÇÑ ¹° °¹¼ö ·»´õ 8 / 8
	_water_indicator[0].img->FrameRender(CAMERAMANAGER->screen.left + 320, CAMERAMANAGER->screen.bottom - 192,
							_water_indicator[0].currentFrameX, _water_indicator[0].currentFrameY);

	_water_indicator[1].img->FrameRender(CAMERAMANAGER->screen.left + 335, CAMERAMANAGER->screen.bottom - 192,
		_water_indicator[1].currentFrameX, _water_indicator[1].currentFrameY);

	_water_indicator[2].img->FrameRender(CAMERAMANAGER->screen.left + 350, CAMERAMANAGER->screen.bottom - 192,
		_water_indicator[2].currentFrameX, _water_indicator[2].currentFrameY);

	// ¼ÒÁö °ñµåÀÇ ¼ýÀÚ ·»´õ
	for (int i = 0; i < _gold.size(); i++)
	{
		_gold[i]->img->FrameRender(_gold[i]->rc.left, _gold[i]->rc.top, _gold[i]->currentFrameX, _gold[i]->currentFrameY);
	}

	// ¼ÒÁö °î±ªÀÌ¼ö ·»´õ
	for (int i = 0; i < _axeList.size(); i++)
	{
		_axeList[i]->img->FrameRender(_axeList[i]->rc.left, _axeList[i]->rc.top, _axeList[i]->currentFrameX, _axeList[i]->currentFrameY);
	}

	// ¼ÒÁö ºñ·á¼ýÀÚ ·»´õ
	for (int i = 0; i < _fertilizerList.size(); i++)
	{
		_fertilizerList[i]->img->FrameRender(_fertilizerList[i]->rc.left, _fertilizerList[i]->rc.top, 
											 _fertilizerList[i]->currentFrameX, _fertilizerList[i]->currentFrameY);
	}

	for (int i = 0; i < _itemList.size(); i++)
	{
		if (_itemList[i]->rc.top == _currentItem.y) continue;
		if (_itemList[i]->rc.bottom == _currentItem.y) continue;

		_itemList[i]->img->render(_itemList[i]->rc.left, _itemList[i]->rc.top);

		_itemList[i]->font->FrameRender(_itemList[i]->rc.right, (_itemList[i]->rc.top + _itemList[i]->rc.bottom) / 2,
										_itemList[i]->currentFrameX, _itemList[i]->currentFrameY);
		//D2DMANAGER->Rectangle(_itemList[i]->rc);
	}

	if (_currentItem.img != NULL)
	{
		_currentItem.img->render(_currentItem.rc.left, _currentItem.rc.top);
		_currentItem.font->FrameRender(_currentItem.x, _currentItem.y, _currentItem.currentFrameX, _currentItem.currentFrameY);
		//D2DMANAGER->Rectangle(_currentItem.rc);
	}

	float screen_x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
	float screen_y = (CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2;

	float x = _mouse.x - screen_x;
	float y = _mouse.y - screen_y;

	CAMERAMANAGER->trans_mat = D2D1::Matrix3x2F::Translation(x, y);
	CAMERAMANAGER->scale_mat = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1.5f, 1.5f), CAMERAMANAGER->center_pos);
	CAMERAMANAGER->back_trans_mat = D2D1::Matrix3x2F::Translation(-2 * x, -2 * y);

	D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->trans_mat * CAMERAMANAGER->scale_mat* CAMERAMANAGER->back_trans_mat);

	_mouse.img->render(_mouse.rc.left, _mouse.rc.top);

	//D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void UI::max_heart_count()
{
	if (_heartList.size() < _player->get_playerStatus().max_hp)
	{
		UI_info* _heart = new UI_info;
		_heart->img = IMAGEMANAGER->findImage("heart_empty");
		_heart->x = CAMERAMANAGER->screen.left + 280 + _heart->img->getFrameWidth() * _heartList.size();
		_heart->y = CAMERAMANAGER->screen.top + 160;
		_heart->rc = D2DMANAGER->RectMake(_heart->x, _heart->y, _heart->img->getFrameWidth(), _heart->img->getFrameHeight());

		_heartList.push_back(_heart);
	}
}

void UI::heart_count()
{
	past_heart = _player->get_playerStatus().current_hp;

	if (_player->get_playerStatus().current_hp > _heartList.size())
	{
		_heartList.clear();
		for (int i = 0; i < _player->get_playerStatus().current_hp; i++)
		{
			UI_info* _heart = new UI_info;
			_heart->img = IMAGEMANAGER->findImage("heart_icon");
			_heart->x = CAMERAMANAGER->screen.left + 280 + _heart->img->getFrameWidth() * i;
			_heart->y = CAMERAMANAGER->screen.top + 160;
			_heart->rc = D2DMANAGER->RectMake(_heart->x, _heart->y, _heart->img->getFrameWidth(), _heart->img->getFrameHeight());

			_heartList.push_back(_heart);
		}
	}

	for (int i = 0; i < _player->get_playerStatus().current_hp; i++)
	{
		_heartList[i]->img = IMAGEMANAGER->findImage("heart_icon");
	}

	if (_player->get_playerStatus().current_hp < _player->get_playerStatus().max_hp)
	{
		int diff = _player->get_playerStatus().max_hp - _player->get_playerStatus().current_hp;
		int interval = 1;

		for (int i = 0; i < diff; i++)
		{
			_heartList[_heartList.size() - interval]->img = IMAGEMANAGER->findImage("heart_empty");
			interval += 1;
		}
	}
}

void UI::gold_count()
{
	_gold.clear();

	past_gold = _player->get_playerResource().gold;

	int length = to_string(_player->get_playerResource().gold).length();
	int assets = _player->get_playerResource().gold;
	int temp_num = 0;

	vector<int> number_list;

	for (int i = length; i > 0; i--)
	{
		int divide = pow(10, i - 1);

		temp_num = (assets / divide) % 10;

		number_list.push_back(temp_num);
	}

	for (int i = 0; i < number_list.size(); i++)
	{
		UI_info* gold_number = new UI_info;
		gold_number->img = IMAGEMANAGER->findImage("number");
		gold_number->x = CAMERAMANAGER->screen.left + 320 + gold_number->img->getFrameWidth() * i;
		gold_number->y = CAMERAMANAGER->screen.top + 220;
		gold_number->rc = D2DMANAGER->RectMake(gold_number->x, gold_number->y, gold_number->img->getFrameWidth(), gold_number->img->getFrameHeight());
		gold_number->currentFrameX = number_list[i];
		gold_number->currentFrameY = 0;

		_gold.push_back(gold_number);
	}
}

void UI::axe_count()
{
	_axeList.clear();

	past_axe = _player->get_playerResource().pickaxe;

	int length = to_string(_player->get_playerResource().pickaxe).length();
	int assets = _player->get_playerResource().pickaxe;
	int temp_num = 0;

	vector<int> number_list;

	for (int i = length; i > 0; i--)
	{
		int divide = pow(10, i - 1);

		temp_num = (assets / divide) % 10;

		number_list.push_back(temp_num);
	}

	for (int i = 0; i < number_list.size(); i++)
	{
		UI_info* axe_number = new UI_info;
		axe_number->img = IMAGEMANAGER->findImage("UI_number");
		axe_number->x = CAMERAMANAGER->screen.left + 440 + axe_number->img->getFrameWidth() * i;
		axe_number->y = CAMERAMANAGER->screen.bottom - 192;
		axe_number->rc = D2DMANAGER->RectMake(axe_number->x, axe_number->y, axe_number->img->getFrameWidth(), axe_number->img->getFrameHeight());
		axe_number->currentFrameX = number_list[i];
		axe_number->currentFrameY = 0;

		_axeList.push_back(axe_number);
	}
}

void UI::fertilizer_count()
{
	_fertilizerList.clear();

	past_fertilizer = _player->get_playerResource().fertilizer;

	int length = to_string(_player->get_playerResource().fertilizer).length();
	int assets = _player->get_playerResource().fertilizer;
	int temp_num = 0;

	vector<int> number_list;

	for (int i = length; i > 0; i--)
	{
		int divide = pow(10, i - 1);

		temp_num = (assets / divide) % 10;

		number_list.push_back(temp_num);
	}

	for (int i = 0; i < number_list.size(); i++)
	{
		UI_info* ferti_number = new UI_info;
		ferti_number->img = IMAGEMANAGER->findImage("UI_number");
		ferti_number->x = CAMERAMANAGER->screen.left + 560 + ferti_number->img->getFrameWidth() * i;
		ferti_number->y = CAMERAMANAGER->screen.bottom - 192;
		ferti_number->rc = D2DMANAGER->RectMake(ferti_number->x, ferti_number->y, ferti_number->img->getFrameWidth(), ferti_number->img->getFrameHeight());
		ferti_number->currentFrameX = number_list[i];
		ferti_number->currentFrameY = 0;

		_fertilizerList.push_back(ferti_number);
	}
}

void UI::inventory_move()
{
	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		index += 1;
		currentItemIndex = (currentItemIndex + 1) % _itemList.size();

		if (index >= _itemList.size()) index = 0;

		for (int i = 0; i < _itemList.size(); i++)
		{
			_itemList[i]->img = _shuffle_img[(index + i) % _itemList.size()];
			_itemList[i]->kind = _shuffle_kind[(index + i) % _itemList.size()];
			_itemList[i]->quantity = _shuffle_quantity[(index + i) % _itemList.size()];
			_itemList[i]->currentFrameX = _itemList[i]->quantity;
			_itemList[i]->shuffle_index = (_itemList[i]->shuffle_index + 1) %_itemList.size();

			if (_itemList[i]->y == _currentItem.y)
			{
				if (_itemList[i]->kind == Crop_Kind::POTATO)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_potato");
					_currentItem.kind = Crop_Kind::POTATO;
				}

				if (_itemList[i]->kind == Crop_Kind::CABBAGE)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_cabbage");
					_currentItem.kind = Crop_Kind::CABBAGE;
				}

				if (_itemList[i]->kind == Crop_Kind::BEAN)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_bean");
					_currentItem.kind = Crop_Kind::BEAN;
				}

				if (_itemList[i]->kind == Crop_Kind::PUMPKIN)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_pumpkin");
					_currentItem.kind = Crop_Kind::PUMPKIN;
				}

				if (_itemList[i]->kind == Crop_Kind::HEART)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_heart");
					_currentItem.kind = Crop_Kind::HEART;
				}
				_currentItem.font = IMAGEMANAGER->findImage("UI_number");
				_currentItem.quantity = _itemList[i]->quantity;
				_currentItem.currentFrameX = _itemList[i]->quantity;
				_currentItem.currentFrameY = 0;
			}
		}
	}

	if (KEYMANAGER->isOnceKeyDown('E'))
	{
		index -= 1;

		currentItemIndex = (currentItemIndex - 1);

		if(currentItemIndex < 0) currentItemIndex = _itemList.size() - 1;

		if (index < 0) index = _itemList.size() - 1;

		for (int i = _itemList.size() - 1; i >= 0; i--)
		{
			_itemList[i]->img = _shuffle_img[abs(i + index) % _itemList.size()];
			_itemList[i]->kind = _shuffle_kind[abs(i + index) % _itemList.size()];
			_itemList[i]->quantity = _shuffle_quantity[abs(i + index) % _itemList.size()];
			_itemList[i]->currentFrameX = _itemList[i]->quantity;

			_itemList[i]->shuffle_index = _itemList[i]->shuffle_index - 1;
			if (_itemList[i]->shuffle_index < 0) _itemList[i]->shuffle_index = _itemList.size() - 1;

			if (_itemList[i]->y == _currentItem.y)
			{
				if (_itemList[i]->kind == Crop_Kind::POTATO)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_potato");
					_currentItem.kind = Crop_Kind::POTATO;
				}

				if (_itemList[i]->kind == Crop_Kind::CABBAGE)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_cabbage");
					_currentItem.kind = Crop_Kind::CABBAGE;
				}

				if (_itemList[i]->kind == Crop_Kind::BEAN)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_bean");
					_currentItem.kind = Crop_Kind::BEAN;
				}

				if (_itemList[i]->kind == Crop_Kind::PUMPKIN)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_pumpkin");
					_currentItem.kind = Crop_Kind::PUMPKIN;
				}

				if (_itemList[i]->kind == Crop_Kind::HEART)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_heart");
					_currentItem.kind = Crop_Kind::HEART;
				}
				_currentItem.font = IMAGEMANAGER->findImage("UI_number");
				_currentItem.quantity = _itemList[i]->quantity;
				_currentItem.currentFrameX = _itemList[i]->quantity;
				_currentItem.currentFrameY = 0;
			}
		}
	}
}

void UI::inventory_Quantity_check()
{
	_shuffle_quantity.clear();

	int sum = 0;
	for (int i = 0; i < _player->get_playerResource().inventory.size(); i++)
	{
		_shuffle_quantity.push_back(_player->get_playerResource().inventory[i]->get_quantity());
		sum += _player->get_playerResource().inventory[i]->get_quantity();
	}
	past_item = sum;

	for (int i = 0; i < _itemList.size(); i++)
	{
		_itemList[i]->quantity = _shuffle_quantity[_itemList[i]->shuffle_index];
		_itemList[i]->currentFrameX = _itemList[i]->quantity;
	}

	_currentItem.quantity = _shuffle_quantity[currentItemIndex];
	_currentItem.currentFrameX = _shuffle_quantity[currentItemIndex];
}

void UI::inventory_size_check()
{
	inventory_init();
}

void UI::clear()
{
	_heartList.clear();
	_gold.clear();
	_axeList.clear();
	_fertilizerList.clear();
	_itemList.clear();
	_shuffle_img.clear();
	_shuffle_kind.clear();
	_shuffle_quantity.clear();
}

void UI::inventory_init()
{
	index = 0;
	_itemList.clear();
	_shuffle_img.clear();
	_shuffle_quantity.clear();
	_shuffle_kind.clear();

	int sum = 0;
	for (int i = 0; i < _player->get_playerResource().inventory.size(); i++)
	{
		sum += _player->get_playerResource().inventory[i]->get_quantity();
	}

	past_item = sum;
	past_size = _player->get_playerResource().inventory.size();
	currentItemIndex = _player->get_playerResource().inventory.size() / 2;

	if (_player->get_playerResource().inventory.size() != 0)
	{
		int size = _player->get_playerResource().inventory.size();

		for (int i = 0; i < _player->get_playerResource().inventory.size(); i++)
		{
			Item *temp = _player->get_playerResource().inventory[i];
			
			UI_info* _item = new UI_info;

			_item->font = IMAGEMANAGER->findImage("UI_number");
			_item->quantity = _player->get_playerResource().inventory[i]->get_quantity();
			_item->currentFrameX = _player->get_playerResource().inventory[i]->get_quantity();
			_item->currentFrameY = 0;

			if (temp->get_seedKind() == Crop_Kind::POTATO)
			{
				_item->img = IMAGEMANAGER->findImage("seed_potato");
				_item->kind = Crop_Kind::POTATO;
			}

			if (temp->get_seedKind() == Crop_Kind::CABBAGE)
			{
				_item->img = IMAGEMANAGER->findImage("seed_cabbage");
				_item->kind = Crop_Kind::CABBAGE;
			}

			if (temp->get_seedKind() == Crop_Kind::BEAN)
			{
				_item->img = IMAGEMANAGER->findImage("seed_bean");
				_item->kind = Crop_Kind::BEAN;
			}

			if (temp->get_seedKind() == Crop_Kind::PUMPKIN)
			{
				_item->img = IMAGEMANAGER->findImage("seed_pumpkin");
				_item->kind = Crop_Kind::PUMPKIN;
			}

			if (temp->get_seedKind() == Crop_Kind::HEART)
			{
				_item->img = IMAGEMANAGER->findImage("seed_heart");
				_item->kind = Crop_Kind::HEART;
			}

			_shuffle_img.push_back(_item->img);
			_shuffle_kind.push_back(_item->kind);
			_shuffle_quantity.push_back(_item->quantity);
			_item->shuffle_index = i;
			_item->x = _currentItem.x;

			if ((int)(size / 2) - i >= 0)
				_item->y = _currentItem.y + (size / 2 - i) * _item->img->getHeight();
			else
				_item->y = _currentItem.y - _item->img->getHeight() + (size / 2 - i) * _item->img->getHeight();
			
			if (_item->y == _currentItem.y)
			{
				if (temp->get_seedKind() == Crop_Kind::POTATO)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_potato");
					_currentItem.kind = Crop_Kind::POTATO;
				}

				if (temp->get_seedKind() == Crop_Kind::CABBAGE)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_cabbage");
					_currentItem.kind = Crop_Kind::CABBAGE;
				}

				if (temp->get_seedKind() == Crop_Kind::BEAN)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_bean");
					_currentItem.kind = Crop_Kind::BEAN;
				}

				if (temp->get_seedKind() == Crop_Kind::PUMPKIN)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_pumpkin");
					_currentItem.kind = Crop_Kind::PUMPKIN;
				}

				if (temp->get_seedKind() == Crop_Kind::HEART)
				{
					_currentItem.img = IMAGEMANAGER->findImage("UI_heart");
					_currentItem.kind = Crop_Kind::HEART;
				}

				_currentItem.font = IMAGEMANAGER->findImage("UI_number");
				_currentItem.quantity = _player->get_playerResource().inventory[i]->get_quantity();
				_currentItem.currentFrameX = _player->get_playerResource().inventory[i]->get_quantity();
				_currentItem.currentFrameY = 0;
			}

			_item->rc = D2DMANAGER->RectMake(_item->x - _item->img->getWidth(), _item->y, _item->img->getWidth(), _item->img->getHeight());
			_itemList.push_back(_item);
		}
	}
	else
		_currentItem.img = NULL;

}
