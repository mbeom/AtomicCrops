#include "stdafx.h"
#include "bullets.h"

HRESULT hitBox::init()
{
	return S_OK;
}

void hitBox::release()
{
}

void hitBox::update()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		_vBullet[i].interval += TIMEMANAGER->getElapsedTime();

		if (_vBullet[i].interval > 0.1f && &_vBullet[i] != NULL)
		{
			removeBullet(i);
		}
	}
}

void hitBox::render()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		D2DMANAGER->Rectangle(_vBullet[i].rc);
	}
}

void hitBox::fire(float x, float y, float width, float height)
{
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.interval = 0;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;

	bullet.rc = D2DMANAGER->RectMake(bullet.x, bullet.y, width, height);

	_vBullet.push_back(bullet);

}

void hitBox::move()
{

}

void hitBox::removeBullet(int arrNum)
{
	//벡터의 값을 지울떈  erase()
	_vBullet.erase(_vBullet.begin() + arrNum);
}


HRESULT basic_bullet::init(float range)
{
	_range = range;

	return S_OK;
}

HRESULT basic_bullet::init(float range, bullet_user val)
{
	_range = range;
	user = val;

	return S_OK;
}

void basic_bullet::release()
{
}

void basic_bullet::update()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = D2DMANAGER->RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void basic_bullet::render()
{
	if (user == bullet_user::PLAYER)
	{
		for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); _viBullet++)
		{
			CAMERAMANAGER->center_pos.x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
			CAMERAMANAGER->center_pos.y = (CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2;
			float x = _viBullet->x - CAMERAMANAGER->center_pos.x;
			float y = _viBullet->y - CAMERAMANAGER->center_pos.y;
			float x2 = 1.5 * (_viBullet->x - CAMERAMANAGER->center_pos.x) * cosf(_viBullet->angle);
			float y2 = 1.5 * (_viBullet->y - CAMERAMANAGER->center_pos.y) * -sinf(_viBullet->angle);
			CAMERAMANAGER->trans_mat = D2D1::Matrix3x2F::Translation(x, y);
			CAMERAMANAGER->back_trans_mat = D2D1::Matrix3x2F::Translation(-1.5* x, - 1.5 * y);
			CAMERAMANAGER->rotation_mat = D2D1::Matrix3x2F::Rotation(-_viBullet->angle * 180 / PI, 
				D2D1::Point2F(_viBullet->rc.left + _viBullet->bulletImage->getWidth() / 2 - CAMERAMANAGER->get_camera_x() + x, 
							  _viBullet->rc.top + _viBullet->bulletImage->getHeight() / 2 - CAMERAMANAGER->get_camera_y() + y ));

			D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->trans_mat * CAMERAMANAGER->rotation_mat *CAMERAMANAGER->scale_mat * CAMERAMANAGER->back_trans_mat);
			_viBullet->bulletImage->render(_viBullet->rc.left, _viBullet->rc.top);
			D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			//_viBullet->bulletImage->rotation_end();
		}
	}
	else
		for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); _viBullet++)
		{
			_viBullet->bulletImage->render(_viBullet->rc.left, _viBullet->rc.top);
		}
}

void basic_bullet::fire(LPCWSTR imagename, float x, float y, float width, float height, float angle, float speed)
{
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = new image;
	bullet.bulletImage->D_Init(imagename, width, height);
	bullet.speed = speed;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.angle = angle;

	bullet.rc = D2DMANAGER->RectMakeCenter(bullet.x, bullet.y,
		bullet.bulletImage->getWidth(),
		bullet.bulletImage->getHeight());

	_vBullet.push_back(bullet);
}

void basic_bullet::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = D2DMANAGER->RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void basic_bullet::removeBullet(int arrNum)
{
	_vBullet[arrNum].bulletImage->release();
	_vBullet.erase(_vBullet.begin() + arrNum);
}

HRESULT harvest_effect::init(image* img)
{
	_img = img;

	return S_OK;
}

void harvest_effect::release()
{
}

void harvest_effect::update()
{
	move();
}

