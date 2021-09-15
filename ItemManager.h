#pragma once
#include "gameNode.h"
#include "Item.h"
#include "seed_potato.h"
#include "seed_cabbage.h"
#include "seed_bean.h"
#include "Pickaxe.h"
#include "fertilizer.h"
#include "SeedBag.h"
#include "seed_heart.h"
#include "heartbag.h"
#include "seed_pumpkin.h"
#include "gun.h"

class player;

class ItemManager
{
private:
	vector<Item*> _itemList;
	player* _player;
public:

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void set_potato(float x, float y);
	void set_bean(float x, float y);
	void set_cabbage(float x, float y);
	void set_heart(float x, float y);
	void set_pumpkin(float x, float y);
	void set_pickaxe(float x, float y);
	void set_fertilizer(float x, float y);
	void set_seedBag(float x, float y, int num);
	void set_heartBag(float x, float y, int num);
	void set_gun(float x, float y, int num);

	void randomItemDrop(float x, float y);

	inline void remove_item(int index) { _itemList.erase(_itemList.begin() + index); }
	void LinkPlayerClass(player* player) { _player = player; }

	vector<Item*>& get_itemList() { return _itemList; }
};

