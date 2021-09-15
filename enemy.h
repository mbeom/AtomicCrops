#pragma once
#include "gameNode.h"
#include "bullets.h"

enum class enemy_direction : int
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

enum class enemy_state : int
{
	IDLE,
	IDLE_SLEEP,
	IDLE_FIND,
	HURT,
	SLEEP_HURT,
	PATROL,
	RUN,
	ATTACK,
	DEAD,

	STATE_NUM
};

enum class enemy_kind : int
{
	WORM,
	RABBIT,
	MAPLE,
	SCORPION,
	BOSS,

	KIND_NUM
};

class enemy : public gameNode
{
protected:
	image* enemy_img;		//에너미 이미지
	animation* enemy_body;
	basic_bullet* _bullet;
	Boss_bullet* _bossBullet;

	D2D1_RECT_F _rc;
	D2D1_RECT_F _destination;

	vector<D2D1_RECT_F> _list;

	int _currentFrameX;		//프레임번호로 사용할 변수
	int _currentFrameY;

	int _count;				//프레임렌더링 용

	int _fireCount;			//발사 카운트 용
	int _Fire_interval;			//발사 주기

	enemy_direction _past_direction;
	enemy_direction _direction;
	enemy_direction _moving_direction;

	enemy_state _state;
	enemy_kind _kind;

	float max_hp, hp;

	float _x, _y;
	float _width;
	float _height;

	float _speed;
	float _angle;

	float _frameUpdateSec;
	float _elapsedSec;

	int _index;
	int _groupNumber;

	bool is_find;
	bool is_arrived;
	bool is_done;
	bool is_hit;
	bool is_wall;
	bool is_dead;

public:
	enemy() {};
	~enemy() {};

	virtual HRESULT init();
	virtual HRESULT init(float x, float y);
	virtual HRESULT init(float x, float y, bool is_find);

	virtual void release();
	virtual void update();
	virtual void render();
	virtual void move();
	virtual bool bullet_Fire();

	void draw();

	inline void set_x(float x) { _x = x; }
	inline void set_y(float y) { _y = y; }
	inline float get_x() { return _x; }
	inline float get_y() { return _y; }

	inline basic_bullet* get_basic_bullet() { return _bullet; }
	inline Boss_bullet* get_BossBullet() { return _bossBullet; }

	inline vector<D2D1_RECT_F>& get_list() { return _list; }
	inline void vector_clear() { _list.clear(); }

	inline void set_angle(float val) { _angle = val; }
	inline void set_index(int val) { _index = val; }

	inline void set_hit(bool val) { is_hit = val; }
	inline bool get_hit() { return is_hit; }

	inline void set_done(bool val) { is_done = val; }
	inline bool get_done() { return is_done; }

	inline int get_groupNumber() { return _groupNumber; }
	inline bool get_dead() { return is_dead; }

	inline void set_arrived(bool val) { is_arrived = val; }
	inline bool get_arrived() { return is_arrived; }

	inline void set_find_player(bool val) { is_find = val; };
	inline bool get_find_player() { return is_find; };

	inline void set_isWall(bool val) { is_wall = val; }
	inline bool get_isWall() { return is_wall; }

	inline void set_destination(D2D1_RECT_F player_pos) { _destination = player_pos; }

	inline enemy_kind get_kind() { return _kind; }

	inline void set_hp(float val) { hp += val; }
	inline float get_hp() { return hp; }

	inline enemy_direction get_direction() { return _direction; }
	inline enemy_direction get_movingDirection() { return _moving_direction; }

	inline D2D1_RECT_F get_rect() { return _rc; }
	inline void set_rect() { _rc = D2DMANAGER->RectMakeCenter(_x, _y, 44, 44); }

	inline void set_left(float val) {
		_rc.left = val;
		_rc.right = _rc.left + _width;
	}
	inline void set_top(float val)
	{
		_rc.top = val;
		_rc.bottom = _rc.top + _height;
	}
	inline void set_right(float val)
	{
		_rc.right = val;
		_rc.left = _rc.right - _width;
	}
	inline void set_bottom(float val)
	{
		_rc.bottom = val;
		_rc.top = _rc.bottom - _height;
	}

};

