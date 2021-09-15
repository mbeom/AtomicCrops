#include "stdafx.h"
#include "enemy.h"

HRESULT enemy::init()
{
	return S_OK;
}

HRESULT enemy::init(float x, float y)
{
	return S_OK;
}

HRESULT enemy::init(float x, float y, bool is_find)
{
	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{

}

void enemy::render()
{
	draw();
}

void enemy::move()
{
}

bool enemy::bullet_Fire()
{
	if (_fireCount % _Fire_interval == 0)
	{
		_Fire_interval = 135;
		_fireCount = 0;
		//is_set = true;
	}

	return true;
}

void enemy::draw()
{

}
