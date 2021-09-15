#pragma once
#include "gameNode.h"
#include "enemy.h"
#include "rabbit.h"
#include "maple.h"
#include "scorpion.h"
#include "boss.h"
#include <vector>

class tool;
class tile;
class player;

class enemyManager : public gameNode
{
private:
	typedef vector<enemy*>				Enemy_pool;
	typedef vector<enemy*>::iterator	pool_iter;

private:
	Enemy_pool _enemy_pool;
	pool_iter _pool_iter;
	player* _player;
	tool* _map;

	int current_index;
	int past_index;
	
	vector<int> reset_list;
	vector<int> zone_num;
	list<int> group_num;

	vector<tile*>			_vTotalList;
	vector<tile*>::iterator _viTotalList;

	//갈 수 있는 타일 담을 벡터
	vector<tile*>			_vOpenList;
	vector<tile*>::iterator _viOpenList;

	//가장 가까운 경로를 담을 벡터
	vector<tile*>			_vCloseList;
	vector<tile*>::iterator _viCloseList;

	tile* _startTile;
	tile* _endtile;
	tile* _currentTile;

	D2D1_RECT_F temp;
	D2D1_RECT_F destination;

	float update_count;
	bool _start;
	bool _FindPath;

public:
	enemyManager() {};
	~enemyManager() {};

	virtual HRESULT init(int num);
	virtual void release();
	virtual void update();
	virtual void render();

	void set_rabbits(float x, float y);
	void set_maple(float x, float y);
	void set_scorpion(float x, float y);
	void set_boss(float x, float y);
	void set_rabbits(float x, float y, int groupCode);
	void set_rabbits(float x, float y, bool is_find);

	inline void LinkPlayerClass(player* player) { _player = player; }
	inline void LinkToolClass(tool* tool) { _map = tool; }

	vector<tile*> addOpenList(tile* currentTile);
	void pathFinder(tile* currentTile);

	void find_path();
	void find_player();
	void boss_only();
	void reset();
	void remove_enemy(int num);
	void check_zoneEnemy();

	POINT PointMake(int x, int y);
	vector<enemy*>& get_enemy() { return _enemy_pool; }


};

