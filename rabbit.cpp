#include "stdafx.h"
#include "rabbit.h"

HRESULT rabbit::init(float x, float y)
{
	IMAGEMANAGER->addFrameImage("rabbit_normal", L"image/enemy/rabbit_normal.png", 102, 102, 1, 1);
	IMAGEMANAGER->addFrameImage("rabbit_shoot", L"image/enemy/rabbit_shoot.png", 102, 102, 1, 1);
	IMAGEMANAGER->addFrameImage("rabbit_arm", L"image/enemy/rabbit_arm.png", 28, 20, 1, 1);
	IMAGEMANAGER->addFrameImage("rabbit_cry", L"image/enemy/rabbit_cry.png", 510, 102, 5, 1);
	IMAGEMANAGER->addFrameImage("rabbit_idle", L"image/enemy/rabbit_idle.png", 510, 102, 5, 1);
	IMAGEMANAGER->addFrameImage("rabbit_find", L"image/enemy/rabbit_find.png", 918, 102, 9, 1);
	IMAGEMANAGER->addFrameImage("rabbit_run", L"image/enemy/rabbit_run.png", 816, 102, 8, 1);

	IMAGEMANAGER->addFrameImage("rabbit_normal_left", L"image/enemy/rabbit_normal_left.png", 102, 102, 1, 1);
	IMAGEMANAGER->addFrameImage("rabbit_shoot_left", L"image/enemy/rabbit_shoot_left.png", 102, 102, 1, 1);
	IMAGEMANAGER->addFrameImage("rabbit_arm_left", L"image/enemy/rabbit_arm_left.png", 28, 20, 1, 1);
	IMAGEMANAGER->addFrameImage("rabbit_cry_left", L"image/enemy/rabbit_cry_left.png", 510, 102, 5, 1);
	IMAGEMANAGER->addFrameImage("rabbit_idle_left", L"image/enemy/rabbit_idle_left.png", 510, 102, 5, 1);
	IMAGEMANAGER->addFrameImage("rabbit_find_left", L"image/enemy/rabbit_find_left.png", 918, 102, 9, 1);
	IMAGEMANAGER->addFrameImage("rabbit_run_left", L"image/enemy/rabbit_run_left.png", 816, 102, 8, 1);

	_rc = D2DMANAGER->RectMakeCenter(x, y, 44, 44);
	enemy_img = IMAGEMANAGER->findImage("rabbit_idle");
	_shoot = IMAGEMANAGER->findImage("rabbit_arm");
	max_hp = hp = 160;

	_x = x;
	_y = y;
	_width = _rc.right - _rc.left;
	_height = _rc.bottom - _rc.top;

	_speed = 2.0f;
	_angle = 0.0f;

	_bullet = new basic_bullet;
	_bullet->init(1000, bullet_user::ENEMY);

	_state = enemy_state::IDLE_SLEEP;
	_direction = enemy_direction::RIGHT;
	_kind = enemy_kind::RABBIT;

	_elapsedSec = 0;
	_frameUpdateSec = 0;

	_index = 0;
	_currentFrameX = _currentFrameY = 0;

	is_find = false;
	is_arrived = false;
	is_hit = false;

	return S_OK;
}

HRESULT rabbit::init(float x, float y, bool _is_find)
{
	_rc = D2DMANAGER->RectMakeCenter(x, y, 44, 44);
	enemy_img = IMAGEMANAGER->findImage("rabbit_idle");
	_shoot = IMAGEMANAGER->findImage("rabbit_arm");
	max_hp = hp = 160;

	_x = x;
	_y = y;
	_width = _rc.right - _rc.left;
	_height = _rc.bottom - _rc.top;

	_speed = 2.0f;
	_angle = 0.0f;

	_bullet = new basic_bullet;
	_bullet->init(1000, bullet_user::ENEMY);

	_state = enemy_state::IDLE;
	_direction = enemy_direction::RIGHT;
	_kind = enemy_kind::RABBIT;

	_elapsedSec = 0;
	_frameUpdateSec = 0;

	_index = 0;
	_groupNumber = 1;
	_currentFrameX = _currentFrameY = 0;

	is_find = _is_find;
	is_arrived = false;
	is_hit = false;

	return S_OK;
}

