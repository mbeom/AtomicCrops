#include "stdafx.h"
#include "player.h"

void player::key_setting_idle()
{
	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		_player_body.state = Player_State::RUN;
		_player_Larm.state = Player_State::RUN;
		_player_Rarm.state = Player_State::RUN;
		_player_leg.state =  Player_State::RUN;

		animation_setting();
	}

	if (KEYMANAGER->isOnceKeyDown('W'))
	{
		_player_body.state = Player_State::RUN;
		_player_Larm.state = Player_State::RUN;
		_player_Rarm.state = Player_State::RUN;
		_player_leg.state =  Player_State::RUN;

		animation_setting();
	}

	if (KEYMANAGER->isOnceKeyDown('D'))
	{
		_player_body.state = Player_State::RUN;
		_player_Larm.state = Player_State::RUN;
		_player_Rarm.state = Player_State::RUN;
		_player_leg.state = Player_State::RUN;

		animation_setting();
	}

	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		_player_body.state = Player_State::RUN;
		_player_Larm.state = Player_State::RUN;
		_player_Rarm.state = Player_State::RUN;
		_player_leg.state = Player_State::RUN;

		animation_setting();
	}

	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		is_fired = true;

		if (_player_body.direction == Player_Direction::RIGHT && bullet_count > 0.2f)
		{
			if(_player_gun == GunType::BASIC_GUN)
				_bullet->fire(L"image/bullet/default_bullet.png", - 20 + _player_Rarm.rc.right + 50 * cosf(_bullet_angle), _player_Rarm.rc.bottom + 100 * -sinf(_bullet_angle), 23, 23, _bullet_angle, 10.0f);
			else if (_player_gun == GunType::GATLING_GUN)
				_bullet->fire(L"image/bullet/GatlingGunBullet.png", _player_Rarm.rc.right + 50 * cosf(_bullet_angle), 20 + _player_Rarm.y + 100 * -sinf(_bullet_angle), 36, 20, _bullet_angle, 14.0f);
			else if (_player_gun == GunType::SQUIRREL_GUN)
				_squirrel_bullet->fire(L"image/bullet/SeedGunBullet.png", _player_Rarm.rc.right + 50 * cosf(_bullet_angle), 20 + _player_Rarm.rc.bottom + 100 * -sinf(_bullet_angle), 50, 28, _bullet_angle, 12.0f);
			
			bullet_count = 0;
		}

		if (_player_body.direction == Player_Direction::LEFT && bullet_count > 0.2f)
		{
			float x = 120 * cosf(_bullet_angle);
			if (_player_gun == GunType::BASIC_GUN)
				_bullet->fire(L"image/bullet/default_bullet.png", -40 * sinf(_bullet_angle) + _player_Rarm.rc.right + 120 * cosf(_bullet_angle), 10 + _player_Rarm.y + 100 * -sinf(_bullet_angle), 23, 23, _bullet_angle, 10.0f);
			else if (_player_gun == GunType::GATLING_GUN)
				_bullet->fire(L"image/bullet/GatlingGunBullet.png", _player_Rarm.x + 120 * cosf(_bullet_angle), 20 + _player_Rarm.y + 120 * -sinf(_bullet_angle), 36, 20, _bullet_angle, 14.0f);
			else if (_player_gun == GunType::SQUIRREL_GUN)
				_squirrel_bullet->fire(L"image/bullet/SeedGunBullet.png", _player_Rarm.rc.right + 120 * cosf(_bullet_angle), 20 + _player_Rarm.rc.bottom + 100 * -sinf(_bullet_angle), 50, 28, _bullet_angle, 12.0f);
		
			bullet_count = 0;
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		is_fired = false;
		bullet_count = 0.3f;
	}
}

