#pragma once
#include "gameNode.h"
#include "effect.h"
#include "bullets.h"

class player;
class tool;
class tile;
class enemyManager;
class CropManager;
class ObjectManager;
class UI;
class ItemManager;

class collision
{
private:
	//충돌 객체
	player* _player;
	tool *_map;
	enemyManager* _enemy;
	CropManager* _crop;
	ObjectManager* _object;
	UI* _UI;
	ItemManager* _item;

	D2D1_RECT_F temp;
	D2D1_RECT_F current_tile_rect;
	D2D1_RECT_F _farm;

	int tileIndex[2];
	int tileIndex2[3];

	tile* tile_onMouse;
	tile* current_tile;
	tile* left_tile;
	tile* right_tile;
	tile* top_tile;
	tile* bottom_tile;
	
	image* _indicator;
	font_effect* _fontEffect;

	float x, y;
	int mapTileX, mapTileY;

	//연산변수
	bool on_tile;

public:
	collision() {};
	~collision() {};

	// 맵 정보를 받아오기 위한 init 함수 
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	//Link용
	inline void Link_PlayerClass(player* player) { _player = player; }                                     
	inline void Link_ToolClass(tool* map) { _map = map; }
	inline void Link_enemyClass(enemyManager* _em) { _enemy = _em; }
	inline void Link_CropClass(CropManager* crop) { _crop = crop; }
	inline void Link_ObjectClass(ObjectManager* object) { _object = object; }
	inline void Link_UIClass(UI* ui) { _UI = ui; }
	inline void LinkItemClass(ItemManager* item) { _item = item; }

	// 충돌관리 함수
	void player_AND_CrossTiles();
	void player_AND_diagonalTiles();
	void player_AND_tile();
	void player_AND_enemy();
	void player_AND_object();
	void player_AND_crops();
	void player_AND_Item();

	void enemy_AND_CrossTiles();
	void enemy_AND_diagonalTiles();

};


