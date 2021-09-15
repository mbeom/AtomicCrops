#include "stdafx.h"
#include "player.h"
#include "Item.h"

HRESULT player::init()
{
	IMAGEMANAGER->addFrameImage("arm_gun", L"image/player/arm_gun.png", 256, 128, 5, 4);
	IMAGEMANAGER->addFrameImage("death", L"image/player/death.png", 924, 792, 8, 6);
	IMAGEMANAGER->addFrameImage("leg", L"image/player/leg.png", 256, 256, 4, 4);
	IMAGEMANAGER->addFrameImage("pickaxe", L"image/player/arm_pick.png", 44, 44, 1, 1);
	IMAGEMANAGER->addImage("basic_gun", L"image/gun/basic_gun.png", 46, 18);
	IMAGEMANAGER->addFrameImage("progressBar", L"image/UI/ProgressBar.png", 510, 224, 5, 7);

	EFFECTMANAGER->addEffect("dead", "death", 924, 792, 132, 132, 3.0f, 0.05f, 1);

	player_init();
	item_init();

	for (int i = 0; i < (int)Player_State::NUM; i++)
	{
		interval[i] = 0;
	}

	_player_Larm.state = Player_State::IDLE;
	_player_Larm.direction = Player_Direction::RIGHT;
	_player_Rarm.state = Player_State::IDLE;
	_player_Rarm.direction = Player_Direction::RIGHT;

	_player_body.direction = Player_Direction::RIGHT;
	_player_head.direction = Player_Direction::RIGHT;

	_player_body.moving_direction = Player_Direction::RIGHT;

	_gun = IMAGEMANAGER->findImage("basic_gun");

	_bullet = new basic_bullet;
	_bullet->init(600, bullet_user::PLAYER);

	_squirrel_bullet = new squirrel_bullet;
	_squirrel_bullet->init(800);

	_progressBar = new image;
	_progressBar = IMAGEMANAGER->findImage("progressBar");

	_currentFrameX = _currentFrameY = 0;

	is_digging = false;
	effect_done = true;
	is_hit = false;
	end = false;

	effect_interval = 0;
	bullet_count = 0.3f;
	count = 0;

	return S_OK;
}

void player::release()
{
	delete player_action;
}

void player::update()
{
	if (!end)
	{
		Player_Direction temp = _player_body.direction;

		if(_player_body.state != Player_State::SHOVELING)
			update_direction();

		// 상태 패턴처럼 각 상태에는 다른 행동만 할수있도록 함수로 나눠서 업데이트에서 실행함.
		switch (_player_body.state)
		{
		case Player_State::IDLE:
			key_setting_idle();
			break;

		case Player_State::RUN:
			key_setting_run();
			break;

		case Player_State::SHOVELING:
			key_setting_shoveling();
			break;
		}
		fix_bodySet();

		wait_process();

		// 행동이 끝났다면, 마우스가 있는 방향을 쳐다보도록 애니매이션 업데이트
		if (_player_body.direction != temp)
		{
			animation_setting();
		}

		for (int i = 0; i < _player_resource.inventory.size(); i++)
		{
			if (_player_resource.inventory[i]->get_quantity() == 0)
			{
				_player_resource.inventory.erase(_player_resource.inventory.begin() + i);
			}
		}
	}

	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		_status.current_hp -= 1;
	}

	if (_status.current_hp <= 0 && !end)
	{
		_player_body.state == Player_State::DEAD;
		EFFECTMANAGER->play("dead", _player_tile.left, _player_tile.top);
		end = true;
	}

	// 이펙트 업데이트
	gun_update();
	KEYANIMANAGER->update();
}