void rabbit::release()
{

}

void rabbit::update()
{
	_frameUpdateSec += TIMEMANAGER->getElapsedTime();

	_past_direction = _direction;

	if (_frameUpdateSec > 0.15f)
	{
		_frameUpdateSec = 0;

		enemy_img->setFrameX(_currentFrameX);

		if (_direction == enemy_direction::RIGHT)
		{
			_currentFrameX += 1;
		}
		else
			_currentFrameX -= 1;
	}

	if (!is_find && _state == enemy_state::RUN)
	{
		_state = enemy_state::IDLE;

		if (_direction == enemy_direction::RIGHT)
		{
			enemy_img = IMAGEMANAGER->findImage("rabbit_normal");
			_currentFrameX = 0;
		}
		else
		{
			enemy_img = IMAGEMANAGER->findImage("rabbit_normal_left");
			_currentFrameX = 0;
		}

	}

	update_direction();

	switch (_state)
	{
	case enemy_state::IDLE:
		idle();
		break;
	case enemy_state::IDLE_SLEEP:
		idle_sleep();
		break;
	case enemy_state::IDLE_FIND:
		idle_find();
		break;
	case enemy_state::RUN:
		move();
		break;
	case enemy_state::HURT:
		hurt();
		break;
	case enemy_state::SLEEP_HURT:
		break;
	case enemy_state::ATTACK:
		attack();
		break;
	case enemy_state::DEAD:
		break;
	}

	_bullet->move();
}

void rabbit::render()
{
	float screen_x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
	float screen_y = (CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2;

	float x = (_rc.left + _rc.right) / 2 - screen_x;
	float y = (_rc.top + _rc.bottom) / 2 - screen_y;

	CAMERAMANAGER->trans_mat = D2D1::Matrix3x2F::Translation(x, y);
	CAMERAMANAGER->back_trans_mat = D2D1::Matrix3x2F::Translation(-1.5* x, -1.5 * y);

	D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->trans_mat * CAMERAMANAGER->scale_mat * CAMERAMANAGER->back_trans_mat);

	enemy_img->FrameRender(_rc.left - 30, _rc.top - 50, _currentFrameX, _currentFrameY);

	if (_state == enemy_state::ATTACK)
	{
		_shoot->FrameRender(_rc.left + 10, _rc.top + 20, 0, 0);
	}

	//D2DMANAGER->Rectangle(_rc);

	_bullet->render();

	D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	/*
	WCHAR str[128];
	swprintf_s(str, L"%d, %d", (int)_state, (int)is_find);
	D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 20, str, _rc.left, _rc.top, _rc.right, _rc.bottom);
	*/
}

void rabbit::idle()
{
	if (_direction == enemy_direction::RIGHT)
	{
		if (_currentFrameX > enemy_img->getMaxFrameX())
		{
			_currentFrameX = 0;
		}

		if (is_find)
		{
			_state = enemy_state::RUN;
			_currentFrameX = 0;
			enemy_img = IMAGEMANAGER->findImage("rabbit_run");
		}

		if (is_hit)
		{
			_state = enemy_state::HURT;
			enemy_img = IMAGEMANAGER->findImage("rabbit_cry");
			_currentFrameX = 0;
		}
	}
	else
	{
		if (_currentFrameX <= 0)
		{
			_currentFrameX = enemy_img->getMaxFrameX();
		}

		if (is_find)
		{
			_state = enemy_state::RUN;
			enemy_img = IMAGEMANAGER->findImage("rabbit_run_left");
			_currentFrameX = enemy_img->getMaxFrameX();
		}

		if (is_hit)
		{
			_state = enemy_state::HURT;
			enemy_img = IMAGEMANAGER->findImage("rabbit_cry_left");
			_currentFrameX = enemy_img->getMaxFrameX();
		}
	}
}

