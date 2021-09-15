#pragma once
#include "Object.h"
#include "gameNode.h"
#include "Well.h"
#include "Plane.h"
#include "NPC.h"
#include "CopterParked.h"

class ObjectManager : public gameNode
{
private:
	vector<Object*> object_List;

public:

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void set_well(float x, float y);
	void set_plane(float x, float y);
	void set_NPC(float x, float y, int num);
	void set_copter(float x, float y);

	inline vector<Object*>& get_objectList() { return object_List; }
};

