#include "stdafx.h"
#include "maple.h"

HRESULT maple::init(float x, float y)
{
	IMAGEMANAGER->addFrameImage("maple", L"image/enemy/MapleA.png", 480, 264, 5, 2);

	enemy_img = IMAGEMANAGER->findImage("maple");

	int idle[] = { 0, 1, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("maple_idle", "maple", idle, 4, 4.0f, true);

	int attack[] = { 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("maple_attack", "maple", attack, 4, 4.0f, false);

	int hurt[] = { 8 };
	KEYANIMANAGER->addArrayFrameAnimation("maple_hurt", "maple", hurt, 1, 8.0f, false);

	enemy_body = KEYANIMANAGER->findAnimation("maple_idle");
	enemy_body->start();

	_rc = D2DMANAGER->RectMakeCenter(x, y, 80, 120);
	max_hp = hp = 200;

	_x = x;
	_y = y;
	_width = _rc.right - _rc.left;
	_height = _rc.bottom - _rc.top;

	_angle = 0.0f;

	_bullet = new basic_bullet;
	_bullet->init(1000, bullet_user::ENEMY);

	_state = enemy_state::IDLE;
	_direction = enemy_direction::LEFT;
	_kind = enemy_kind::MAPLE;

	_elapsedSec = 0;

	is_find = false;
	is_arrived = false;
	is_hit = false;
	is_fired = false;

	return S_OK;
}

void maple::release()
{
}

void maple::update()
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
	case enemy_state::HURT:
		hurt();
		break;
	case enemy_state::ATTACK:
		attack();
		break;
	}

	_bullet->update();
}

void maple::render()
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

	D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void maple::idle()
{
	if (is_find)
	{
		_state = enemy_state::ATTACK;
		enemy_body = KEYANIMANAGER->findAnimation("maple_attack");
		enemy_body->start();
	}

	if (is_hit)
	{
		is_hit = false;
		_state = enemy_state::HURT;
		enemy_body = KEYANIMANAGER->findAnimation("maple_hurt");
		enemy_body->start();
	}
}

void maple::attack()
{
	float enemy_x = (_rc.left + _rc.right) / 2;
	float enemy_y = (_rc.top + _rc.bottom) / 2;

	float dest_x = (_destination.left + _destination.right) / 2;
	float dest_y = (_destination.top + _destination.bottom) / 2;

	_angle = getAngle(enemy_x, enemy_y, dest_x, dest_y);

	if(!is_fired)
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

	if (!enemy_body->isPlay())
	{
		_state = enemy_state::IDLE;
		enemy_body = KEYANIMANAGER->findAnimation("maple_idle");
		enemy_body->start();
	}

	if (is_hit)
	{
		is_hit = false;
		_state = enemy_state::HURT;
		enemy_body = KEYANIMANAGER->findAnimation("maple_hurt");
		enemy_body->start();
	}
}

void maple::hurt()
{
	if (!enemy_body->isPlay())
	{
		_state = enemy_state::IDLE;
		enemy_body = KEYANIMANAGER->findAnimation("maple_idle");
		enemy_body->start();
	}
}

void maple::update_direction()
{
	if (_destination.right > _rc.right)
	{
		_direction = enemy_direction::RIGHT;
	}
	else
		_direction = enemy_direction::LEFT;
}