void harvest_effect::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); _viBullet++)
	{
		_viBullet->bulletImage->FrameRender(_viBullet->rc.left, _viBullet->rc.top, 0, 0);
		//D2DMANAGER->Rectangle(_viBullet->rc);
	}
}

void harvest_effect::fire(float x, float y)
{
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = new image;
	bullet.bulletImage = _img;
	bullet.speed = 4.0f;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.angle = PI/2;

	bullet.rc = D2DMANAGER->RectMakeCenter(bullet.x, bullet.y,
		bullet.bulletImage->getFrameWidth(),
		bullet.bulletImage->getFrameHeight());

	_vBullet.push_back(bullet);

	_range = 88;
}

void harvest_effect::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		//_viBullet->x += cosf(_viBullet->angle * 180/PI) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle * 180 / PI) * _viBullet->speed;

		_viBullet->rc = D2DMANAGER->RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getFrameWidth(),
			_viBullet->bulletImage->getFrameHeight());

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			EFFECTMANAGER->play("harvest_effect", _viBullet->x, _viBullet->y);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void harvest_effect::removeBullet(int arrNum)
{
}

HRESULT font_effect::init()
{
	_range = 88;
	return S_OK;
}

void font_effect::release()
{
}

void font_effect::update()
{
	move();
}

void font_effect::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); _viBullet++)
	{
		_viBullet->bulletImage->FrameRender(_viBullet->rc.left, _viBullet->rc.top, _viBullet->currentFrameX, _viBullet->currentFrameY);
	}
}

void font_effect::fire(float x, float y, int price)
{
	int first_digit = (price / 100) % 10;
	int second_digit = (price / 10) % 10;
	int third_digit = price % 10;

	for (int i = 0; i < 3; i++)
	{
		tagBullet bullet;
		ZeroMemory(&bullet, sizeof(tagBullet));
		bullet.bulletImage = new image;
		bullet.bulletImage = IMAGEMANAGER->findImage("number");
		bullet.x = bullet.fireX = x + bullet.bulletImage->getFrameWidth() * i;
		bullet.y = bullet.fireY = y;
		bullet.angle = PI / 2;
		bullet.speed = 1.0f;

		bullet.rc = D2DMANAGER->RectMakeCenter(bullet.x, bullet.y,
			bullet.bulletImage->getFrameWidth(),
			bullet.bulletImage->getFrameHeight());

		switch (i)
		{
		case 0:
			bullet.currentFrameX = first_digit;
			bullet.currentFrameY = 0;

			if (price > 99)
				_vBullet.push_back(bullet);
			break;
		case 1:
			bullet.currentFrameX = second_digit;
			bullet.currentFrameY = 0;

			if (price > 9)
				_vBullet.push_back(bullet);
			break;
		case 2:
			bullet.currentFrameX = third_digit;
			bullet.currentFrameY = 0;

			if (price > 0)
				_vBullet.push_back(bullet);
			break;
		}
	}

}

void font_effect::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		//_viBullet->x += cosf(_viBullet->angle * 180/PI) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle * 180 / PI) * _viBullet->speed;

		_viBullet->rc = D2DMANAGER->RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getFrameWidth(),
			_viBullet->bulletImage->getFrameHeight());

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void font_effect::removeBullet(int arrNum)
{
}

HRESULT squirrel_bullet::init(float range)
{
	_range = range;

	return S_OK;
}

void squirrel_bullet::release()
{
}

void squirrel_bullet::update()
{
	move();
}

void squirrel_bullet::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); _viBullet++)
	{
		_viBullet->bulletImage->rotation_start(_viBullet->rc.left, _viBullet->rc.top, -_viBullet->angle * 180 / PI);
		_viBullet->bulletImage->render(_viBullet->rc.left, _viBullet->rc.top);
		_viBullet->bulletImage->rotation_end();
	}
}