void player::render()
{
	if(!end)
	if (_player_body.direction == Player_Direction::RIGHT)
	{
		CAMERAMANAGER->scale_mat = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1.5f, 1.5f), CAMERAMANAGER->center_pos);

		CAMERAMANAGER->center_pos.x = _player_Rarm.rc.left - CAMERAMANAGER->get_camera_x() + _player_Rarm.img->getFrameWidth() / 2;
		CAMERAMANAGER->center_pos.y = _player_Rarm.rc.top - CAMERAMANAGER->get_camera_y() + _player_Rarm.img->getFrameHeight() / 2;
		CAMERAMANAGER->rotation_mat = D2D1::Matrix3x2F::Rotation(-_player_Rarm.angle * 180 / PI, CAMERAMANAGER->center_pos);

		D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->scale_mat * CAMERAMANAGER->rotation_mat);

		_player_Rarm.img->FrameRender(_player_Rarm.x, _player_Rarm.y, 0, 0);

		D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());


		D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->scale_mat);

		_player_head.img->render(_player_head.x, _player_head.y);
		_player_body.img->render(_player_body.x, _player_body.y);
		_player_leg.img->aniRender(_player_leg.x, _player_leg.y, _player_leg.ani);

		D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		CAMERAMANAGER->center_pos.x = _player_Larm.rc.left - CAMERAMANAGER->get_camera_x() + _player_Larm.img->getFrameWidth() / 2;
		CAMERAMANAGER->center_pos.y = _player_Larm.rc.top - CAMERAMANAGER->get_camera_y() + _player_Larm.img->getFrameHeight() / 2;
		CAMERAMANAGER->rotation_mat = D2D1::Matrix3x2F::Rotation(-_player_Larm.angle * 180 / PI, CAMERAMANAGER->center_pos);

		D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->scale_mat * CAMERAMANAGER->rotation_mat);
		if(_player_body.state != Player_State::SHOVELING)
			_gun->render(_player_body.x, _player_body.rc.bottom);
		_player_Larm.img->FrameRender(_player_Larm.x, _player_Larm.y, 0, 0);

		D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	}
	else
	{
		CAMERAMANAGER->scale_mat = D2D1::Matrix3x2F::Scale(D2D1::SizeF(-1.5f, 1.5f), CAMERAMANAGER->center_pos);
		CAMERAMANAGER->center_pos.x = _player_Larm.rc.left - CAMERAMANAGER->get_camera_x() + _player_Larm.img->getFrameWidth() / 2;
		CAMERAMANAGER->center_pos.y = _player_Larm.rc.top - CAMERAMANAGER->get_camera_y() + _player_Larm.img->getFrameHeight() / 2;
		CAMERAMANAGER->rotation_mat = D2D1::Matrix3x2F::Rotation(-180 - _player_Larm.angle * 180 / PI, CAMERAMANAGER->center_pos);

		D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->scale_mat * CAMERAMANAGER->rotation_mat);

		_player_Rarm.img->FrameRender(_player_Rarm.x, _player_Rarm.y, 0, 0);

		D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->scale_mat);

		_player_head.img->render(_player_head.x, _player_head.y);
		_player_body.img->render(_player_body.x, _player_body.y);
		_player_leg.img->aniRender(_player_leg.x, _player_leg.y, _player_leg.ani);

		D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		CAMERAMANAGER->center_pos.x = _player_Larm.rc.left - CAMERAMANAGER->get_camera_x() + _player_Larm.img->getFrameWidth() / 2;
		CAMERAMANAGER->center_pos.y = _player_Larm.rc.top - CAMERAMANAGER->get_camera_y() + _player_Larm.img->getFrameHeight() / 2;
		CAMERAMANAGER->rotation_mat = D2D1::Matrix3x2F::Rotation(-180 - _player_Larm.angle * 180 / PI, CAMERAMANAGER->center_pos);

		D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->scale_mat * CAMERAMANAGER->rotation_mat);
		if (_player_body.state != Player_State::SHOVELING)
			_gun->render(_player_body.x, _player_body.rc.bottom);
		_player_Larm.img->FrameRender(_player_Larm.x, _player_Larm.y, 0, 0);

		D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	}
	CAMERAMANAGER->scale_mat = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1.5f, 1.5f), CAMERAMANAGER->center_pos);
	D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->scale_mat);
	_squirrel_bullet->render();

	if (_player_body.direction == Player_Direction::RIGHT && is_digging)
	{
		_progressBar->FrameRender(_player_head.x - 35, _player_head.y - 30, _currentFrameX, _currentFrameY);
	}
	else if(_player_body.direction == Player_Direction::LEFT && is_digging)
		_progressBar->FrameRender(_player_head.x - 50, _player_head.y - 30, _currentFrameX, _currentFrameY);
	D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	
	_bullet->render();

	/*
	D2DMANAGER->Rectangle(_player_tile);

	WCHAR str[128];
	swprintf_s(str, L"Direction is : %.1f,  %.1f", (float)TIMEMANAGER->day, (float)_player_tile.top);
	D2DMANAGER->DrawTextD2D(D2DMANAGER->defaultBrush, L"메이플스토리", 17.0f, str, _player_body.rc.left - 40, _player_body.rc.top - 40, _player_body.rc.left + 100, _player_body.rc.top + 100);
	*/
}