void player::key_setting_run()
{
	float speed = _status.move_speed * TIMEMANAGER->getElapsedTime() * 60;

	if (KEYMANAGER->isOnceKeyUp('A'))
	{
		current_direction[(int)Player_Direction::LEFT] = false;
	}

	if (KEYMANAGER->isOnceKeyUp('D'))
	{
		current_direction[(int)Player_Direction::RIGHT] = false;
	}

	if (KEYMANAGER->isOnceKeyUp('W'))
	{
		current_direction[(int)Player_Direction::TOP] = false;
	}

	if (KEYMANAGER->isOnceKeyUp('S'))
	{
		current_direction[(int)Player_Direction::BOTTOM] = false;
	}
	
	if (KEYMANAGER->isStayKeyDown('W'))
	{
		_player_body.y -= speed;
		_player_body.rc = D2DMANAGER->RectMakeCenter(_player_body.x, _player_body.y, _player_body.width, _player_body.height);
		_player_tile = D2DMANAGER->RectMakeCenter(_player_body.x, _player_body.y, 44, 44);
		_player_body.moving_direction = Player_Direction::TOP;
		current_direction[(int)Player_Direction::TOP] = true;
	}

	if (KEYMANAGER->isStayKeyDown('S'))
	{
		_player_body.y += speed;
		_player_body.rc = D2DMANAGER->RectMakeCenter(_player_body.x, _player_body.y, _player_body.width, _player_body.height);
		_player_tile = D2DMANAGER->RectMakeCenter(_player_body.x, _player_body.y, 44, 44);
		_player_body.moving_direction = Player_Direction::BOTTOM;
		current_direction[(int)Player_Direction::BOTTOM] = true;
	}

	if (KEYMANAGER->isStayKeyDown('A'))
	{
		_player_body.x -= speed;
		_player_body.rc = D2DMANAGER->RectMakeCenter(_player_body.x, _player_body.y, _player_body.width, _player_body.height);
		_player_tile = D2DMANAGER->RectMakeCenter(_player_body.x, _player_body.y, 44, 44);
		_player_body.moving_direction = Player_Direction::LEFT;
		current_direction[(int)Player_Direction::LEFT] = true;
	}

	if (KEYMANAGER->isStayKeyDown('D'))
	{
		_player_body.x += speed;
		_player_body.rc = D2DMANAGER->RectMakeCenter(_player_body.x, _player_body.y, _player_body.width, _player_body.height);
		_player_tile = D2DMANAGER->RectMakeCenter(_player_body.x, _player_body.y, 44, 44);
		_player_body.moving_direction = Player_Direction::RIGHT;
		current_direction[(int)Player_Direction::RIGHT] = true;
	}

	if (_player_body.x > 7860)
		_player_body.x = 7860;
	if (_player_body.y < 420)
		_player_body.y = 420;

	if (_player_body.x < 800)
		_player_body.x = 800;
	if (_player_body.y > 4750)
		_player_body.y = 4750;

	if (current_direction[(int)Player_Direction::LEFT] == true && current_direction[(int)Player_Direction::TOP] == true)
	{
		_player_body.moving_direction = Player_Direction::LEFT_TOP;
	}
	if (current_direction[(int)Player_Direction::LEFT] == true && current_direction[(int)Player_Direction::BOTTOM] == true)
	{
		_player_body.moving_direction = Player_Direction::LEFT_BOTTOM;
	}
	if (current_direction[(int)Player_Direction::RIGHT] == true && current_direction[(int)Player_Direction::TOP] == true)
	{
		_player_body.moving_direction = Player_Direction::RIGHT_TOP;
	}
	if (current_direction[(int)Player_Direction::RIGHT] == true && current_direction[(int)Player_Direction::BOTTOM] == true)
	{
		_player_body.moving_direction = Player_Direction::RIGHT_BOTTOM;
	}

	if (current_direction[(int)Player_Direction::LEFT] == false && current_direction[(int)Player_Direction::TOP] == false &&
		current_direction[(int)Player_Direction::RIGHT] == false && current_direction[(int)Player_Direction::BOTTOM] == false)
	{
		_player_body.state = Player_State::IDLE;
		animation_setting();
	}

	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		is_fired = true;

		if (_player_body.direction == Player_Direction::RIGHT && bullet_count > 0.2f)
		{
			if (_player_gun == GunType::BASIC_GUN)
				_bullet->fire(L"image/bullet/default_bullet.png", - 20 + _player_Rarm.rc.right + 50 * cosf(_bullet_angle), _player_Rarm.rc.bottom + 100 * -sinf(_bullet_angle), 23, 23, _bullet_angle, 10.0f);
			else if (_player_gun == GunType::GATLING_GUN)
				_bullet->fire(L"image/bullet/GatlingGunBullet.png", _player_Rarm.rc.right + 50 * cosf(_bullet_angle), 20 + _player_Rarm.y + 100 * -sinf(_bullet_angle), 36, 20, _bullet_angle, 14.0f);
			else if (_player_gun == GunType::SQUIRREL_GUN)
				_squirrel_bullet->fire(L"image/bullet/SeedGunBullet.png", _player_Rarm.rc.right + 50 * cosf(_bullet_angle), 20 + _player_Rarm.rc.bottom + 100 * -sinf(_bullet_angle), 50, 28, _bullet_angle, 12.0f);

			bullet_count = 0;
		}

		if (_player_body.direction == Player_Direction::LEFT && bullet_count > 0.2f)
		{
			float x = 120 * cosf(_bullet_angle);
			if (_player_gun == GunType::BASIC_GUN)
				_bullet->fire(L"image/bullet/default_bullet.png", -40 * sinf(_bullet_angle) + _player_Rarm.rc.right + 120 * cosf(_bullet_angle), 10 + _player_Rarm.y + 100 * -sinf(_bullet_angle), 23, 23, _bullet_angle, 10.0f);
			else if (_player_gun == GunType::GATLING_GUN)
				_bullet->fire(L"image/bullet/GatlingGunBullet.png", _player_Rarm.x + 120 * cosf(_bullet_angle), 20 + _player_Rarm.y + 120 * -sinf(_bullet_angle), 36, 20, _bullet_angle, 14.0f);
			else if (_player_gun == GunType::SQUIRREL_GUN)
				_squirrel_bullet->fire(L"image/bullet/SeedGunBullet.png", _player_Rarm.rc.right + 120 * cosf(_bullet_angle), 20 + _player_Rarm.rc.bottom + 100 * -sinf(_bullet_angle), 50, 28, _bullet_angle, 12.0f);

			bullet_count = 0;
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		is_fired = false;
		bullet_count = 0.3f;
	}
}

void player::key_setting_shoveling()
{
	_player_Larm.angle += 0.1;

	if (_player_body.direction == Player_Direction::RIGHT)
	{
		if (_player_Larm.angle >= 1.6) _player_Larm.angle = 0;
	}
	else
	{
		if (_player_Larm.angle >= 3.2) _player_Larm.angle = 1.7;
	}
}