void squirrel_bullet::fire(LPCWSTR imagename, float x, float y, float width, float height, float angle, float speed)
{
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = new image;
	bullet.bulletImage->D_Init(imagename, width, height);
	bullet.speed = speed;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.angle = angle;

	bullet.rc = D2DMANAGER->RectMakeCenter(bullet.x, bullet.y,
		bullet.bulletImage->getWidth(),
		bullet.bulletImage->getHeight());

	_vBullet.push_back(bullet);

	tagBullet bullet2;
	ZeroMemory(&bullet2, sizeof(tagBullet));
	bullet2.bulletImage = new image;
	bullet2.bulletImage->D_Init(imagename, width, height);
	bullet2.speed = speed;
	bullet2.x = bullet2.fireX = x;
	bullet2.y = bullet2.fireY = y;
	bullet2.angle = -angle;

	bullet2.rc = D2DMANAGER->RectMakeCenter(bullet2.x, bullet2.y,
		bullet2.bulletImage->getWidth(),
		bullet2.bulletImage->getHeight());

	_vBullet.push_back(bullet2);
}

void squirrel_bullet::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) *_viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = D2DMANAGER->RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void squirrel_bullet::removeBullet(int arrNum)
{
	_vBullet[arrNum].bulletImage->release();
	_vBullet.erase(_vBullet.begin() + arrNum);
}

HRESULT Boss_bullet::init(float range)
{
	_range = range;
	return S_OK;
}

void Boss_bullet::release()
{
}

void Boss_bullet::update()
{
	move();
}

void Boss_bullet::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); _viBullet++)
	{
		CAMERAMANAGER->center_pos.x = (CAMERAMANAGER->screen.left + CAMERAMANAGER->screen.right) / 2;
		CAMERAMANAGER->center_pos.y = (CAMERAMANAGER->screen.top + CAMERAMANAGER->screen.bottom) / 2;
		float x = _viBullet->x - CAMERAMANAGER->center_pos.x;
		float y = _viBullet->y - CAMERAMANAGER->center_pos.y;
		float x2 = 1.5 * (_viBullet->x - CAMERAMANAGER->center_pos.x) * cosf(_viBullet->angle);
		float y2 = 1.5 * (_viBullet->y - CAMERAMANAGER->center_pos.y) * -sinf(_viBullet->angle);
		CAMERAMANAGER->trans_mat = D2D1::Matrix3x2F::Translation(x, y);
		CAMERAMANAGER->back_trans_mat = D2D1::Matrix3x2F::Translation(-1.5* x, -1.5 * y);
		CAMERAMANAGER->rotation_mat = D2D1::Matrix3x2F::Rotation(-_viBullet->angle * 180 / PI,
			D2D1::Point2F(_viBullet->rc.left + _viBullet->bulletImage->getFrameWidth() / 2 - CAMERAMANAGER->get_camera_x() + x,
				_viBullet->rc.top + _viBullet->bulletImage->getFrameHeight() / 2 - CAMERAMANAGER->get_camera_y() + y));

		D2DMANAGER->pRenderTarget->SetTransform(CAMERAMANAGER->trans_mat * CAMERAMANAGER->rotation_mat *CAMERAMANAGER->scale_mat * CAMERAMANAGER->back_trans_mat);
		_viBullet->bulletImage->FrameRender(_viBullet->rc.left, _viBullet->rc.top, _viBullet->currentFrameX, 0);
		D2DMANAGER->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}
}

void Boss_bullet::fire(LPCWSTR imagename, float x, float y, float width, float height, int frameX, int frameY, float angle, float speed)
{
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = new image;
	bullet.bulletImage->D_Init(imagename, width, height, frameX, frameY);
	bullet.speed = speed;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.angle = angle;

	bullet.rc = D2DMANAGER->RectMakeCenter(bullet.x, bullet.y,
		bullet.bulletImage->getFrameWidth(),
		bullet.bulletImage->getFrameHeight());

	_vBullet.push_back(bullet);
}

void Boss_bullet::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) *_viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = D2DMANAGER->RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getFrameWidth(),
			_viBullet->bulletImage->getFrameHeight());

		_viBullet->currentFrameX += 1;

		if (_viBullet->currentFrameX > _viBullet->bulletImage->getMaxFrameX())
			_viBullet->currentFrameX = 0;

		_viBullet->bulletImage->setFrameX(_viBullet->currentFrameX);

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void Boss_bullet::removeBullet(int arrNum)
{
	_vBullet[arrNum].bulletImage->release();
	_vBullet.erase(_vBullet.begin() + arrNum);
}