// 마우스 위치에 따라 플레이어가 바라보는 방향을 바꾸는 함수
void player::update_direction()
{
	_player_body.angle = getAngle(_player_body.x, _player_body.y, _ptMouse.x, _ptMouse.y);

	if (_player_body.angle >= PI2) _player_body.angle -= PI2;
	if (_player_body.angle <= 0) _player_body.angle += PI2;

	if (_player_body.angle > 0 && _player_body.angle <= PI/2)
	{
		_player_body.direction = Player_Direction::RIGHT;
	}
	else if (_player_body.angle > PI/2 && _player_body.angle <= PI)
	{
		_player_body.direction = Player_Direction::LEFT;
	}
	else if (_player_body.angle > PI && _player_body.angle <= 3*PI/2)
	{
		_player_body.direction = Player_Direction::LEFT;
	}
	else
	{
		_player_body.angle -= PI2;
		_player_body.direction = Player_Direction::RIGHT;
	}

	if (_player_body.angle <= -0.8) _player_body.angle = -0.8;
	if (_player_body.angle >= 3.7) _player_body.angle = 3.7;

	_bullet_angle = _player_body.angle;

	_player_Rarm.angle = _player_body.angle;
	_player_Larm.angle = _player_Rarm.angle;
}

// 플레이어의 행동이 끝나는지 안끝나는지 불값을 받는 함수
void player::wait_process()
{
	if (_player_body.state == Player_State::SHOVELING)
	{
		count += TIMEMANAGER->getElapsedTime();

		_progressBar->setFrameX(_currentFrameX);
		_progressBar->setFrameY(_currentFrameY);
	}

	if (count > 0.05f)
	{
		_currentFrameX += 1;
		count = 0;
	}

	if (_currentFrameX > _progressBar->getMaxFrameX())
	{
		_currentFrameY += 1;
	}

	if (_currentFrameX > _progressBar->getMaxFrameX() && _currentFrameY <= _progressBar->getMaxFrameY())
	{
		_currentFrameX = 0;
	}
	else if (_currentFrameX > _progressBar->getMaxFrameX() && _currentFrameY > _progressBar->getMaxFrameY())
	{
		_currentFrameX = 0;
		_currentFrameY = 0;

		is_digging = false;
		is_complete = true;

		_player_body.state = Player_State::IDLE;
		animation_setting();
	}
}

