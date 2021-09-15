#pragma once
#include "gameNode.h"
#include <vector>

enum class bullet_user
{
	PLAYER,
	ENEMY,

	USER_NUM
};

struct tagBullet
{
	image* bulletImage;		//총알 이미지 
	D2D1_RECT_F rc;				//총알 렉트
	float x, y;				//총알 좌표(실시간)
	float angle;			//총알 각도
	float interval, interval2;			//총알 반지름
	float speed;			//총알 스피드
	float width;
	float height;
	float jump_power;
	float gravity;
	float opacity;
	float fireX, fireY;		//총알 발사시 처음 좌표(발사된 좌표)
	bool isFire;			//발사됐누?
	bool isGround;
	bool is_hit;
	int count;				//총알 프레임카운트용
	int direction;
	int durability;
	int currentFrameX;
	int currentFrameY;
};

/******************************************* 에너미 불렛 *****************************************************/
class hitBox : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;

public:
	hitBox() {};
	~hitBox() {};

	HRESULT init();
	void release();
	void update();
	void render();

	void fire(float x, float y, float width, float height);

	void move();

	//총알을 지워달라고 소통하는 함수
	void removeBullet(int arrNum);


	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }

};

class basic_bullet : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;
	float _range;
	int _bulletMax;
	bullet_user user;

public:
	basic_bullet() {};
	~basic_bullet() {};

	HRESULT init(float range);
	HRESULT init(float range, bullet_user val);
	void release();
	void update();
	void render();

	void fire(LPCWSTR imagename, float x, float y, float width, float height, float angle, float speed);

	void move();

	//총알을 지워달라고 소통하는 함수
	void removeBullet(int arrNum);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }

};

class Boss_bullet : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;
	float _range;
	int _bulletMax;

public:
	Boss_bullet() {};
	~Boss_bullet() {};

	HRESULT init(float range);
	void release();
	void update();
	void render();

	void fire(LPCWSTR imagename, float x, float y, float width, float height, int frameX, int frameY, float angle, float speed);

	void move();

	//총알을 지워달라고 소통하는 함수
	void removeBullet(int arrNum);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }

};

class squirrel_bullet : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;
	float _range;
	int _bulletMax;

public:
	squirrel_bullet() {};
	~squirrel_bullet() {};

	HRESULT init(float range);
	void release();
	void update();
	void render();

	void fire(LPCWSTR imagename, float x, float y, float width, float height, float angle, float speed);

	void move();

	//총알을 지워달라고 소통하는 함수
	void removeBullet(int arrNum);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }

};

class harvest_effect : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;
	float _range;
	int _bulletMax;
	image* _img;
	effect* _effect;

public:
	harvest_effect() {};
	~harvest_effect() {};

	HRESULT init(image* img);
	void release();
	void update();
	void render();

	void fire(float x, float y);

	void move();

	//총알을 지워달라고 소통하는 함수
	void removeBullet(int arrNum);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }

};

class font_effect : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;
	float _range;
	int _bulletMax;

public:
	font_effect() {};
	~font_effect() {};

	HRESULT init();
	void release();
	void update();
	void render();

	void fire(float x, float y, int price);

	void move();

	//총알을 지워달라고 소통하는 함수
	void removeBullet(int arrNum);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }

};