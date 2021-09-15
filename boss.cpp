#include "stdafx.h"
#include "boss.h"

HRESULT boss::init(float x, float y)
{
	IMAGEMANAGER->addFrameImage("boss", L"image/enemy/SpringBoss.png", 1750, 1980, 5, 9);
	IMAGEMANAGER->addFrameImage("boss_dead", L"image/enemy/SpringBossDeath.png", 1750, 880, 5, 4);

	enemy_img = IMAGEMANAGER->findImage("boss");

	int idle[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	KEYANIMANAGER->addArrayFrameAnimation("boss_idle", "boss", idle, 16, 4.0f, true);

	int move[] = { 16, 17, 18, 19 };
	KEYANIMANAGER->addArrayFrameAnimation("boss_move", "boss", move, 4, 4.0f, true);

	int escape[] = { 20, 21, 22, 23, 24, 25, 26 };
	KEYANIMANAGER->addArrayFrameAnimation("boss_escape", "boss", escape, 7, 4.0f, true);

	int attack[] = { 27, 28, 29, 30, 31, 32, 33 };
	KEYANIMANAGER->addArrayFrameAnimation("boss_attack", "boss", attack, 7, 4.0f, false);

	int hurt[] = { 34 };
	KEYANIMANAGER->addArrayFrameAnimation("boss_hurt", "boss", hurt, 1, 4.0f, false);

	int dead[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
	KEYANIMANAGER->addArrayFrameAnimation("boss_dead", "boss_dead", dead, 18, 8.0f, false);

	enemy_body = KEYANIMANAGER->findAnimation("boss_idle");
	enemy_body->start();

	_rc = D2DMANAGER->RectMakeCenter(x, y, 168, 168);
	max_hp = hp = 3000;

	_x = x;
	_y = y;
	_width = _rc.right - _rc.left;
	_height = _rc.bottom - _rc.top;

	_speed = 1.0f;
	_angle = 0.0f;
	_groupNumber = 1;

	_bullet = new basic_bullet;
	_bullet->init(1000, bullet_user::ENEMY);

	_bossBullet = new Boss_bullet;
	_bossBullet->init(1000);

	_state = enemy_state::IDLE;
	_direction = enemy_direction::LEFT;
	_kind = enemy_kind::BOSS;

	_map.x = 3920;
	_map.y = 1759;
	_map.radius = 1000;

	_hp = new progressBar;
	_hp->init(CAMERAMANAGER->screen.left + (CAMERAMANAGER->screen.right - CAMERAMANAGER->screen.left) * 0.2, 
			  CAMERAMANAGER->screen.top, 406, 28);

	_count = _escape_count = 0;

	alpha = 1.0f;

	is_find = true;
	is_arrived = false;
	is_hit = false;
	is_fired = false;
	is_ready = true;
	is_patrol = false;

	return S_OK;
}

void boss::release()
{
}

void boss::update()
{
	if (is_fired)
		_count += TIMEMANAGER->getElapsedTime();

	if (!is_ready)
		hit_interval += TIMEMANAGER->getElapsedTime();

	if (_count > 1.0f)
	{
		is_fired = false;
		_count = 0;
	}

	if (hit_interval > 0.5f)
	{
		is_ready = true;
		hit_interval = 0;
	}

	_past_direction = _direction;

	update_direction();

	switch (_state)
	{
	case enemy_state::IDLE:
		idle();
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
	case enemy_state::PATROL:
		escape();
		break;
	case enemy_state::ATTACK:
		attack();
		break;
	case enemy_state::DEAD:
		dead();
		break;
	}

	if (hp < 0 && !end)
	{
		end = true;
		_state = enemy_state::DEAD;
		enemy_img = IMAGEMANAGER->findImage("boss_dead");
		enemy_body = KEYANIMANAGER->findAnimation("boss_dead");
		enemy_body->start();
	}

	_bullet->update();
	_bossBullet->update();

	_hp->setX(CAMERAMANAGER->screen.left + WINSIZEX * 0.5);
	_hp->setY(CAMERAMANAGER->screen.top + 150);
	_hp->setGauge(hp, max_hp);
	_hp->update();
}

void boss::render()
{
	float screen_x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
	float screen_y = (CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2;

	float x = _x - screen_x;
	float y = _y - screen_y;

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

	enemy_img->aniRender(_rc.left, _rc.top, enemy_body, alpha);

	D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	CAMERAMANAGER->scale_mat = D2D1::Matrix3x2F::Scale(D2D1::SizeF(1.5f, 1.5f), CAMERAMANAGER->center_pos);
	CAMERAMANAGER->back_trans_mat = D2D1::Matrix3x2F::Translation(-1.5 * x, -1.5 * y);

	D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->trans_mat * CAMERAMANAGER->scale_mat* CAMERAMANAGER->back_trans_mat);
	_bullet->render();
	_hp->render();
	//D2DMANAGER->Rectangle(_rc);

	WCHAR temp[128];
	//swprintf_s(temp, L"%d, %d", (int)_state, (int)is_fired);
	D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 20, temp, _rc.left, _rc.top, _rc.right, _rc.bottom);
	D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_bossBullet->render();
}

void boss::idle()
{
	float dest_x = (_destination.left + _destination.right) / 2;
	float dest_y = (_destination.top + _destination.bottom) / 2;

	float angle = getAngle(_x, _y, dest_x, dest_y);

	if (!is_fired)
	{
		if (_direction == enemy_direction::RIGHT)
		{
			_bullet->fire(L"image/bullet/EnemyBasicBullet.png", _rc.right + 5, _y, 32, 32, angle, 5.0f);
			is_fired = true;
		}
		else
		{
			_bullet->fire(L"image/bullet/EnemyBasicBullet.png", _rc.left - 5, _y, 32, 32, angle, 5.0f);
			is_fired = true;
		}
	}

	if (is_hit && is_ready)
	{
		is_hit = false;
		is_ready = false;

		_state = enemy_state::HURT;
		enemy_body = KEYANIMANAGER->findAnimation("boss_hurt");
		enemy_body->start();
	}
	else if (getDistance(_x, _y, dest_x, dest_y) > 100)
	{
		_state = enemy_state::RUN;
		enemy_body = KEYANIMANAGER->findAnimation("boss_move");
		enemy_body->start();
	}
}

void boss::idle_find()
{
	float dest_x = (_destination.left + _destination.right) / 2;
	float dest_y = (_destination.top + _destination.bottom) / 2;

	_angle = getAngle(_x, _y, dest_x, dest_y);

	_x += _speed * cosf(_angle) * TIMEMANAGER->getElapsedTime() * 100;
	_y += _speed * -sinf(_angle) * TIMEMANAGER->getElapsedTime() * 100;

	_rc = D2DMANAGER->RectMakeCenter(_x, _y, 168, 168);

	if (getDistance(_x, _y, dest_x, dest_y) <= 250)
	{
		_speed = 1;
		_state = enemy_state::ATTACK;
		enemy_body = KEYANIMANAGER->findAnimation("boss_attack");
		enemy_body->start();
	}
}

void boss::move()
{
	float dest_x = (_destination.left + _destination.right) / 2;
	float dest_y = (_destination.top + _destination.bottom) / 2;

	float angle = getAngle(_x, _y, dest_x, dest_y);

	if (getDistance(_x, _y, dest_x, dest_y) > 200)
	{
		_x += _speed * cosf(angle) * TIMEMANAGER->getElapsedTime() * 100;
		_y += _speed * -sinf(angle) * TIMEMANAGER->getElapsedTime() * 100;

		_rc = D2DMANAGER->RectMakeCenter(_x, _y, 168, 168);
	}

	if (!is_fired)
	{
		if (_direction == enemy_direction::RIGHT)
		{
			_bullet->fire(L"image/bullet/EnemyBasicBullet.png", _rc.right + 5, _y, 32, 32, angle, 5.0f);
			is_fired = true;
		}
		else
		{
			_bullet->fire(L"image/bullet/EnemyBasicBullet.png", _rc.left - 5, _y, 32, 32, angle, 5.0f);
			is_fired = true;
		}
	}

	if (is_hit && is_ready)
	{
		is_hit = false;
		is_ready = false;
		_state = enemy_state::HURT;
		enemy_body = KEYANIMANAGER->findAnimation("boss_hurt");
		enemy_body->start();
	}

}

void boss::escape()
{
	if (getDistance(_x, _y, _map.x, _map.y) > _map.radius)
		is_patrol = true;

	if (getDistance(_x, _y, _map.x, _map.y) < _map.radius && !is_patrol)
	{
		_x += _speed * cosf(escape_angle) * TIMEMANAGER->getElapsedTime() * 100;
		_y += _speed * -sinf(escape_angle) * TIMEMANAGER->getElapsedTime() * 100;

		_rc = D2DMANAGER->RectMakeCenter(_x, _y, 168, 168);
	}

	if(is_patrol)
	{
		_x = _map.x + _map.radius * cosf(escape_angle);
		_y = _map.y + _map.radius * -sinf(escape_angle);

		_rc = D2DMANAGER->RectMakeCenter(_x, _y, 168, 168);

		escape_angle += TIMEMANAGER->getElapsedTime() * 0.5;

		if (escape_angle >= PI2) escape_angle -= PI2;
		if (escape_angle <= 0) escape_angle += PI2;
	}

	if (is_patrol)
		_escape_count += TIMEMANAGER->getElapsedTime();

	if (_escape_count > 2.0f)
	{
		is_patrol = false;
		_escape_count = 0;
		_state = enemy_state::IDLE_FIND;
	}
}

void boss::attack()
{
	if (!is_fired)
	{
		float dest_x = (_destination.left + _destination.right) / 2;
		float dest_y = (_destination.top + _destination.bottom) / 2;

		float angle = getAngle(_x, _y, dest_x, dest_y);

		if (_direction == enemy_direction::RIGHT)
		{
			_bossBullet->fire(L"image/bullet/SlowEnemyGoo.png", _rc.right + 5, _y, 352, 88, 4, 1, angle, 5.0f);
			_bossBullet->fire(L"image/bullet/SlowEnemyGoo.png", _rc.right + 5, _y, 352, 88, 4, 1, angle - PI8, 5.0f);
			_bossBullet->fire(L"image/bullet/SlowEnemyGoo.png", _rc.right + 5, _y, 352, 88, 4, 1, angle + PI8, 5.0f);
			is_fired = true;
		}
		else
		{
			_bossBullet->fire(L"image/bullet/SlowEnemyGoo.png", _x, _y + 40, 352, 88, 4, 1, angle, 5.0f);
			_bossBullet->fire(L"image/bullet/SlowEnemyGoo.png", _x, _y + 40, 352, 88, 4, 1, angle - PI8, 5.0f);
			_bossBullet->fire(L"image/bullet/SlowEnemyGoo.png", _x, _y + 40, 352, 88, 4, 1, angle + PI8, 5.0f);
			is_fired = true;
		}
	}

	if (!enemy_body->isPlay())
	{
		_state = enemy_state::RUN;
		enemy_body = KEYANIMANAGER->findAnimation("boss_move");
		enemy_body->start();
	}
}

void boss::hurt()
{
	if (hit_count >= 5)
	{
		float dest_x = (_destination.left + _destination.right) / 2;
		float dest_y = (_destination.top + _destination.bottom) / 2;

		if (_destination.right > _rc.right)
		{
			_direction = enemy_direction::LEFT;
		}
		else
			_direction = enemy_direction::RIGHT;

		hit_count = 0;
		_speed = 3.0f;
		escape_angle = _angle;
		_state = enemy_state::PATROL;
		enemy_body = KEYANIMANAGER->findAnimation("boss_escape");
		enemy_body->start();

	}

	if (!enemy_body->isPlay() && hit_count < 5)
	{
		hit_count += 1;

		_state = enemy_state::IDLE;
		enemy_body = KEYANIMANAGER->findAnimation("boss_idle");
		enemy_body->start();
	}
}

void boss::dead()
{
	if (!enemy_body->isPlay())
	{
		alpha -= TIMEMANAGER->getElapsedTime();
	}

	if (alpha < 0)
	{
		is_dead = true;
	}
}

void boss::update_direction()
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

	if(is_find && _state != enemy_state::PATROL)
	{
		if (_destination.right > _rc.right)
		{
			_direction = enemy_direction::RIGHT;
		}
		else
			_direction = enemy_direction::LEFT;
	}
	
}
