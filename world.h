#pragma once
#include "gameNode.h"

class tool;
class tile;
class player;
class enemyManager;
class collision;
class CropManager;
class ObjectManager;
class ItemManger;
class UI;

class world : public gameNode
{
private:
	
	tool* _map;
	player* _player;
	enemyManager* _em;
	collision* _collision;
	CropManager* _crop;
	ObjectManager* _object;
	ItemManager* _item;
	UI* _UI;

	vector<tile*> _tiles;
	vector<int> _loopList;

	D2D1_POINT_2F center_pos;

	image* _loading;
	image* _blackScreen;
	image* _sunRise;
	image* _gameover;
	animation* _animation;

	float time_holder;
	float _count;
	float rgb_alpha;
	int day_time;
	int wave_time;

	bool start;
	bool return_home;
	bool sun_rise;
	bool sun_set;
	bool last_day;
	bool end;
	bool wave[3];

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void save();
	void load();
	void count_time();
	void set_enemies();

	void LinkToolClass(tool* tool) { _map = tool; }
	void LinkPlayerClass(player* player) { _player = player; }
	void LinkEnemyClass(enemyManager* em) { _em = em; }
	void LinkCollisionClass(collision* collision) { _collision = collision; }
	void LinkCropClass(CropManager* crop) { _crop = crop; }
	void LinkObjectClass(ObjectManager* object) { _object = object; }
	void LinkItemClass(ItemManager* item) { _item = item; }
	void LinkUIClass(UI* ui) { _UI = ui; }
};

