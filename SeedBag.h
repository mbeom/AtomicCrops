#pragma once
#include "Item.h"

class SeedBag : public Item
{
private:

public:

	virtual HRESULT init();
	virtual HRESULT init(float x, float y, int num);
	virtual HRESULT init(float x, float y, bool is_taken);
	virtual void release();
};

