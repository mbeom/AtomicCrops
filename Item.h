#pragma once
#include "gameNode.h"
#include "Crop.h"

enum class ItemType
{
	CONSUMPTION,
	EQUIPMENT,
	PICKAXE,
	FERTILIZER,

	TYPE_NUM
};

enum class GunType
{
	BASIC_GUN,
	GATLING_GUN,
	SQUIRREL_GUN,

	TYPE_NUM
};

struct ItemObject
{
	float x, y;
	float angle;
	float speed, gravity;
	float bounce;
	D2D1_RECT_F rc;
	image* img;
	
	bool is_taken;
	bool is_drop;
};

struct Description_box
{
	float x, y;
	float width, height;
	D2D1_RECT_F rc;
	image* img;
	wstring description;
};

class Item : public gameNode
{

protected:
	float _x, _y;
	float width, height;
	float count, loading;
	D2D1_RECT_F rc;
	image* _img;
	animation* _ani;

	int quantity;
	int damage;
	int price;

	ItemObject* object;
	ItemType type;
	GunType gun_type;
	Crop_Kind seed_kind;

	Description_box bottom_title;
	Description_box bottom_rect;
	Description_box bottom_description;

	Description_box top_box;
	Description_box top_description;

	bool is_selling;
	bool is_closed;
	bool is_ready;

public:

	virtual HRESULT init();
	virtual HRESULT init(float x, float y, int num);
	virtual HRESULT init(float x, float y, bool is_taken);
	virtual void release();
	void update();
	void render();

	inline void set_quantity(int val) { quantity += val; }
	inline int get_quantity() { return quantity; }

	inline void set_taken(bool val) { object->is_taken = val; }
	bool get_taken(int index) { return object->is_taken; }

	inline void set_angle(float val) { object->angle = val; }

	inline void set_isClosed(bool val) { is_closed = val; }
	inline bool get_isClosed() { return is_closed; }

	inline void set_isSelling(bool val) { is_selling = val; }
	inline bool get_isSelling() { return is_selling; }

	inline int get_price() { return price; }
	inline Crop_Kind get_seedKind() { return seed_kind; }
	inline ItemType get_itemType() { return type; }
	inline GunType get_gunType() { return gun_type; }

	ItemObject* get_itemObject() { return object; }
	

	inline void removeObject() { delete object; object = NULL; }
};