void rabbit::idle_find()
{
	if (_direction == enemy_direction::RIGHT)
	{
		if (_currentFrameX > enemy_img->getMaxFrameX())
		{
			_state = enemy_state::RUN;
			enemy_img = IMAGEMANAGER->findImage("rabbit_run");
			_currentFrameX = 0;
		}

		if (is_hit)
		{
			_state = enemy_state::HURT;
			enemy_img = IMAGEMANAGER->findImage("rabbit_cry");
			_currentFrameX = 0;
		}
	}
	else
	{
		if (_currentFrameX <= 0)
		{
			_state = enemy_state::RUN;
			enemy_img = IMAGEMANAGER->findImage("rabbit_run_left");
			_currentFrameX = enemy_img->getMaxFrameX();
		}

		if (is_hit)
		{
			_state = enemy_state::HURT;
			enemy_img = IMAGEMANAGER->findImage("rabbit_cry_left");
			_currentFrameX = enemy_img->getMaxFrameX();
		}
	}

}

void rabbit::idle_sleep()
{
	if (_direction == enemy_direction::RIGHT)
	{
		if (_currentFrameX > enemy_img->getMaxFrameX())
		{
			_currentFrameX = 0;
		}

		if (is_hit)
		{
			_state = enemy_state::HURT;
			enemy_img = IMAGEMANAGER->findImage("rabbit_cry");
			_currentFrameX = 0;
		}

		if (is_find)
		{
			_state = enemy_state::IDLE_FIND;
			enemy_img = IMAGEMANAGER->findImage("rabbit_find");
			_currentFrameX = 0;
		}
	}
	else
	{
		if (_currentFrameX <= 0)
		{
			_currentFrameX = enemy_img->getMaxFrameX();
		}

		if (is_hit)
		{
			_state = enemy_state::HURT;
			enemy_img = IMAGEMANAGER->findImage("rabbit_cry_left");
			_currentFrameX = enemy_img->getMaxFrameX();
		}

		if (is_find)
		{
			_state = enemy_state::IDLE_FIND;
			enemy_img = IMAGEMANAGER->findImage("rabbit_find_left");
			_currentFrameX = enemy_img->getMaxFrameX();
		}
	}
}

void rabbit::sleep_hurt()
{
}

void rabbit::hurt()
{
	is_hit = false;
	is_find = true;
	
	_x -= cosf(_angle);
	_y -= cosf(_angle);

	D2DMANAGER->RectMakeCenter(_x, _y, 44, 44);

	if (_direction == enemy_direction::RIGHT)
	{
		if (_currentFrameX > enemy_img->getMaxFrameX())
		{
			_state = enemy_state::IDLE;
			enemy_img = IMAGEMANAGER->findImage("rabbit_normal");
			_currentFrameX = 0;
		}
	}
	else
	{
		if (_currentFrameX <= 0)
		{
			_state = enemy_state::IDLE;
			enemy_img = IMAGEMANAGER->findImage("rabbit_normal");
			_currentFrameX = 0;
		}
	}
}

void rabbit::move()
{
	if (_list.size() == 0) return;

	if (_direction == enemy_direction::RIGHT)
	{
		enemy_img = IMAGEMANAGER->findImage("rabbit_run");

		if (_currentFrameX > enemy_img->getMaxFrameX())
		{
			_currentFrameX = 0;
		}

		if (is_hit)
		{
			_state = enemy_state::HURT;
			enemy_img = IMAGEMANAGER->findImage("rabbit_cry");
			_currentFrameX = 0;
		}
	}
	else
	{
		enemy_img = IMAGEMANAGER->findImage("rabbit_run_left");

		if (_currentFrameX <= 0)
		{
			_currentFrameX = enemy_img->getMaxFrameX();
		}

		if (is_hit)
		{
			_state = enemy_state::HURT;
			enemy_img = IMAGEMANAGER->findImage("rabbit_cry_left");
			_currentFrameX = enemy_img->getMaxFrameX();
		}
	}

	float enemy_x = (_rc.left + _rc.right) / 2;
	float enemy_y = (_rc.top + _rc.bottom) / 2;

	D2D1_RECT_F temp = _list[_index];

	float dest_x = (temp.left + temp.right) / 2;
	float dest_y = (temp.top + temp.bottom) / 2;

	float distance = getDistance(enemy_x, enemy_y, dest_x, dest_y);
	float distance2 = getDistance(enemy_x, enemy_y, (_destination.left + _destination.right) / 2, (_destination.top + _destination.bottom) / 2);
	float angle = getAngle(enemy_x, enemy_y, dest_x, dest_y);

	_angle = angle;

	update_direction();

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

	if (distance2 < 300)
	{
		is_arrived = true;
		_state = enemy_state::ATTACK;

		if (_direction == enemy_direction::RIGHT)
		{
			enemy_img = IMAGEMANAGER->findImage("rabbit_shoot");
			_shoot = IMAGEMANAGER->findImage("rabbit_arm");
			_currentFrameX = 0;
		}
		else
		{
			enemy_img = IMAGEMANAGER->findImage("rabbit_shoot_left");
			_shoot = IMAGEMANAGER->findImage("rabbit_arm_left");
			_currentFrameX = enemy_img->getMaxFrameX();
		}
	}

}

