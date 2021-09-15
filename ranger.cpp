#include "stdafx.h"
#include "player.h"

void player::animation_init_ranger()
{
	int left_idle[] = { 4 };
	KEYANIMANAGER->addArrayFrameAnimation("left_idle", "ranger", left_idle, 1, 4.0, true);
	int left_run[] = { 4, 24 };
	KEYANIMANAGER->addArrayFrameAnimation("left_run", "ranger", left_run, 2, 4.0, true);
	int left_attack[] = { 24, 34, 44 };
	KEYANIMANAGER->addArrayFrameAnimation("left_attack", "ranger", left_attack, 3, 10.0, false);

	int left_top_idle[] = { 3 };
	KEYANIMANAGER->addArrayFrameAnimation("left_top_idle", "ranger", left_top_idle, 1, 4.0, true);
	int left_top_run[] = { 3, 13 };
	KEYANIMANAGER->addArrayFrameAnimation("left_top_run", "ranger", left_top_run, 2, 4.0, true);
	int left_top_attack[] = { 23, 33, 43 };
	KEYANIMANAGER->addArrayFrameAnimation("left_top_attack", "ranger", left_top_attack, 3, 10.0, false);

	int top_idle[] = { 2 };
	KEYANIMANAGER->addArrayFrameAnimation("top_idle", "ranger", top_idle, 1, 4.0, true);
	int top_run[] = { 2, 12 };
	KEYANIMANAGER->addArrayFrameAnimation("top_run", "ranger", top_run, 2, 4.0, true);
	int top_attack[] = { 22, 32, 42 };
	KEYANIMANAGER->addArrayFrameAnimation("top_attack", "ranger", top_attack, 3, 10.0, false);

	int right_top_idle[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("right_top_idle", "ranger", right_top_idle, 1, 4.0, true);
	int right_top_run[] = { 1, 11 };
	KEYANIMANAGER->addArrayFrameAnimation("right_top_run", "ranger", right_top_run, 2, 4.0, true);
	int right_top_attack[] = { 21, 31, 41 };
	KEYANIMANAGER->addArrayFrameAnimation("right_top_attack", "ranger", right_top_attack, 3, 10.0, false);

	int right_idle[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("right_idle", "ranger", right_idle, 1, 4.0, true);
	int right_run[] = { 10, 20 };
	KEYANIMANAGER->addArrayFrameAnimation("right_run", "ranger", right_run, 2, 4.0, true);
	int right_attack[] = { 20, 30, 40 };
	KEYANIMANAGER->addArrayFrameAnimation("right_attack", "ranger", right_attack, 3, 10.0, false);

	int left_bottom_idle[] = { 5 };
	KEYANIMANAGER->addArrayFrameAnimation("left_bottom_idle", "ranger", left_bottom_idle, 1, 4.0, true);
	int left_bottom_run[] = { 5, 15 };
	KEYANIMANAGER->addArrayFrameAnimation("left_bottom_run", "ranger", left_bottom_run, 2, 4.0, true);
	int left_bottom_attack[] = { 25, 35, 45 };
	KEYANIMANAGER->addArrayFrameAnimation("left_bottom_attack", "ranger", left_bottom_attack, 3, 10.0, false);

	int bottom_idle[] = { 6 };
	KEYANIMANAGER->addArrayFrameAnimation("bottom_idle", "ranger", bottom_idle, 1, 4.0, true);
	int bottom_run[] = { 6, 16 };
	KEYANIMANAGER->addArrayFrameAnimation("bottom_run", "ranger", bottom_run, 2, 4.0, true);
	int bottom_attack[] = { 26, 36, 46 };
	KEYANIMANAGER->addArrayFrameAnimation("bottom_attack", "ranger", bottom_attack, 3, 10.0, false);

	int right_bottom_idle[] = { 7 };
	KEYANIMANAGER->addArrayFrameAnimation("right_bottom_idle", "ranger", right_bottom_idle, 1, 4.0, true);
	int right_bottom_run[] = { 7, 17 };
	KEYANIMANAGER->addArrayFrameAnimation("right_bottom_run", "ranger", right_bottom_run, 2, 4.0, true);
	int right_bottom_attack[] = { 27, 37, 47 };
	KEYANIMANAGER->addArrayFrameAnimation("right_bottom_attack", "ranger", right_bottom_attack, 3, 10.0, false);

	int dead[] = { 8, 9, 18, 19, 28, 29, 38, 39 };
	KEYANIMANAGER->addArrayFrameAnimation("dead", "ranger", dead, 8, 10.0, false);

	int skill[] = { 60, 61, 62, 63, 64, 65, 66, 67 };
	KEYANIMANAGER->addArrayFrameAnimation("skill", "ranger", skill, 8, 20, true);
}

void player::status_init_ranger()
{
	_status.level = 1;
	_status.hp = 50;
	_status.mp = 50;
	_status.max_hp = 50;
	_status.max_mp = 50;
	_status.hp_regen = 0.0f;
	_status.mp_regen = 0.50f;
	_status.move_speed = 2.0f;
	_status.exp = 0.0f;

	_status.damage = 10;
	_status.attack_dmg = 0;
	_status.ability_power = 0;
	_status.crit = 0;
	_status.evasion = 0;
	_status.armor = 10;
	_status.resistance = 0;
	_status.luck = 0;

	_status.dash_speed = 4.0f;
	_status.skill_cd = 0.8f;
}

void player::attack_ranger()
{
	rotation_mat = IMAGEMANAGER->findImage("smash_effect")->Frame_rotation_matrix(_player.rc.left - 40, _player.rc.top - 15, 0);
	_arrow->fire(L"effect/my_arrow.png", _player.x, _player.y, 18, 6, _player.angle, 10.0f);
}

void player::skillAnimation_ranger()
{
	switch (_player.state)
	{
	case Player_State::SKILL:
		player_action = KEYANIMANAGER->findAnimation("skill");
		player_action->start();

		break;

	case Player_State::SKILL2:

		break;
	}
}

void player::skillAction_ranger()
{
	D2D_RECT_F rcCollision;

	int tileIndex[2];
	int tileX, tileY;

	rcCollision = _player.rc;

	count += TIMEMANAGER->getElapsedTime() / 2;
	effect_interval = effect_interval + TIMEMANAGER->getElapsedTime();

	if (count >= TIMEMANAGER->getElapsedTime())
	{
		_arrow->fire(L"effect/my_arrow.png", _player.x, _player.y, 18, 6, interval[(int)Player_State::SKILL] * PI / 4, 10.0f);
		interval[(int)Player_State::SKILL] ++;
		count = 0;
	}

	if (KEYMANAGER->isStayKeyDown('A'))
	{
		_player.x -= _status.move_speed;
		_player.rc = D2DMANAGER->RectMakeCenter(_player.x, _player.y, _player.width, _player.height);
		_player.moving_direction = Player_Direction::LEFT;
		current_direction[(int)Player_Direction::LEFT] == true;
	}

	if (KEYMANAGER->isStayKeyDown('W'))
	{
		_player.y -= _status.move_speed;
		_player.rc = D2DMANAGER->RectMakeCenter(_player.x, _player.y, _player.width, _player.height);
		_player.moving_direction = Player_Direction::TOP;
		current_direction[(int)Player_Direction::TOP] == true;
	}

	if (KEYMANAGER->isStayKeyDown('D'))
	{
		_player.x += _status.move_speed;
		_player.rc = D2DMANAGER->RectMakeCenter(_player.x, _player.y, _player.width, _player.height);
		_player.moving_direction = Player_Direction::RIGHT;
		current_direction[(int)Player_Direction::RIGHT] == true;
	}

	if (KEYMANAGER->isStayKeyDown('S'))
	{
		_player.y += _status.move_speed;
		_player.rc = D2DMANAGER->RectMakeCenter(_player.x, _player.y, _player.width, _player.height);
		_player.moving_direction = Player_Direction::BOTTOM;
		current_direction[(int)Player_Direction::BOTTOM] == true;
	}

	if (current_direction[(int)Player_Direction::LEFT] == true && current_direction[(int)Player_Direction::TOP] == true)
	{
		_player.moving_direction = Player_Direction::LEFT_TOP;
	}
	if (current_direction[(int)Player_Direction::LEFT] == true && current_direction[(int)Player_Direction::BOTTOM] == true)
	{
		_player.moving_direction = Player_Direction::LEFT_BOTTOM;
	}
	if (current_direction[(int)Player_Direction::RIGHT] == true && current_direction[(int)Player_Direction::TOP] == true)
	{
		_player.moving_direction = Player_Direction::RIGHT_TOP;
	}
	if (current_direction[(int)Player_Direction::RIGHT] == true && current_direction[(int)Player_Direction::BOTTOM] == true)
	{
		_player.moving_direction = Player_Direction::RIGHT_BOTTOM;
	}

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


	if (effect_interval > 0.8f)
	{
		action_done = true;
	}

	if (action_done)
	{
		count = 0;
		effect_interval = 0;
		_player.state = Player_State::RUN;
		interval[(int)Player_State::SKILL] = 0;
		animation_setting();
	}
}