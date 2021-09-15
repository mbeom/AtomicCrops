#include "stdafx.h"
#include "ObjectManager.h"
#include "Player.h"

HRESULT ObjectManager::init()
{
	object_List.clear();

	return S_OK;
}

void ObjectManager::release()
{
}

void ObjectManager::update()
{
	for (int i = 0; i < object_List.size(); i++)
	{
		object_List[i]->update();
	}
}

void ObjectManager::render()
{
	for (int i = 0; i < object_List.size(); i++)
	{
		object_List[i]->render();
	}
}

void ObjectManager::set_well(float x, float y)
{
	Object* _well = new Well;
	_well->init(x, y);

	object_List.push_back(_well);
}

void ObjectManager::set_plane(float x, float y)
{
	Object* _plane = new Plane;
	_plane->init(x, y);

	object_List.push_back(_plane);
}

void ObjectManager::set_NPC(float x, float y, int num)
{
	Object* _NPC = new NPC;
	_NPC->init(x, y, num);

	object_List.push_back(_NPC);
}

void ObjectManager::set_copter(float x, float y)
{
	Object* _copter = new CopterParked;
	_copter->init(x, y);

	object_List.push_back(_copter);
}
