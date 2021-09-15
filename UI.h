#pragma once
#include "gameNode.h"

class player;

struct UI_info
{
	float x, y;
	int currentFrameX, currentFrameY;
	image* img;
	image* font;
	D2D1_RECT_F rc;
	Crop_Kind kind;
	int quantity;
	int shuffle_index;
};

class UI : public gameNode
{
private:

	player* _player;

	vector<UI_info*> _heartList;
	vector<UI_info*> _gold;
	vector<UI_info*> _axeList;
	vector<UI_info*> _fertilizerList;
	vector<UI_info*> _itemList;
	vector<image*> _shuffle_img;
	vector<Crop_Kind> _shuffle_kind;
	vector<int> _shuffle_quantity;

	UI_info _gold_icon;
	UI_info _pickaxe_icon;
	UI_info _fertilizer_icon;
	UI_info _water_icon;
	UI_info _water_indicator[3];

	UI_info _timeBackground;
	UI_info _daytime;
	UI_info _calender;
	UI_info _day;

	UI_info _currentItem;
	UI_info _mouse;

	int index;
	int currentItemIndex;
	int currentFrameX, currentFrameY;
	int past_heart, past_gold, past_axe, past_fertilizer, past_item, past_size;

	float center_x, center_y;
	float _count;

	bool start;

public:

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void LinkPlayerClass(player* player) { _player = player; }
	void max_heart_count();
	void heart_count();
	void gold_count();
	void axe_count();
	void fertilizer_count();
	void inventory_move();
	void inventory_init();
	void inventory_Quantity_check();
	void inventory_size_check();
	void clear();

	inline int get_currentItemIndex() { return currentItemIndex; }
	UI_info get_timeBackGround() { return _timeBackground; }
	UI_info get_currentItem() { return _currentItem; }
};

