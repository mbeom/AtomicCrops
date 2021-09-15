#pragma once
#include "gameNode.h"
#include "animation.h"
#include "effect.h"
#include "bullets.h"

class ItemManager;

// 플레이어 상태
enum class Player_State
{
	IDLE,
	RUN,
	SHOVELING,
	DEAD,

	NUM
};

// 플레이어 방향
enum class Player_Direction
{
	LEFT,
	TOP,
	RIGHT,
	BOTTOM,

	LEFT_TOP,
	RIGHT_TOP,
	LEFT_BOTTOM,
	RIGHT_BOTTOM,

	DIRECTION_NUM
};

// 플레이어 위치 정보
struct Player_Info
{
	D2D1_RECT_F rc;
	image* img;
	animation* ani;

	float x, y;
	float width, height;
	float start_xPos, start_yPos;
	float angle;
	Player_Direction direction;
	Player_Direction moving_direction;
	Player_State state;

	float _playTime;
	float _startTime, _endTime;
};

// 플레이어 능력치 정보
struct Player_Status
{
	int current_hp;
	int max_hp;
	int max_water;

	int damage;
	float move_speed;

	float current_gauge;
	float max_gauge;
};

// 플레이어 자원 -> 마을에서 보이는 정보
struct Player_Resource
{
	int gold;
	int water;
	int pickaxe;
	int fertilizer;

	vector<Item*> inventory;
};

class player
{

private:
	Player_Info _player_head;
	Player_Info _player_body;
	Player_Info _player_Larm;
	Player_Info _player_Rarm;
	Player_Info _player_leg;

	D2D1_RECT_F _player_tile;

	Player_Status _status;
	Player_Resource _player_resource;
	GunType _player_gun;
	image* _gun;
	image* _progressBar;
	basic_bullet* _bullet;
	squirrel_bullet* _squirrel_bullet;

	float _bullet_angle;
	hitBox* _hitbox;
	animation* player_action;
	ItemManager* _item;

	D2D1_MATRIX_3X2_F scale_mat;
	D2D1_MATRIX_3X2_F rotation_mat;

	int _score;
	int interval[(int)Player_State::NUM];
	int _currentFrameX, _currentFrameY;
	bool current_direction[(int)Player_Direction::DIRECTION_NUM];
	float effect_interval = 0;
	float count, bullet_count;

	bool is_digging;
	bool effect_done;
	bool is_hit;
	bool is_complete;
	bool is_fired;
	bool in_world;
	bool end;

public:

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	// 플레이어 좌표, 렉트, 방향, 상태(IDLE, ATTACK) 정보 반환
	inline Player_Info get_playerInfo() { return _player_body; }

	// 플레이어 좌표, 렉트, 방향, 상태(IDLE, ATTACK) 정보 반환
	inline D2D1_RECT_F get_playerRect() { return _player_tile; }

	// 플레이어 스탯(레벨, HP, 공격력, 스피드 ....) 정보 반환
	inline Player_Status get_playerStatus() { return _status; }

	// 플레이어 소지금, 광석 정보 반환
	inline Player_Resource& get_playerResource() { return _player_resource; }

	inline basic_bullet* get_bullet() { return _bullet; }
	inline squirrel_bullet* get_squirrel() { return _squirrel_bullet; }

	inline hitBox* get_hitBox() { return _hitbox; }
	inline float get_currentCD() { return effect_interval; }
	inline int get_score() { return _score; }

	inline void set_state(Player_State val) 
	{ 
		if(val == Player_State::SHOVELING) 	is_digging = true;
		_player_body.state = val; 
	}
	inline Player_State get_state() { return _player_body.state; }

	inline void set_hp(int hp) 
	{
		_status.current_hp += hp; 
		if (_status.current_hp < 0)
			_status.current_hp = 0;
		if (_status.current_hp > _status.max_hp) 
			_status.current_hp = _status.max_hp; 
	}
	inline void set_Maxhp(int hp)
	{
		_status.max_hp += hp;
		if (_status.max_hp > 10)
			_status.max_hp = 10;
	}
	inline void set_hit(bool hit) { is_hit = hit; }
	inline void set_x(float x) { _player_body.x = x; }
	inline void set_y(float y) { _player_body.y = y; }
	inline void set_gunType(GunType type) { _player_gun = type; }

	inline void set_complete(bool val) { is_complete = val; }
	inline bool get_complete() { return is_complete; }

	inline void set_inWorld(bool val) { in_world = val; }
	inline bool get_inWorld() { return in_world; }

	inline void set_water(int val) { _player_resource.water = val; }
	inline int get_water() { return _player_resource.water; }

	inline void set_pickaxe(int val) { _player_resource.pickaxe += val; }
	inline void set_fertilizer(int val) { _player_resource.fertilizer += val; }

	inline int get_inventorySize()
	{
		int size = 0;
		for (int i = 0; i < _player_resource.inventory.size(); i++)
		{
			size += _player_resource.inventory[i]->get_quantity();
		}
		return size;
	}

	inline void set_rect()
	{
		//float x = (_player_tile.left + _player_tile.right) / 2;
		//float y = (_player_tile.top + _player_tile.bottom) / 2;

		_player_body.rc = D2DMANAGER->RectMakeCenter(_player_body.x, _player_body.y, _player_body.width, _player_body.height);
		_player_tile = D2DMANAGER->RectMakeCenter(_player_body.x, _player_body.y, 44, 44);
	}
	inline void put_rect(D2D1_RECT_F temp) { _player_body.rc = temp; }

	void player_init();
	void item_init();
	void fix_bodySet();
	void animation_setting();
	void gun_update();

	// 캐릭터가 바라보는 방향을 마우스 쪽으로 업데이트 해주는 함수
	void update_direction();
	
	// 공격이 끝날때까지 캐릭터를 기다리게 만드는 함수
	void wait_process();

	// 각 상태마다 다른 행동을 하기위해 함수로 나눔
	void key_setting_idle();
	void key_setting_run();
	void key_setting_shoveling();

	//구매시
	void set_gold(int gold) { _player_resource.gold += gold; 
	if (gold > 0) _score += gold;
	}

	void LinkItemClass(ItemManager* item) { _item = item; }

};

