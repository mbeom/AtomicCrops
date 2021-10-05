#include "stdafx.h"
#include "scorpion.h"

HRESULT scorpion::init(float x, float y)
{
	IMAGEMANAGER->addFrameImage("scorpion", L"image/enemy/ScorpionA.png", 440, 264, 5, 3);

	enemy_img = IMAGEMANAGER->findImage("scorpion");

	int idle[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("scorpion_idle", "scorpion", idle, 8, 4.0f, false);

	int attack[] = { 9, 10, 11, 12 };
	KEYANIMANAGER->addArrayFrameAnimation("scorpion_attack", "scorpion", attack, 4, 4.0f, false);

	int hurt[] = { 13 };
	KEYANIMANAGER->addArrayFrameAnimation("scorpion_hurt", "scorpion", hurt, 1, 8.0f, false);

	enemy_body = KEYANIMANAGER->findAnimation("scorpion_idle");
	enemy_body->start();

	_rc = D2DMANAGER->RectMakeCenter(x, y, 44, 44);
	max_hp = hp = 120;

	_x = x;
	_y = y;
	_width = _rc.right - _rc.left;
	_height = _rc.bottom - _rc.top;

	_angle = 0.0f;

	_bullet = new basic_bullet;
	_bullet->init(1000, bullet_user::ENEMY);

	_state = enemy_state::IDLE;
	_direction = enemy_direction::RIGHT;
	_kind = enemy_kind::SCORPION;

	is_find = false;
	is_arrived = false;
	is_hit = false;
	is_fired = false;

	return S_OK;
}

void scorpion::release()
{
}

void scorpion::update()
{
	if (is_fired)
		_count += TIMEMANAGER->getElapsedTime();

	if (_count > 1.0f)
	{
		is_fired = false;
		_count = 0;
	}

	_past_direction = _direction;

	update_direction();

	switch (_state)
	{
	case enemy_state::IDLE:
		idle();
		break;
	case enemy_state::RUN:
		move();
		break;
	case enemy_state::HURT:
		hurt();
		break;
	case enemy_state::ATTACK:
		attack();
		break;
	}

	_bullet->update();
}

void scorpion::render()
{
	float screen_x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
	float screen_y = (CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2;

	float x = (_rc.left + _rc.right) / 2 - screen_x;
	float y = (_rc.top + _rc.bottom) / 2 - screen_y;

	float x2 = _rc.left + enemy_img->getFrameWidth() / 2 - CAMERAMANAGER->get_camera_x() + x;
	float y2 = _rc.top + enemy_img->getFrameHeight() / 2 - CAMERAMANAGER->get_camera_y() + y;

	CAMERAMANAGER->trans_mat = D2D1::Matrix3x2F::Translation(x, y);

	if (_direction == enemy_direction::RIGHT)
	{
		CAMERAMANAGER->scale_mat = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1.5f, 1.5f), CAMERAMANAGER->center_pos);
		CAMERAMANAGER->back_trans_mat = D2D1::Matrix3x2F::Translation(-1.5 * x, -1.5 * y);
	}
	else
	{
		CAMERAMANAGER->scale_mat = D2D1::Matrix3x2F::Scale(D2D1::SizeF(-1.5f, 1.5f), D2D1::Point2F(x2, y2));
		CAMERAMANAGER->back_trans_mat = D2D1::Matrix3x2F::Translation(-0.5 * x, -0.5 * y);
	}

	D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->trans_mat * CAMERAMANAGER->scale_mat* CAMERAMANAGER->back_trans_mat);

	enemy_img->aniRender(_rc.left, _rc.top, enemy_body);

	D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	CAMERAMANAGER->scale_mat = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1.5f, 1.5f), CAMERAMANAGER->center_pos);
	CAMERAMANAGER->back_trans_mat = D2D1::Matrix3x2F::Translation(-1.5 * x, -1.5 * y);

	D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->trans_mat * CAMERAMANAGER->scale_mat* CAMERAMANAGER->back_trans_mat);
	_bullet->render();
	//D2DMANAGER->Rectangle(_rc);
	
	WCHAR temp[128];
	//swprintf_s(temp, L"%d, %d", (int)_state, (int)is_fired);
	//D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 20, temp, _rc.left, _rc.top, _rc.right, _rc.bottom);
	D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void scorpion::idle()
{
	if (!is_find)
		idle_count += TIMEMANAGER->getElapsedTime();
	else
		idle_count = 0;

	if (idle_count > 2.0f && !is_find)
	{
		idle_count = 0;
		_speed = 1;
		_angle = RND->getFromFloatTo(0, PI2);
		_state = enemy_state::RUN;
		enemy_body = KEYANIMANAGER->findAnimation("scorpion_idle");
		enemy_body->start();
	}

	if (is_find)
	{
		_state = enemy_state::ATTACK;
		enemy_body = KEYANIMANAGER->findAnimation("scorpion_attack");
		enemy_body->start();
	}

	if (is_hit)
	{
		is_hit = false;
		_state = enemy_state::HURT;
		enemy_body = KEYANIMANAGER->findAnimation("scorpion_hurt");
		enemy_body->start();
	}
}

