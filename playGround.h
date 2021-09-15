#pragma once
#include "gameNode.h"
#include "tool.h"
#include "base.h"
#include "world.h"
#include "player.h"
#include "enemyManager.h"
#include "collision.h"
#include "CropManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "UI.h"
#include "title.h"

class playGround : public gameNode
{
private:
	tool* _tool;
	image* _test;
	base* _base;
	world* _world;
	player* _player;
	enemyManager* _em;
	collision* _collision;
	CropManager* _crop;
	ObjectManager* _object;
	ItemManager* _item;
	UI* _UI;
	title* _title;

public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};