void player::player_init()
{
	_player_body.img = new image;
	_player_body.img->D_Init(L"image/player/body.png", 20, 13);
	//_player_body.x = 3952;
	//_player_body.y = 1744;
	_player_body.width = _player_body.img->getWidth();
	_player_body.height = _player_body.img->getHeight();
	_player_body.rc = D2DMANAGER->RectMakeCenter(_player_body.x, _player_body.y, _player_body.width, _player_body.height);
	_player_tile = D2DMANAGER->RectMakeCenter(_player_body.x, _player_body.y, 44, 44);

	_player_head.img = new image;
	_player_head.img->D_Init(L"image/player/head.png", 34, 32);
	_player_head.x = _player_body.x - 10;
	_player_head.y = _player_body.y - 32;
	_player_head.width = _player_head.img->getWidth();
	_player_head.height = _player_head.img->getHeight();
	_player_head.rc = D2DMANAGER->RectMakeCenter(_player_head.x, _player_head.y, _player_head.width, _player_head.height);

	_player_Larm.img = IMAGEMANAGER->findImage("arm_gun");
	_player_Larm.width = _player_Larm.img->getFrameWidth();
	_player_Larm.height = _player_Larm.img->getFrameHeight();
	_player_Larm.x = _player_body.rc.left + 2;
	_player_Larm.y = _player_body.rc.top - 10;
	_player_Larm.rc = D2DMANAGER->RectMakeCenter(_player_Larm.x, _player_Larm.y, _player_Larm.width, _player_Larm.height);

	_player_Rarm.img = IMAGEMANAGER->findImage("arm_gun");
	_player_Rarm.width = _player_Rarm.img->getFrameWidth();
	_player_Rarm.height = _player_Rarm.img->getFrameHeight();
	_player_Rarm.x = _player_body.rc.right - 2;
	_player_Rarm.y = _player_body.rc.top - 10;
	_player_Rarm.rc = D2DMANAGER->RectMakeCenter(_player_Rarm.x, _player_Rarm.y, _player_Rarm.width, _player_Rarm.height);

	_player_leg.img = IMAGEMANAGER->findImage("leg");
	_player_leg.width = _player_leg.img->getFrameWidth();
	_player_leg.height = _player_leg.img->getFrameHeight();
	_player_leg.x = _player_body.rc.left - 16;
	_player_leg.y = _player_body.rc.top - 5;
	_player_leg.rc = D2DMANAGER->RectMakeCenter(_player_leg.x, _player_leg.y, _player_leg.width, _player_leg.height);

	_status.current_hp = 5; 
	_status.max_hp = 5;
	_status.max_water = 8;
	_status.move_speed = 4.0f;
	_status.current_gauge = 0;
	_status.max_gauge = 3.0f;
	_status.damage = 20;

	_player_resource.gold = 500;
	_player_resource.pickaxe = 2;

	int right_idle_arm[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("right_idle_arm", "arm_gun", right_idle_arm, 1, 4.0, true);

	int right_run_arm[] = { 0, 1, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("right_run_arm", "arm_gun", right_run_arm, 4, 4.0, true);

	int right_idle_leg[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("right_idle_leg", "leg", right_idle_leg, 1, 10.0, true);

	int right_run_leg[] = { 2, 3, 4, 5, 6, 7, 8, 9 };
	KEYANIMANAGER->addArrayFrameAnimation("right_run_leg", "leg", right_run_leg, 8, 10.0, true);

	int right_pixaxe[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("right_pixaxe", "pickaxe", right_idle_arm, 1, 4.0, true);

	int death[43];
	for (int i = 0; i < 43; i++)
	{
		death[i] = i;
	}
	KEYANIMANAGER->addArrayFrameAnimation("dead", "death", death, 43, 4.0, true);

	_player_Larm.ani = KEYANIMANAGER->findAnimation("right_idle_arm");
	_player_Rarm.ani = KEYANIMANAGER->findAnimation("right_idle_arm");
	_player_leg.ani = KEYANIMANAGER->findAnimation("right_idle_leg");

	_player_Larm.ani->start();
	_player_Rarm.ani->start();
	_player_leg.ani->start();
}

void player::item_init()
{
	_player_resource.inventory.clear();

	_player_gun = GunType::BASIC_GUN;

	Item* _potato = new seed_potato;
	_potato->init();
	_potato->set_quantity(4);

	Item* _cabbage = new seed_cabbage;
	_cabbage->init();
	_cabbage->set_quantity(4);

	Item* _bean = new seed_bean;
	_bean->init();
	_bean->set_quantity(4);

	_player_resource.inventory.push_back(_potato);
	_player_resource.inventory.push_back(_cabbage);
	_player_resource.inventory.push_back(_bean);

}

void player::fix_bodySet()
{
	_player_head.x = _player_body.x - 10;
	_player_head.y = _player_body.y - 32;

	_player_Larm.x = _player_body.rc.left + 10;
	_player_Larm.y = _player_body.rc.top + 5;

	_player_Rarm.x = _player_body.rc.right + 5;
	_player_Rarm.y = _player_body.rc.top + 5;

	_player_leg.x = _player_body.rc.left - 16;
	_player_leg.y = _player_body.rc.top - 5;

	_player_head.rc = D2DMANAGER->RectMakeCenter(_player_head.x, _player_head.y, _player_head.width, _player_head.height);
	_player_Larm.rc = D2DMANAGER->RectMakeCenter(_player_Larm.x, _player_Larm.y, _player_Larm.width, _player_Larm.height);
	_player_Rarm.rc = D2DMANAGER->RectMakeCenter(_player_Rarm.x, _player_Rarm.y, _player_Rarm.width, _player_Rarm.height);
	_player_leg.rc = D2DMANAGER->RectMakeCenter(_player_leg.x, _player_leg.y, _player_leg.width, _player_leg.height);
}

// 플레이어 상태에 따라 이미지를 바꿔주는 함수
void player::animation_setting()
{
	if (_player_body.state == Player_State::IDLE)
	{
		is_digging = false;
		_player_Larm.img = IMAGEMANAGER->findImage("arm_gun");
		_currentFrameX = _currentFrameY = 0;

		_player_Larm.ani = KEYANIMANAGER->findAnimation("right_idle_arm");
		_player_Rarm.ani = KEYANIMANAGER->findAnimation("right_idle_arm");
		_player_leg.ani = KEYANIMANAGER->findAnimation("right_idle_leg");

		_player_Larm.ani->start();
		_player_Rarm.ani->start();
		_player_leg.ani->start();
	}

	if (_player_body.state == Player_State::RUN)
	{
		_player_Larm.ani = KEYANIMANAGER->findAnimation("right_run_arm");
		_player_Rarm.ani = KEYANIMANAGER->findAnimation("right_run_arm");
		_player_leg.ani = KEYANIMANAGER->findAnimation("right_run_leg");

		_player_Larm.ani->start();
		_player_Rarm.ani->start();
		_player_leg.ani->start();
	}

	if (_player_body.state == Player_State::SHOVELING)
	{
		_player_Larm.img = IMAGEMANAGER->findImage("pickaxe");

		_player_Larm.ani = KEYANIMANAGER->findAnimation("right_idle_arm");
		_player_Rarm.ani = KEYANIMANAGER->findAnimation("right_idle_arm");
		_player_leg.ani = KEYANIMANAGER->findAnimation("right_idle_leg");

		_player_Larm.ani->start();
		_player_Rarm.ani->start();
		_player_leg.ani->start();
	}
}

void player::gun_update()
{
	if (is_fired)
		bullet_count += TIMEMANAGER->getElapsedTime();

	if (_player_gun == GunType::BASIC_GUN)
	{
		_gun = IMAGEMANAGER->findImage("basic_gun");
		_status.damage = 20;
		_bullet->update();
	}

	if (_player_gun == GunType::SQUIRREL_GUN)
	{
		_gun = IMAGEMANAGER->findImage("squirrel_gun");
		_status.damage = 25;
		_squirrel_bullet->update();
	}

	if (_player_gun == GunType::GATLING_GUN)
	{
		_gun = IMAGEMANAGER->findImage("gatling_root");
		_status.damage = 30;
		_bullet->update();
	}

}
