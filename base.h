#pragma once
#include "gameNode.h"

class tool;
class tile;
class ItemManager;
class player;
class ObjectManager;
class collision;
class UI;

class base : public gameNode
{
private:
	tool* _tool;
	ItemManager* _item;
	vector<tile*> _map;
	player* _player;
	ObjectManager* _object;
	collision* _collision;
	UI* _ui;

	image* _loading;
	image* _blackScreen;
	image* _scoreBackground;
	animation* _animation;

	int _score;
	int _maxScore;
	float _count;
	bool start;

public:

	virtual HRESULT init();
	virtual void release();
	virtual void render();
	virtual void update();

	void load();

	void LinkToolClass(tool* tool) { _tool = tool; }
	void LinkItemClass(ItemManager* item) { _item = item; }
	void LinkPlayerClass(player* player) { _player = player; }
	void LinkObjectClass(ObjectManager* object) { _object = object; }
	void LinkCollisionClass(collision* collision) { _collision = collision; }
	void LinkUIClass(UI* ui) { _ui = ui; }
};