void scorpion::move()
{
	if (!is_find)
	{
		idle_count += TIMEMANAGER->getElapsedTime();

		_x += _speed * cosf(_angle) * TIMEMANAGER->getElapsedTime() * 50;
		_y += _speed * -sinf(_angle) * TIMEMANAGER->getElapsedTime() * 50;

		_rc = D2DMANAGER->RectMakeCenter(_x, _y, 44, 44);
	}
	else if(is_find)
	{
		float enemy_x = (_rc.left + _rc.right) / 2;
		float enemy_y = (_rc.top + _rc.bottom) / 2;

		D2D1_RECT_F temp = _list[_index];

		float dest_x = (temp.left + temp.right) / 2;
		float dest_y = (temp.top + temp.bottom) / 2;

		float distance = getDistance(enemy_x, enemy_y, dest_x, dest_y);
		float distance2 = getDistance(enemy_x, enemy_y, (_destination.left + _destination.right) / 2, (_destination.top + _destination.bottom) / 2);
		float angle = getAngle(enemy_x, enemy_y, dest_x, dest_y);

		_angle = angle;

		if (distance > 1 && !is_arrived)
		{
			_x += _speed * cosf(_angle)* TIMEMANAGER->getElapsedTime() * 60;
			_y += _speed * -sinf(_angle)* TIMEMANAGER->getElapsedTime() * 60;

			_rc = D2DMANAGER->RectMakeCenter(_x, _y, 42, 42);
		}
		else
		{
			is_done = true;
		}

		if (is_done)
		{
			_index -= 1;
			is_done = false;
		}

		if (_index <= 0)
		{
			_index = 0;
			is_arrived = true;
		}

		if (is_find && distance2 < 300)
		{
			_state = enemy_state::ATTACK;
			enemy_body = KEYANIMANAGER->findAnimation("scorpion_attack");
			enemy_body->start();
		}
	}

	if (is_hit)
	{
		is_hit = false;
		_state = enemy_state::HURT;
		enemy_body = KEYANIMANAGER->findAnimation("scorpion_hurt");
		enemy_body->start();
	}

	if (idle_count > 1.0f)
	{
		idle_count = 0;
		_state = enemy_state::IDLE;
		enemy_body = KEYANIMANAGER->findAnimation("scorpion_idle");
		enemy_body->start();
	}
}

void scorpion::attack()
{
	float enemy_x = (_rc.left + _rc.right) / 2;
	float enemy_y = (_rc.top + _rc.bottom) / 2;

	float dest_x = (_destination.left + _destination.right) / 2;
	float dest_y = (_destination.top + _destination.bottom) / 2;

	_angle = getAngle(enemy_x, enemy_y, dest_x, dest_y);

	if (!is_fired)
	{
		if (_direction == enemy_direction::RIGHT)
		{
			_bullet->fire(L"image/bullet/EnemyBasicBullet.png", _rc.right + 5, enemy_y, 32, 32, _angle, 5.0f);
			is_fired = true;
		}
		else
		{
			_bullet->fire(L"image/bullet/EnemyBasicBullet.png", _rc.left - 5, enemy_y, 32, 32, _angle, 5.0f);
			is_fired = true;
		}
	}

	if (!enemy_body->isPlay())
	{
		_state = enemy_state::RUN;
		enemy_body = KEYANIMANAGER->findAnimation("scorpion_idle");
		enemy_body->start();
	}

	if (is_hit)
	{
		is_hit = false;
		_state = enemy_state::HURT;
		enemy_body = KEYANIMANAGER->findAnimation("scorpion_hurt");
		enemy_body->start();
	}
}

void scorpion::hurt()
{
	if (!enemy_body->isPlay())
	{
		_state = enemy_state::IDLE;
		enemy_body = KEYANIMANAGER->findAnimation("scorpion_idle");
		enemy_body->start();
	}
}

void scorpion::update_direction()
{
	if (_angle >= PI2) _angle -= PI2;
	if (_angle <= 0) _angle += PI2;

	if (_angle > 0 && _angle <= PI8)
	{
		_moving_direction = enemy_direction::RIGHT;
	}
	else if (_angle > PI8 && _angle <= 3 * PI8)
	{
		_moving_direction = enemy_direction::RIGHT_TOP;
	}
	else if (_angle > 3 * PI8 && _angle <= 5 * PI8)
	{
		_moving_direction = enemy_direction::TOP;
	}
	else if (_angle > 5 * PI8 && _angle <= 7 * PI8)
	{
		_moving_direction = enemy_direction::LEFT_TOP;
	}
	else if (_angle > 7 * PI8 && _angle <= 9 * PI8)
	{
		_moving_direction = enemy_direction::LEFT;
	}
	else if (_angle > 9 * PI8 && _angle <= 11 * PI8)
	{
		_moving_direction = enemy_direction::LEFT_BOTTOM;
	}
	else if (_angle > 11 * PI8 && _angle <= 13 * PI8)
	{
		_moving_direction = enemy_direction::BOTTOM;
	}
	else if (_angle > 13 * PI8 && _angle <= 15 * PI8)
	{
		_moving_direction = enemy_direction::RIGHT_BOTTOM;
	}
	else
	{
		_moving_direction = enemy_direction::RIGHT;
	}

	if (!is_find)
	{
		if (_angle >= 0 && _angle < PI / 2)
		{
			_direction = enemy_direction::RIGHT;
		}
		else if (_angle >= PI / 2 && _angle < PI + PI / 2)
		{
			_direction = enemy_direction::LEFT;
		}
		else
			_direction = enemy_direction::RIGHT;

	}
	else
	{
		if (_destination.right > _rc.right)
		{
			_direction = enemy_direction::RIGHT;
		}
		else
			_direction = enemy_direction::LEFT;
	}
}
