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
	image* bulletImage;		//�Ѿ� �̹��� 
	D2D1_RECT_F rc;				//�Ѿ� ��Ʈ
	float x, y;				//�Ѿ� ��ǥ(�ǽð�)
	float angle;			//�Ѿ� ����
	float interval, interval2;			//�Ѿ� ������
	float speed;			//�Ѿ� ���ǵ�
	float width;
	float height;
	float jump_power;
	float gravity;
	float opacity;
	float fireX, fireY;		//�Ѿ� �߻�� ó�� ��ǥ(�߻�� ��ǥ)
	bool isFire;			//�߻�ƴ�?
	bool isGround;
	bool is_hit;
	int count;				//�Ѿ� ������ī��Ʈ��
	int direction;
	int durability;
	int currentFrameX;
	int currentFrameY;
};

/******************************************* ���ʹ� �ҷ� *****************************************************/
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

	//�Ѿ��� �����޶�� �����ϴ� �Լ�
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

	//�Ѿ��� �����޶�� �����ϴ� �Լ�
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

	//�Ѿ��� �����޶�� �����ϴ� �Լ�
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

	//�Ѿ��� �����޶�� �����ϴ� �Լ�
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

	//�Ѿ��� �����޶�� �����ϴ� �Լ�
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

	//�Ѿ��� �����޶�� �����ϴ� �Լ�
	void removeBullet(int arrNum);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }

};