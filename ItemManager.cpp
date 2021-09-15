#include "stdafx.h"
#include "ItemManager.h"
#include "player.h"

HRESULT ItemManager::init()
{
	return S_OK;
}

void ItemManager::release()
{
}

void ItemManager::update()
{
	for (int i = 0; i < _itemList.size(); i++)
	{
		if (_itemList[i]->get_itemObject() != NULL) _itemList[i]->update();

		ItemObject* _item = _itemList[i]->get_itemObject();

		if (_item != NULL && getDistance(_item->x, _item->y, _player->get_playerInfo().x, _player->get_playerInfo().y) < 100)
		{
			float angle = getAngle(_item->x, _item->y, _player->get_playerInfo().x, _player->get_playerInfo().y);
			_itemList[i]->set_taken(true);
			_itemList[i]->set_angle(angle);
		}
	}
}

void ItemManager::render()
{
	for (int i = 0; i < _itemList.size(); i++)
	{
		_itemList[i]->render();
	}
}

void ItemManager::set_potato(float x, float y)
{
	Item* _item = new seed_potato;
	_item->init(x, y, false);

	_itemList.push_back(_item);
}

void ItemManager::set_bean(float x, float y)
{
	Item* _item = new seed_bean;
	_item->init(x, y, false);

	_itemList.push_back(_item);
}

void ItemManager::set_cabbage(float x, float y)
{
	Item* _item = new seed_cabbage;
	_item->init(x, y, false);

	_itemList.push_back(_item);
}

void ItemManager::set_heart(float x, float y)
{
	Item* _item = new seed_heart;
	_item->init(x, y, false);

	_itemList.push_back(_item);
}

void ItemManager::set_pumpkin(float x, float y)
{
	Item* _item = new seed_pumpkin;
	_item->init(x, y, false);

	_itemList.push_back(_item);
}

void ItemManager::set_pickaxe(float x, float y)
{
	Item* _item = new Pickaxe;
	_item->init(x, y, false);

	_itemList.push_back(_item);
}

void ItemManager::set_fertilizer(float x, float y)
{
	Item* _item = new fertilizer;
	_item->init(x, y, false);

	_itemList.push_back(_item);
}

void ItemManager::set_seedBag(float x, float y, int num)
{
	Item* _item = new SeedBag;
	_item->init(x, y, num);

	_itemList.push_back(_item);
}

void ItemManager::set_heartBag(float x, float y, int num)
{
	Item* _item = new HeartBag;
	_item->init(x, y, num);

	_itemList.push_back(_item);
}

void ItemManager::set_gun(float x, float y, int num)
{
	Item* _item = new gun;
	_item->init(x, y, num);

	_itemList.push_back(_item);
}

void ItemManager::randomItemDrop(float x, float y)
{
	int num = RND->getFromIntTo(0, 10);
	int dropRate = RND->getFromIntTo(0, 11);

	if (dropRate % 2 == 0)
	{
		if (num < 6)
		{
			set_fertilizer(x, y);
		}
		else if (num == 6)
		{
			set_potato(x, y);
		}
		else if (num == 7)
		{
			set_bean(x, y);
		}
		else if (num == 8)
		{
			set_cabbage(x, y);
		}
		else
			set_pickaxe(x, y);
	}
}