void rabbit::attack()
{
	_count += TIMEMANAGER->getElapsedTime();

	if (_direction == enemy_direction::RIGHT && (_direction == _past_direction))
	{
		if (_currentFrameX > enemy_img->getMaxFrameX())
		{
			_currentFrameX = 0;
		}

		if (is_hit)
		{
			_state = enemy_state::HURT;
			enemy_img = IMAGEMANAGER->findImage("rabbit_cry");
			_currentFrameX = 0;
		}
	}
	else
	{
		if (_currentFrameX <= 0)
		{
			_currentFrameX = 0;
		}

		if (is_hit)
		{
			_state = enemy_state::HURT;
			enemy_img = IMAGEMANAGER->findImage("rabbit_cry_left");
			_currentFrameX = enemy_img->getMaxFrameX();
		}
	}

	float enemy_x = (_rc.left + _rc.right) / 2;
	float enemy_y = (_rc.top + _rc.bottom) / 2;

	float dest_x = (_destination.left + _destination.right) / 2;
	float dest_y = (_destination.top + _destination.bottom) / 2;

	_angle = getAngle(enemy_x, enemy_y, dest_x, dest_y);

	if (_count > 1.5f)
	{
		_count = 0;
		_state = enemy_state::IDLE;

		if (_direction == enemy_direction::RIGHT)
		{
			_bullet->fire(L"image/bullet/EnemyBasicBullet.png", _rc.right + 5, _rc.top + 16, 32, 32, _angle, 5.0f);
			enemy_img = IMAGEMANAGER->findImage("rabbit_normal");
			_currentFrameX = 0;
		}
		else
		{
			_bullet->fire(L"image/bullet/EnemyBasicBullet.png", _rc.left - 5, _rc.top + 16, 32, 32, _angle, 5.0f);
			enemy_img = IMAGEMANAGER->findImage("rabbit_normal_left");
			_currentFrameX = 0;
		}
	}
}

void rabbit::dead()
{
}

void rabbit::update_direction()
{
	if (_angle >= PI2) _angle -= PI2;
	if (_angle <= 0) _angle += PI2;

	if (_angle > 0 && _angle <= PI8)
	{
		_angle = 0.0f;
	}
	else if (_angle > PI8 && _angle <= 3 * PI8)
	{

	}
	else if (_angle > 3 * PI8 && _angle <= 5 * PI8)
	{
		_angle = PI / 2;
	}
	else if (_angle > 5 * PI8 && _angle <= 7 * PI8)
	{

	}
	else if (_angle > 7 * PI8 && _angle <= 9 * PI8)
	{
		_angle = PI;
	}
	else if (_angle > 9 * PI8 && _angle <= 11 * PI8)
	{

	}
	else if (_angle > 11 * PI8 && _angle <= 13 * PI8)
	{
		_angle = PI + PI / 2;
	}
	else if (_angle > 13 * PI8 && _angle <= 15 * PI8)
	{

	}
	else
	{

	}

	if (_destination.right > _rc.right)
	{
		_direction = enemy_direction::RIGHT;
	}
	else
		_direction = enemy_direction::LEFT;
}
